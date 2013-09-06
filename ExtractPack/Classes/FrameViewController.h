//
//  ExtractPackViewController.h
//  ExtractPack
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MainViewController.h"

@interface FrameViewController : UIViewController <UIAlertViewDelegate>
{
//    IBOutlet UIImageView *resultImageView;
}
@property (nonatomic, strong) IBOutlet UIImageView *resultImageView;
@property (nonatomic, strong) MainViewController *parentController;

- (IBAction) continueButtonTapped;
- (IBAction) retryButtonTapped;

- (NSString*) saveImageFile: (UIImage*) image;
@end
