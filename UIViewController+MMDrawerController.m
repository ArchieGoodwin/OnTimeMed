


#import "UIViewController+MMDrawerController.h"

@implementation UIViewController (MMDrawerController)


-(MMDrawerController*)mm_drawerController{
    if([self.parentViewController isKindOfClass:[MMDrawerController class]]){
        return (MMDrawerController*)self.parentViewController;
    }
    else if([self.parentViewController isKindOfClass:[UINavigationController class]] &&
            [self.parentViewController.parentViewController isKindOfClass:[MMDrawerController class]]){
        return (MMDrawerController*)[self.parentViewController parentViewController];
    }
    else{
        return nil;
    }
}

-(CGRect)mm_visibleDrawerFrame{
    if([self isEqual:self.mm_drawerController.leftDrawerViewController] ||
       [self.navigationController isEqual:self.mm_drawerController.leftDrawerViewController]){
        CGRect rect = self.mm_drawerController.view.bounds;
        rect.size.width = self.mm_drawerController.maximumLeftDrawerWidth;
        return rect;
        
    }
    else if([self isEqual:self.mm_drawerController.rightDrawerViewController] ||
             [self.navigationController isEqual:self.mm_drawerController.rightDrawerViewController]){
        CGRect rect = self.mm_drawerController.view.bounds;
        rect.size.width = self.mm_drawerController.maximumRightDrawerWidth;
        rect.origin.x = CGRectGetWidth(self.mm_drawerController.view.bounds)-rect.size.width;
        return rect;
    }
    else {
        return CGRectNull;
    }
}

@end
