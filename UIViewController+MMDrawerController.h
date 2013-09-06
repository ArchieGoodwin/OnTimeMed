

#import <UIKit/UIKit.h>

#import "MMDrawerController.h"

/**
 A helper category on `UIViewController` that exposes the parent drawer controller, the visible side drawer frame, and a `mm_drawerWillAppear` method that is called when the drawer is about to appear.
 */

@interface UIViewController (MMDrawerController)

///---------------------------------------
/// @name Accessing Drawer View Controller Properties
///---------------------------------------

/**
 The `MMDrawerController` that the view controller is contained within. If the view controller is not contained within a `MMDrawerController`, this property is nil. Note that if the view controller is contained within a `UINavigationController`, that navigation controller is contained within a `MMDrawerController`, this property will return a refernce to the `MMDrawerController`, despite the fact that it is not the direct parent of the view controller.
 */
@property(nonatomic, strong, readonly) MMDrawerController *mm_drawerController;

/**
 The visible rect of the side drawer view controller in the drawer controller coordinate space. If the view controller is not a drawer in a `MMDrawerController`, then this property returns `CGRectNull`
 */
@property(nonatomic, assign, readonly) CGRect mm_visibleDrawerFrame;

@end
