#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>

#import <CoreMedia/CoreMedia.h>
#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CoreVideo.h>
#import "MainViewController.h"
#define HAS_VIDEO_CAPTURE (__IPHONE_OS_VERSION_MIN_REQUIRED >= 40000 && TARGET_OS_EMBEDDED)


@interface CaptureViewController : UIViewController <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    NSTimer* _timer;
    CVLib::FrameDetector _frameDetector;
}
@property (nonatomic, strong) CALayer *overlayLayer;
@property (nonatomic, strong) AVCaptureVideoPreviewLayer *previewLayer;
@property (nonatomic, strong) AVCaptureSession *session;

@property (nonatomic, strong) MainViewController *parentController;


- (IBAction)dismiss:(id)sender;

- (CGAffineTransform)affineTransformForVideoFrame:(CGRect)videoFrame orientation:(AVCaptureVideoOrientation)videoOrientation;
- (void)startCameraCapture;
- (AVCaptureDevice *)frontFacingCamera;
- (IBAction)OnCancel:(id)sender;

@end
