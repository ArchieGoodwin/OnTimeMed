//
//  ExtractPackAppDelegate.h
//  ExtractPack
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import <UIKit/UIKit.h>

#include "PackageInfo.h"
#import "MainViewController.h"	

enum {
	VMODE_FIRST = 0,
    VMODE_CAPTURE,
	VMODE_CAMERA,
	VMODE_FRAME,
};

@class ExtractPackViewController;
@class FrameViewController;
@class CaptureViewController;

@interface ExtractPackAppDelegate : UIResponder <UIApplicationDelegate>
{

@public
    CVLib::PackageInfo  m_packageInfo;
	IBOutlet MainViewController *mainViewController;
    UIWindow *window;
}
@property (nonatomic, strong) UIStoryboard *initalStoryboard;

@property (nonatomic, strong) IBOutlet UIWindow *window;
@property (readonly, strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (readonly, strong, nonatomic) NSManagedObjectModel *managedObjectModel;
@property (readonly, strong, nonatomic) NSPersistentStoreCoordinator *persistentStoreCoordinator;
//@property (strong, nonatomic) CaptureViewController     *captureViewController;

@property (nonatomic, strong) UIImage*  m_cropedImage;
@property (nonatomic, assign) bool      m_fCapturing;

- (void)	toggleView: (id) sender nVMode:(int)nVMode nState: (int) nState;
-(void)showCapture;
-(void)clearNotifications;
- (void)resetWindowToInitialView;
@end

