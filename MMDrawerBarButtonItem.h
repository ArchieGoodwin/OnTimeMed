

#import <UIKit/UIKit.h>

/**
  `MMDrawerBarButtonItem` provides convenience methods to create `UIBarButtonItems` with a default hamburger-menu asset.
 */

@interface MMDrawerBarButtonItem : UIBarButtonItem

///---------------------------------------
/// @name Initializing a `MMDrawerBarButtonItem`
///---------------------------------------

/**
 Creates and initializes an `MMDrawerBarButtonItem` without a border.
 
 @param target The target to forward the `action` to when the button is pressed.
 @param action The action to call when the button is pressed.
 
 @return The newly-initialized bar button item.
 */
-(id)initWithTarget:(id)target action:(SEL)action;

/**
 Returns the current color of the menu button for the state requested.
 
 @param state The UIControl state that the color is being requested for.
 
 @return The menu button color for the requested state.
 */
-(UIColor *)menuButtonColorForState:(UIControlState)state;

/**
 Sets the color of the menu button for the specified state. For this control, only set colors for `UIControlStateNormal` and `UIControlStateHighlighted`.
 
 @param color The color to set.
 @param state The state to set the color for.
 */
-(void)setMenuButtonColor:(UIColor *)color forState:(UIControlState)state;

/**
 Returns the current color of the shadow for the state requested.
 
 @param state The UIControl state that the color is being requested for.
 
 @return The menu button color for the requested state.
 */
-(UIColor *)shadowColorForState:(UIControlState)state;

/**
 Sets the color of the shadow for the specified state. For this control, only set colors for `UIControlStateNormal` and `UIControlStateHighlighted`.
 
 @param color The color to set.
 @param state The state to set the color for.
 */
-(void)setShadowColor:(UIColor *)color forState:(UIControlState)state;

@end
