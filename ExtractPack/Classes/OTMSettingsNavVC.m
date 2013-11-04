//
//  OTMSettingsNavVC.m
//  ExtractPack
//
//  Created by Nero Wolfe on 04/11/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMSettingsNavVC.h"

@interface OTMSettingsNavVC ()

@end

@implementation OTMSettingsNavVC

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
    
    
    
    UIImage *settings = [UIImage imageNamed:@"912-gears"];
    UIImage *settingsSel = [UIImage imageNamed:@"912-gears-selected"];
    
    settings = [settings imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    settingsSel = [settingsSel imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    self.tabBarItem = [[UITabBarItem alloc] initWithTitle:@"Settings" image:settings selectedImage:settingsSel];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
