//
//  MainViewController.h
//  MainViewController
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import <UIKit/UIKit.h>


@interface MainViewController : UIViewController
{
    int m_nVMode;
	int m_nState;
	//temp variants
	UIViewController* controller1;
	UIViewController* controller2;
    
@public	UIViewController* activeViewController;
}

@property (nonatomic, strong) NSString *keptBarCode;
- (void) changeView: (int) nVMode;
- (void) showWindow;
- (void) replaceSubview:(UIView *)oldView withSubview:(UIView *)newView transition:(NSString *)transition direction:(NSString *)direction duration:(NSTimeInterval)duration;
- (void) toggleView: (int)nVMode nState: (int) nState;
- (int)	 getVMode;
- (int) getSubStatus;
-(void)closeMeWithBarcode:(NSString *)barcode;
-(void)closeMe;
-(void)closeAndReturn;
-(void)showFrame;
-(void)showCaptureView;
@end;
