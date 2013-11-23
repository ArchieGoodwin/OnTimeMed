#import "CaptureViewController.h"
#import "ExtractPackAppDelegate.h"
#import "ExtractPackViewController.h"
#import "FrameViewController.h"

#import "MainViewController.h"
static void convertImage (cv::Mat& mat1, CVLib::Mat& mat2)
{
    if (mat1.elemSize() == 1)
    {
        mat2.Create (mat1.rows, mat1.cols, CVLib::MAT_Tbyte);
        int k=0;
        for (int i=0; i< mat2.Rows(); i++)
        {
            for (int j=0; j<mat2.Cols(); j++)
                mat2.data.ptr[i][j]=mat1.data[k++];
		}
	}
    else
    {
        mat2.Create (mat1.rows, mat1.cols, CVLib::MAT_Tbyte3);
        int k=0;
        for (int i=0; i< mat2.Rows(); i++)
        {
            for (int j=0, kk=0; j<mat2.Cols(); j++, kk+=3,k+=4)
            {
                mat2.data.ptr[i][kk+0]=mat1.data[k+2]; // R
                mat2.data.ptr[i][kk+1]=mat1.data[k+1]; // G
                mat2.data.ptr[i][kk+2]=mat1.data[k+0]; // B
            }
		}
    }
}

void dataProviderReleaseCallback(void* info, const void* data, size_t size)
{
    free ((void*)data);
}

static UIImage* Mat2UIImage(const CVLib::Mat& img)
{
    UIImage *newImage = nil;
    
    int width = img.Cols();
	int height = img.Rows();
	uchar *rawImageData = (uchar*) malloc(height * width * 4);
    int nrOfColorComponents = 4; //RGBA
    int bitsPerColorComponent = 8;
    int rawImageDataLength = width * height * nrOfColorComponents;
    BOOL interpolateAndSmoothPixels = NO;
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast;//kCGBitmapByteOrderDefault;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    
    CGDataProviderRef dataProviderRef;
    CGColorSpaceRef colorSpaceRef;
    CGImageRef imageRef;
    
    for(int i=0,kk=0;i<rawImageDataLength;i+=4, kk+=3 )
    {
        {
            rawImageData[i]  =img.data.ptr[0][kk];
            rawImageData[i+1]=img.data.ptr[0][kk+1];
            rawImageData[i+2]=img.data.ptr[0][kk+2];
            rawImageData[i+3]=255;
        }
    }
    @try
    {
        GLubyte *rawImageDataBuffer = rawImageData;
        dataProviderRef = CGDataProviderCreateWithData(NULL, rawImageDataBuffer, rawImageDataLength, dataProviderReleaseCallback);
        
        colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        imageRef = CGImageCreate(width, height, bitsPerColorComponent, bitsPerColorComponent * nrOfColorComponents, width * nrOfColorComponents, colorSpaceRef, bitmapInfo, dataProviderRef, NULL, interpolateAndSmoothPixels, renderingIntent);
        newImage = [UIImage imageWithCGImage:imageRef scale:1.0f orientation:UIImageOrientationUp];
    }
    @finally
    {
        CGDataProviderRelease(dataProviderRef);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
    }
    dataProviderRef = nil;
    return newImage;
}

@interface CaptureViewController ()
- (void)processFrame:(cv::Mat&)mat videoRect:(CGRect)rect videoOrientation:(AVCaptureVideoOrientation)orientation;
@end

@implementation CaptureViewController
@synthesize overlayLayer;
@synthesize previewLayer;
@synthesize session;

#pragma mark -
#pragma mark UIViewControllerDelegate

static int delayParam=0;
- (void)viewDidLoad
{
	[super viewDidLoad];
	//[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackOpaque animated:YES];
    
    //    _nBorderThickness = 2;
    //    [self getBorderRect];
    delayParam=0;
    [self startCameraCapture];
    //    [self.view addSubview:_backButton];
    
    [self performSelector:@selector(showBorder) withObject:self afterDelay:0.2];
}
- (void)viewDidUnload;
{
    self.previewLayer = nil;
    self.overlayLayer = nil;
    self.session = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
- (void)startCameraCapture
{
    // Create the AVCapture
	NSLog(@"startcameracapture");
    //#if HAS_VIDEO_CAPTURE
	self.session = [[AVCaptureSession alloc] init];
    
    /// Create a preview layer to show the output from the camera
    self.previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:session];
    [previewLayer setFrame:self.view.bounds];
    previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
    [self.view.layer insertSublayer:previewLayer atIndex:0];
	
	// Create recognition rectangle overlay
	self.overlayLayer = [CALayer layer];
    [overlayLayer setFrame:CGRectMake(0.0f,0.0f,320.0f,440.0f)];
	overlayLayer.borderColor = [[UIColor greenColor] CGColor];
    overlayLayer.borderWidth = 5.0f;
    //	overlayLayer.delegate = self;
	[self.view.layer addSublayer:overlayLayer];
	
	AVCaptureDevice *camera = [self frontFacingCamera];
	
	// Create a AVCaptureInput with the camera device
	NSError *error = nil;
	AVCaptureInput *cameraInput = [[AVCaptureDeviceInput alloc] initWithDevice:camera error:&error];
	if(cameraInput == nil) {
		NSLog(@"Error to create camera capture:%@", error);
	}
	
	// Set the outpua
	AVCaptureVideoDataOutput *videoOutput = [[AVCaptureVideoDataOutput alloc] init];
	videoOutput.alwaysDiscardsLateVideoFrames = YES;
	
	// create a queue besides the main thread queue to run the capture on
	dispatch_queue_t captureQueue = dispatch_queue_create("captureQueue", NULL);
	
	// setup our delegate
	[videoOutput setSampleBufferDelegate:self queue:captureQueue];
	
	// release the queue.  I still don't entirely understand why we're release it here,
	// but the code examples I've found indicate this is the right thing.
	//dispatch_release(captureQueue);
	
	// configure the pixel format
	
	[videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]]; // BGRA is necessary for manual preview
	
	// and the size of the frames we want
	// try AVCaptureSessionPresetLow if this is too slow...
	[session setSessionPreset:AVCaptureSessionPresetMedium];
    //	[session setSessionPreset:AVCaptureSessionPresetLow];
	// If you wish to cap the frame rate to a known value, such as 10 fps, set
	// minFrameDuration.
    //	videoOutput.minFrameDuration = CMTimeMake(1, 10);
	
	// Add the input and output
	[session addInput:cameraInput];
	[session addOutput:videoOutput];
	
	// Start the session
	[session startRunning];
    
    //  _timer = [[NSTimer scheduledTimerWithTimeInterval:0.02 target:self selector:@selector(showBorder) userInfo:nil repeats:YES] retain];
}

- (AVCaptureDevice *)frontFacingCamera {
	NSArray *cameras = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	for (AVCaptureDevice *device in cameras) {
		if(device.position == AVCaptureDevicePositionBack) {
			return device;
		}
	}
	return [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    @autoreleasepool {
        ExtractPackAppDelegate* appdelegate = [UIApplication sharedApplication].delegate;
        
        
        CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
        OSType format = CVPixelBufferGetPixelFormatType(pixelBuffer);
        CGRect videoRect = CGRectMake(0.0f, 0.0f, CVPixelBufferGetWidth(pixelBuffer), CVPixelBufferGetHeight(pixelBuffer));
        //    AVCaptureVideoOrientation videoOrientation = [[session objectAtIndex:0] videoOrientation];
        AVCaptureVideoOrientation videoOrientation = AVCaptureVideoOrientationLandscapeRight;
        
        if (format == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
            // For grayscale mode, the luminance channel of the YUV data is used
            CVPixelBufferLockBaseAddress(pixelBuffer, 0);
            void *baseaddress = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
            
            if ( appdelegate.m_fCapturing)
            {
                cv::Mat mat(videoRect.size.height, videoRect.size.width, CV_8UC1, baseaddress, 0);
                [self processFrame:mat videoRect:videoRect videoOrientation:videoOrientation];
            }
            
            CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
        }
        else if (format == kCVPixelFormatType_32BGRA) {
            // For color mode a 4-channel cv::Mat is created from the BGRA data
            CVPixelBufferLockBaseAddress(pixelBuffer, 0);
            void *baseaddress = CVPixelBufferGetBaseAddress(pixelBuffer);
            
            if ( appdelegate.m_fCapturing)
            {
                cv::Mat mat(videoRect.size.height, videoRect.size.width, CV_8UC4, baseaddress, 0);
                [self processFrame:mat videoRect:videoRect videoOrientation:videoOrientation];
            }
            
            CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
        }
        else {
            NSLog(@"Unsupported video format");
        }

    }
    
    
    
    
    
}
- (void)processFrame:(cv::Mat &)mat videoRect:(CGRect)rect videoOrientation:(AVCaptureVideoOrientation)videOrientation
{
    float rscaleX=mat.cols/240.0f;
    float rscaleY=rscaleX;
    // face detect
    // Shrink video frame to 320X240
    cv::Mat resized;
    cv::resize(mat, resized, cv::Size(), 1/rscaleX, 1/rscaleY, CV_INTER_LINEAR);
    rect.size.width /= rscaleX;
    rect.size.height /= rscaleY;
    
    cv::transpose(resized, resized);
    CGFloat temp = rect.size.width;
    rect.size.width = rect.size.height;
    rect.size.height = temp;
    
    if (videOrientation == AVCaptureVideoOrientationLandscapeRight)
    {
        // flip around y axis for back camera
        cv::flip(resized, resized, 1);
    }
    else {
        // Front camera output needs to be mirrored to match preview layer so no flip is required here
    }
    
    videOrientation = AVCaptureVideoOrientationPortrait;
    
    CVLib::Mat realMat;
    CVLib::Vector<CVLib::Point2i> borders;
	NSLog(@"process frame");
    
    if (delayParam < 3)
    {
        delayParam++;
        return ;
    }
    if (1){
        convertImage (resized, realMat);
        if (!_frameDetector.flagInit())
        {
            ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
            _frameDetector.init(realMat, appDelegate->m_packageInfo);
        }
        
        bool state=_frameDetector.detect (realMat);
        
        if (state == true)
        {
            _frameDetector.getCardRegion(borders);
            
            int i;
            CVLib::Vector<CVLib::Point2i> realborders(borders.GetSize());
            // real position
            for (i=0; i<borders.GetSize(); i++)
            {
                realborders[i].x=(int)(borders[i].y*rscaleX+0.5f);
                realborders[i].y=(int)((realMat.Cols()-borders[i].x-1)*rscaleY);
                
                NSMutableString *myString = [NSMutableString string];
                NSString* myNewString = [NSString stringWithFormat:@"%d %d %d", i, realborders[i].x, realborders[i].y];
                [myString appendString:myNewString];
                NSLog(@"point %@", myString);  // result: myString = 0
            }
            
            CVLib::Mat frame, cropmat;
            convertImage (mat, frame);
            _frameDetector.crop (frame, cropmat, realborders);
            
            UIImage* pui=Mat2UIImage (cropmat);
            UIImage* dateImg=nil;
            {
                ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
                
                NSDate *localDate = [NSDate date];
                NSDateFormatter *dateFormatter = [[NSDateFormatter alloc]init];
                dateFormatter.dateFormat = @"MM/dd/yy";
                NSMutableString *dateString = [NSMutableString string];
                NSString *dateString2 = [dateFormatter stringFromDate: localDate] ;
                [dateString appendString:dateString2];
                
                /*                NSDateFormatter *timeFormatter = [[[NSDateFormatter alloc]init]autorelease];
                 timeFormatter.dateFormat = @"HH:mm:ss";
                 NSString *timeString = [timeFormatter stringFromDate: localDate];*/
                NSString* packageID2=[NSString stringWithFormat:@"%s",appDelegate->m_packageInfo.szID];
                NSMutableString* packageID=[NSMutableString string];
                [packageID appendString:packageID2];
                
                CVLib::COLORREF rgb=appDelegate->m_packageInfo.dotColor;
                NSString* cornerColorString2=[NSString stringWithFormat:@"%d,%d,%d",GetRValue(rgb),GetGValue(rgb),GetBValue(rgb)];
                NSMutableString* cornerColorString=[NSMutableString string];
                [cornerColorString appendString:cornerColorString2];
                
                NSString* packageItemID2=[NSString stringWithFormat:@"%s", appDelegate->m_packageInfo.szItemCode];
                NSMutableString* packageItemID=[NSMutableString string];
                [packageItemID appendString:packageItemID2];
                
                CGSize size=[pui size];
                UIGraphicsBeginImageContext(size);
                
                // draw original image into the context
                [pui drawAtPoint:CGPointZero];
                UIFont *font = [UIFont systemFontOfSize:20.0];
                CGContextRef ctx = UIGraphicsGetCurrentContext();
                
                CGContextSetShadowWithColor(ctx, CGSizeMake(1.0, 1.0), 5.0, [[UIColor brownColor] CGColor]);
                
                CGFloat startx=160.0, starty=50.0;
                CGFloat linegap=40.0;
                
                NSMutableParagraphStyle *textStyle = [[NSMutableParagraphStyle defaultParagraphStyle] mutableCopy];
                textStyle.lineBreakMode = NSLineBreakByWordWrapping;
                textStyle.alignment = NSTextAlignmentCenter;

                // draw in context, you can use also drawInRect:withFont:
                [dateString drawAtPoint:CGPointMake(startx, starty) withAttributes:@{NSFontAttributeName:font, NSParagraphStyleAttributeName:textStyle}];
                [packageID drawAtPoint:CGPointMake(startx, starty+linegap) withAttributes:@{NSFontAttributeName:font, NSParagraphStyleAttributeName:textStyle}];
                [cornerColorString drawAtPoint:CGPointMake(startx, starty+linegap*2) withAttributes:@{NSFontAttributeName:font, NSParagraphStyleAttributeName:textStyle}];
                [packageItemID drawAtPoint:CGPointMake(startx, starty+linegap*3) withAttributes:@{NSFontAttributeName:font, NSParagraphStyleAttributeName:textStyle}];
                
                //CGImageRef cimg = UIGraphicsGetCurrentContext();
                
                dateImg = UIGraphicsGetImageFromCurrentImageContext();
                UIGraphicsEndImageContext();
                
            }
            
            ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
            
            if (appDelegate.m_fCapturing)
            {
                appDelegate.m_cropedImage = [[UIImage alloc] initWithCGImage:[dateImg CGImage]];
                NSLog(@"image here %f", appDelegate.m_cropedImage.size.width);
            }
            
            appDelegate.m_fCapturing = false;
            
           
            
            //  [self performSelector:@selector(presentCaptureViewController) withObject:self afterDelay:1];
            dispatch_sync(dispatch_get_main_queue(), ^{
                NSLog(@"view frameViewController");
                //[self.previewLayer removeFromSuperlayer];
                //[self.overlayLayer removeFromSuperlayer];
                [self closing];
                
                //[self performSelector:@selector(dismissViewController) withObject:self afterDelay:0.2];

                
                [_parentController toggleView:VMODE_FRAME nState:0];
                //[appDelegate->mainViewController closeMe];
                //[appDelegate->mainViewController toggleView:VMODE_FRAME nState:0];
                /*
                 FrameViewController* vc = [[FrameViewController alloc] initWithNibName:@"FrameViewController" bundle:nil];
                 [self presentViewController: vc animated:YES completion:nil];
                 [vc release];*/
            });
        }
    }
}
// MARK: Geometry methods

// Create an affine transform for converting CGPoints and CGRects from the video frame coordinate space to the
// preview layer coordinate space. Usage:
//
// CGPoint viewPoint = CGPointApplyAffineTransform(videoPoint, transform);
// CGRect viewRect = CGRectApplyAffineTransform(videoRect, transform);
//
// Use CGAffineTransformInvert to create an inverse transform for converting from the view cooridinate space to
// the video frame coordinate space.
//
// videoFrame: a rect describing the dimensions of the video frame
// video orientation: the video orientation
//
// Returns an affine transform
//
- (CGAffineTransform)affineTransformForVideoFrame:(CGRect)videoFrame orientation:(AVCaptureVideoOrientation)videoOrientation
{
    CGSize viewSize = self.view.bounds.size;
    //    NSString * const videoGravity = previewLayer.videoGravity;
    CGFloat widthScale = 1.0f;
    CGFloat heightScale = 1.0f;
    
    // Move origin to center so rotation and scale are applied correctly
    CGAffineTransform t = CGAffineTransformMakeTranslation(-videoFrame.size.width / 2.0f, -videoFrame.size.height / 2.0f);
    
    switch (videoOrientation) {
        case AVCaptureVideoOrientationPortrait:
            widthScale = viewSize.width / videoFrame.size.width;
            heightScale = viewSize.height / videoFrame.size.height;
            break;
            
        case AVCaptureVideoOrientationPortraitUpsideDown:
            t = CGAffineTransformConcat(t, CGAffineTransformMakeRotation(M_PI));
            widthScale = viewSize.width / videoFrame.size.width;
            heightScale = viewSize.height / videoFrame.size.height;
            break;
            
        case AVCaptureVideoOrientationLandscapeRight:
            t = CGAffineTransformConcat(t, CGAffineTransformMakeRotation(M_PI_2));
            widthScale = viewSize.width / videoFrame.size.height;
            heightScale = viewSize.height / videoFrame.size.width;
            break;
            
        case AVCaptureVideoOrientationLandscapeLeft:
            t = CGAffineTransformConcat(t, CGAffineTransformMakeRotation(-M_PI_2));
            widthScale = viewSize.width / videoFrame.size.height;
            heightScale = viewSize.height / videoFrame.size.width;
            break;
    }
    
    // Adjust scaling to match video gravity mode of video preview
    /*   if (videoGravity == AVLayerVideoGravityResizeAspect) {
     heightScale = MIN(heightScale, widthScale);
     widthScale = heightScale;
     }
     else if (videoGravity == AVLayerVideoGravityResizeAspectFill) {
     heightScale = MAX(heightScale, widthScale);
     widthScale = heightScale;
     }*/
    
    // Apply the scaling
    t = CGAffineTransformConcat(t, CGAffineTransformMakeScale(widthScale, heightScale));
    
    // Move origin back from center
    t = CGAffineTransformConcat(t, CGAffineTransformMakeTranslation(viewSize.width / 2.0f, viewSize.height / 2.0f));
    
    return t;
}

- (void)showBorder
{
	[overlayLayer setNeedsDisplay];
}

-(void)closing
{
    [_timer invalidate];
    AVCaptureInput* input = [session.inputs objectAtIndex:0];
    [session removeInput:input];
    AVCaptureVideoDataOutput* output = [session.outputs objectAtIndex:0];
    [session removeOutput:output];
    
    [previewLayer removeFromSuperlayer];
    [overlayLayer removeFromSuperlayer];
    
    [session stopRunning];
}

- (IBAction)dismiss:(id)sender
{
    [self closing];
    [self performSelector:@selector(dismissViewController) withObject:self afterDelay:1];
    
}
- (void)dismissViewController
{
    [self dismissViewControllerAnimated:YES completion:nil];
}


- (IBAction)OnCancel:(id)sender
{
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
	[appDelegate->mainViewController toggleView: VMODE_CAPTURE nState:0];
    appDelegate.m_fCapturing = false;
    //    ExtractPackViewController* extractViewController = [[[ExtractPackViewController alloc] initWithNibName:@"ExtractPackViewController" bundle:nil] autorelease];
    //
    //    [self presentViewController:extractViewController animated:YES completion:nil];
    //    [extractViewController release];
    //    [self dismissViewControllerAnimated:YES completion:nil];
    [self performSelector:@selector(dismissViewController) withObject:self afterDelay:1];
}
@end
