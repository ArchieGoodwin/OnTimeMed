//
//  ExtractPackViewController.h
//  ExtractPack
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MainViewController.h"
#import "MedPackage.h"
@interface ExtractPackViewController : UIViewController <ZBarReaderDelegate, ZBarReaderViewDelegate >
{
///    UITextView *resultText;
}
@property (nonatomic, strong) MedPackage *package;
@property (strong, nonatomic) IBOutlet UILabel *lblMessage;

@property (nonatomic, retain) IBOutlet UITextView *resultText;
@property (nonatomic, strong) MainViewController *parentController;
- (IBAction) scanButtonTapped;
- (IBAction) caputreImageButtonTapped;

- (NSString*) saveImageFile: (UIImage*) image;
@end;
