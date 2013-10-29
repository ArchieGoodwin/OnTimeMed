//
//  ExtractPackViewController.m
//  ExtractPack
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import "MainViewController.h"
#import "CaptureViewController.h"
#import "ExtractPackAppDelegate.h"
#import "FrameViewController.h"
#import "CaptureScreenViewController.h"
#import "ExtractPackViewController.h"
#import "MEDCenterTableViewController.h"
#import "InstructionsVC.h"
@interface MainViewController ()
{
}

@end

@implementation MainViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
   
    m_nVMode = VMODE_FIRST;
	activeViewController = [self loadExtractPackViewController];
	
	[self.view addSubview:activeViewController.view];
	[activeViewController viewDidAppear:NO];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (UIViewController*) loadExtractPackViewController {
	ExtractPackViewController *viewController = [ExtractPackViewController alloc];
    viewController.parentController = self;
    if(_package)
    {
        viewController.package = _package;
    }
	return viewController;
}

- (UIViewController*) loadCaptureScreenViewController{
	CaptureScreenViewController *viewController = [CaptureScreenViewController alloc];
	return viewController;
   
}
- (UIViewController*) loadCaptureViewController {
	CaptureViewController *viewController = [CaptureViewController alloc];
    viewController.parentController = self;
    if(_package)
    {
        viewController.package = _package;
    }
	return viewController;
}

- (UIViewController*) loadFrameViewContoller {
	FrameViewController *viewController = [FrameViewController alloc];
    viewController.parentController = self;
    if(_package)
    {
        viewController.package = _package;
    }
	return viewController;
}

- (UIViewController*) loadInstructions {
	InstructionsVC *viewController = [InstructionsVC alloc];
    viewController.parentController = self;
    viewController.package = _package;
	return viewController;
}



-(void)closeMe
{
    
    [self dismissViewControllerAnimated:YES completion:nil];
    
    
    [self.navigationController popToRootViewControllerAnimated:YES];
}

-(void)closeMeWithBarcode:(NSString *)barcode
{
    NSLog(@"closeMeWithBarcode %@", barcode);
    
    _keptBarCode = barcode;
    
    [self showCaptureView];

}

-(void)showCaptureView
{
    ExtractPackAppDelegate* appdelegate = [UIApplication sharedApplication].delegate;
    appdelegate.m_fCapturing = YES;
    
    [self toggleView:VMODE_CAMERA nState:0];
}

-(void)showInstructions
{
    ExtractPackAppDelegate* appdelegate = [UIApplication sharedApplication].delegate;
    appdelegate.m_fCapturing = YES;
    
    [self toggleView:VMODE_INSTRUCTIONS nState:0];
}


-(void)showFrame
{
    ExtractPackAppDelegate* appdelegate = [UIApplication sharedApplication].delegate;
    appdelegate.m_fCapturing = NO;
    [self toggleView:VMODE_FRAME nState:0];
}

-(void)closeAndReturn
{
    NSLog(@"closeAndReturn");

    [[NSNotificationCenter defaultCenter] postNotificationName:@"checkBarcodeOnServer" object:_keptBarCode];
    
    [self.navigationController popToRootViewControllerAnimated:YES];
    
}

-(void)closeAndReturnWhenTakingMed
{
    NSLog(@"closeAndReturnWhenTakingMed");
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"takingMed" object:_package];
    
    [self.navigationController popToRootViewControllerAnimated:YES];
    
}



- (void) toggleView: (int)nVMode nState: (int) nState {
	m_nVMode = nVMode;
	m_nState = nState;
	[self changeView:nVMode];
}

- (int) getState: (id) sender {
	return m_nState;
}

- (int)	 getVMode {
	return m_nVMode;
}

- (UIViewController*) getController: (int) nVMode {
	UIViewController* pResult = nil;
	switch(nVMode) {
		case VMODE_FIRST:
			pResult = [self loadExtractPackViewController];
			break;
        case VMODE_CAPTURE:
            pResult = [self loadCaptureScreenViewController];
            break;
		case VMODE_CAMERA:
			pResult = [self loadCaptureViewController];
			break;
		case VMODE_FRAME:
			pResult = [self loadFrameViewContoller];
			break;
        case VMODE_INSTRUCTIONS: ;
            pResult = [self loadInstructions];
			break;
            
	}
	
	return pResult;
}

- (void) changeView: (int) nVMode {
	self.view.userInteractionEnabled=YES;
	controller1 = activeViewController;
	controller2 = [self getController:nVMode];
	UIView *mainView = controller1.view;
	UIView *flipsideView = controller2.view;
	
	[self replaceSubview:mainView withSubview:flipsideView transition:kCATransitionFade direction:kCATransitionFromLeft duration:0.2];
	[controller1 viewDidDisappear:YES];
	[controller2 viewDidAppear:YES];
	activeViewController = controller2;
}

- (void) showWindow {}

- (void)replaceSubview:(UIView *)oldView withSubview:(UIView *)newView transition:(NSString *)transition direction:(NSString *)direction duration:(NSTimeInterval)duration {
	
    
    //[oldView removeFromSuperview];
    //[self.view addSubview:newView];
    
    
	NSArray *subViews = [self.view subviews];
	NSUInteger index;
	
    
    
    
	if ([oldView superview] == self.view) {
		// Find the index of oldView so that we can insert newView at the same place
		for(index = 0; [subViews objectAtIndex:index] != oldView; ++index) {}
		[oldView removeFromSuperview];
	}
	
	// If there's a new view and it doesn't already have a superview, insert it where the old view was
	if (newView && ([newView superview] == nil))
		[self.view addSubview:newView];// atIndex:index];
}

- (int) getSubStatus {
	return m_nState;
}

@end
