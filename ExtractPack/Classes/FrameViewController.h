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
    
}
@property (nonatomic, strong) IBOutlet UIImageView *resultImageView;
@property (nonatomic, strong) MainViewController *parentController;
@property (nonatomic, strong) MedPackage *package;

- (IBAction) continueButtonTapped;
- (IBAction) retryButtonTapped;

- (NSString*) saveImageFile: (UIImage*) image;
@end
