//
//  FrameViewController.m
//  ExtractPack
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import "FrameViewController.h"
#import "CaptureViewController.h"
#import "ExtractPackAppDelegate.h"
#import <OpenAL/al.h>
#import <AVFoundation/AVFoundation.h>

//UInt32 beepSound;
AVAudioPlayer* mplayer;


@interface FrameViewController ()
{
}
@end


@implementation FrameViewController
//@synthesize resultImage;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        
    }
    return self;
}
- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    ExtractPackAppDelegate* appdelegate = [UIApplication sharedApplication].delegate;
    _resultImageView.image = appdelegate.m_cropedImage;
    
    NSString* resourcePath=[[NSBundle mainBundle] pathForResource:@"beep-7" ofType:@"mp3"];
    
    NSError* err;
    mplayer=[[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL fileURLWithPath:resourcePath] error:&err];
    if (err) {
        NSLog(@"Failed with reason: %@", [err localizedDescription]);
    }
    else{
        //        mplayer.delegate=self;
    }
    
    [mplayer play];
	// Do any additional setup after loading the view, typically from a nib.
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView.tag == 1001)
    {
        if(buttonIndex == 0)
        {
            
        }
        else
        {
            UIImageWriteToSavedPhotosAlbum (_resultImageView.image, nil, nil, nil);
            
        }
        
        ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
        appDelegate.m_fCapturing=false;
        
        if(_package)
        {
            [_parentController closeAndReturnWhenTakingMed];

        }
        else
        {
            [_parentController closeAndReturn];

        }
        [self dismissViewControllerAnimated:YES completion:^{
            
        }];
    }
    
}


- (IBAction) continueButtonTapped
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Image saving" message:@"Do you want to save this image in Photos?" delegate:self cancelButtonTitle:@"NO" otherButtonTitles:@"YES", nil];
    alert.tag = 1001;
    
    [alert show];
    
   
    //[appDelegate->mainViewController toggleView:VMODE_FIRST nState:0];
}
- (IBAction) retryButtonTapped
{
    [self presentCaptureViewController];
}

#define DOCSFOLDER [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]
- (NSString*) saveImageFile: (UIImage*) image {
    int i = 0;
    NSString *path = [DOCSFOLDER stringByAppendingPathComponent:@"selectedImage.png"];
    while ([[NSFileManager defaultManager] fileExistsAtPath:path])
        path = [NSString stringWithFormat:@"%@/%@-%d.%@", DOCSFOLDER, @"selectedImage", ++i, @"png"];
    printf("Writing selected image to Documents folder\n");
    
    [UIImagePNGRepresentation(image)  writeToFile: path atomically:YES];
    return path;
}

- (void) postprocess
{
    [self dismissViewControllerAnimated:YES completion:nil];
    ExtractPackAppDelegate* appdelegate = [UIApplication sharedApplication].delegate;
    appdelegate.m_fCapturing=true;
}

- (void)presentCaptureViewController
{
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    appDelegate.m_fCapturing=true;
    [self.view removeFromSuperview];
    [_parentController toggleView:VMODE_CAMERA nState:0];
}

- (void)viewDidUnload {
    [self setResultImageView:nil];
    [super viewDidUnload];
}
@end
