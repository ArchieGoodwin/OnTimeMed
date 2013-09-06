//
//  CaptureScreenViewController.m
//  ExtractPack
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import "CaptureScreenViewController.h"
#import "CaptureViewController.h"
#import "ExtractPackAppDelegate.h"

@interface CaptureScreenViewController ()
{
}

@end

@implementation CaptureScreenViewController
//@synthesize resultText;

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


- (IBAction) captureImageButtonTapped
{
    [self presentCaptureViewController];
}

- (void)presentCaptureViewController
{
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    appDelegate.m_fCapturing = true;
	[appDelegate->mainViewController toggleView: VMODE_CAMERA nState:0];
}


@end
