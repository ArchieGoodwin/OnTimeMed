//
//  ExtractPackAppDelegate.m
//  ExtractPack
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import "ExtractPackAppDelegate.h"

#import "ExtractPackViewController.h"
#import "FrameViewController.h"

#import <QuartzCore/QuartzCore.h>
#import "MEDNetworkHelper.h"
#import "MedPackage.h"
#import "OTMhelper.h"
@implementation ExtractPackAppDelegate
@synthesize managedObjectContext = _managedObjectContext;
@synthesize managedObjectModel = _managedObjectModel;
@synthesize persistentStoreCoordinator = _persistentStoreCoordinator;

@synthesize window;
@synthesize m_cropedImage;
@synthesize m_fCapturing;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
//    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
    
    
    
	
	
    
    application.applicationIconBadgeNumber = 0;

    _initalStoryboard = self.window.rootViewController.storyboard;

    
    /*UIViewController * leftSideDrawerViewController = [[MEDLeftSideDrawerViewController alloc] init];
    
    UIViewController * centerViewController = [[MEDCenterTableViewController alloc] initWithStyle:UITableViewStyleGrouped];
    
    UIViewController * rightSideDrawerViewController = [[MEDRightSideDrawerViewController alloc] init];
    
    UINavigationController * navigationController = [[UINavigationController alloc] initWithRootViewController:centerViewController];
    
    MMDrawerController * drawerController = [[MMDrawerController alloc]
                                             initWithCenterViewController:navigationController
                                             leftDrawerViewController:leftSideDrawerViewController
                                             rightDrawerViewController:rightSideDrawerViewController];
    [drawerController setMaximumRightDrawerWidth:200.0];
    [drawerController setOpenDrawerGestureModeMask:MMOpenDrawerGestureModeAll];
    [drawerController setCloseDrawerGestureModeMask:MMCloseDrawerGestureModeAll];
    
    [drawerController  setDrawerVisualStateBlock:^(MMDrawerController *drawerController, MMDrawerSide drawerSide, CGFloat percentVisible) {
         MMDrawerControllerDrawerVisualStateBlock block;
         block = [[MEDDrawerVisualStateManager sharedManager]
                  drawerVisualStateBlockForDrawerSide:drawerSide];
         if(block){
             block(drawerController, drawerSide, percentVisible);
         }
     }];
    */
    //self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
   // [self.window setRootViewController:drawerController];
    // Override point for customization after application launch.
    //self.window.backgroundColor = [UIColor whiteColor];
    //[self.window makeKeyAndVisible];
    

    
    return YES;
}

- (void)resetWindowToInitialView
{
    for (UIView* view in self.window.subviews)
    {
        [view removeFromSuperview];
    }
    
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    
    UIViewController* initialScene = [_initalStoryboard instantiateInitialViewController];
    self.window.rootViewController = initialScene;
}

-(void)clearNotifications
{
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber: 1];
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber: 0];
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

- (void)application:(UIApplication *)app didReceiveLocalNotification:(UILocalNotification *)notif {
	// Handle the notificaton when the app is running
	NSLog(@"Recieved Notification %@",notif);
    
    NSDictionary *infoDict = notif.userInfo;
    /*[[MEDNetworkHelper sharedInstance] postEvent:EVENTTakeMedication packageid:[[infoDict objectForKey:@"packageId"] integerValue] completionBlock:^(BOOL result, NSError *error) {
        NSLog(@"EVENTTakeMedication sent");
    }];*/
    
    dispatch_async(dispatch_get_main_queue(),^{
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:notif.alertAction message:notif.alertBody delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
        [alert show];
        
    });
   
}

-(void)showCapture
{
    [self.window addSubview:mainViewController->activeViewController.view];
    [self.window makeKeyAndVisible];
    
    
    m_fCapturing = true;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
- (void) toggleView: (id) sender nVMode:(int)nVMode nState: (int) nState {
	[mainViewController toggleView: nVMode nState: nState];
}

- (int) getSubStatus {
	return [mainViewController getSubStatus];
}


#pragma mark - Core Data stack

// Returns the managed object context for the application.
// If the context doesn't already exist, it is created and bound to the persistent store coordinator for the application.
- (NSManagedObjectContext *)managedObjectContext
{
    if (_managedObjectContext != nil) {
        return _managedObjectContext;
    }
    
    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (coordinator != nil) {
        _managedObjectContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];
        [_managedObjectContext setPersistentStoreCoordinator:coordinator];
        //_managedObjectContext.undoManager = [[NSUndoManager alloc] init];
        
    }
    return _managedObjectContext;
}

// Returns the managed object model for the application.
// If the model doesn't already exist, it is created from the application's model.
- (NSManagedObjectModel *)managedObjectModel
{
    if (_managedObjectModel != nil) {
        return _managedObjectModel;
    }
    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"Model" withExtension:@"momd"];
    _managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    return _managedObjectModel;
}

// Returns the persistent store coordinator for the application.
// If the coordinator doesn't already exist, it is created and the application's store added to it.
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator
{
    if (_persistentStoreCoordinator != nil) {
        return _persistentStoreCoordinator;
    }
    
    NSURL *storeURL = [[self applicationDocumentsDirectory] URLByAppendingPathComponent:@"Model.sqlite"];
    
    NSError *error = nil;
    
    
    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSNumber numberWithBool:YES], NSMigratePersistentStoresAutomaticallyOption,
                             [NSNumber numberWithBool:YES], NSInferMappingModelAutomaticallyOption, nil];
    
    
    _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
    if (![_persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeURL options:options error:&error]) {
        
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    
    return _persistentStoreCoordinator;
}

#pragma mark - Application's Documents directory

// Returns the URL to the application's Documents directory.
- (NSURL *)applicationDocumentsDirectory
{
    
    
    return [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
}

-(void)applyAttributes:(NSString *)folderPath
{
    NSArray *filesArray = [[NSFileManager defaultManager] subpathsOfDirectoryAtPath:folderPath error:nil];
    NSEnumerator *filesEnumerator = [filesArray objectEnumerator];
    NSString *fileName;
    
    while (fileName = [filesEnumerator nextObject]) {
        // NSLog(@"apply to %@", [[NSURL fileURLWithPath:[folderPath stringByAppendingPathComponent:fileName]] path]);
        if([self addSkipBackupAttributeToItemAtURL:[NSURL fileURLWithPath:[folderPath stringByAppendingPathComponent:fileName]]])
        {
            //NSLog(@"success applying");
        }
        
        //NSDictionary *fileDictionary = [[NSFileManager defaultManager] attributesOfItemAtPath:[folderPath stringByAppendingPathComponent:fileName] error:nil];
        //fileSize += [fileDictionary fileSize];
    }
    
}



- (BOOL)addSkipBackupAttributeToItemAtURL:(NSURL *)URL
{
    if([[NSFileManager defaultManager] fileExistsAtPath: [URL path]])
    {
        NSError *error = nil;
        BOOL success = [URL setResourceValue: [NSNumber numberWithBool: YES]
                                      forKey: NSURLIsExcludedFromBackupKey error: &error];
        if(!success){
            NSLog(@"Error excluding %@ from backup %@", [URL lastPathComponent], error);
        }
        return success;
    }
    
    return NO;
    
}


@end
