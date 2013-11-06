//
//  OTMTabbarVC.m
//  ExtractPack
//
//  Created by Nero Wolfe on 28/10/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMTabbarVC.h"

@implementation OTMTabbarVC


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

-(void)viewDidLoad
{
    
    [super viewDidLoad];
    
    self.navigationController.interactivePopGestureRecognizer.enabled = NO;

    self.tabBar.tintAdjustmentMode = UIViewTintAdjustmentModeNormal;

    self.tabBar.tintColor = [UIColor whiteColor];
    


    [self addAllTabbarImages];
    
    
}


-(void)addAllTabbarImages
{
    
    UITabBarItem *item1 = [self.tabBar.items objectAtIndex:0];
    
    item1.image = [[UIImage imageNamed:@"960-pill"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    item1.selectedImage = [[UIImage imageNamed:@"960-pill-selected"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    
    UITabBarItem *item2 = [self.tabBar.items objectAtIndex:1];
    
    item2.image = [[UIImage imageNamed:@"977-checkbox"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    item2.selectedImage = [[UIImage imageNamed:@"977-checkbox-selected"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    UITabBarItem *item3 = [self.tabBar.items objectAtIndex:2];
    
    item3.image = [[UIImage imageNamed:@"1018-box"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    item3.selectedImage = [[UIImage imageNamed:@"1018-box-selected"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    
    UITabBarItem *item4 = [self.tabBar.items objectAtIndex:3];
    
    item4.image = [[UIImage imageNamed:@"912-gears"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    item4.selectedImage = [[UIImage imageNamed:@"912-gears-selected"] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
}


-(void)tabBar:(UITabBar *)tabBar didSelectItem:(UITabBarItem *)item
{
    
   
    [self addAllTabbarImages];
    
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
