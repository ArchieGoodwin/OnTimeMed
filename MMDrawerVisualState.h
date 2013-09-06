


#import <Foundation/Foundation.h>
#import "MMDrawerController.h"

/**
 `MMDrawerVisualState` provides prebuilt visual state update blocks that can be used in the callback block of `MMDrawerController` for drawer animation.
 */

@interface MMDrawerVisualState : NSObject

/**
 Creates a slide and scale visual state block that gives an experience similar to Mailbox.app. It scales from 90% to 100%, and translates 50 pixels in the x direction. In addition, it also sets alpha from 0.0 to 1.0.
 
 @return The visual state block.
 */
+(MMDrawerControllerDrawerVisualStateBlock)slideAndScaleVisualStateBlock;

/**
 Creates a slide visual state block that gives the user an experience that slides at the same speed of the center view controller during animation. This is equal to calling `parallaxVisualStateBlockWithParallaxFactor:` with a parallax factor of 1.0.
 
 @return The visual state block.
 */
+(MMDrawerControllerDrawerVisualStateBlock)slideVisualStateBlock;

/**
 Creates a swinging door visual state block that gives the user an experience that animates the drawer in along the hinge. 
 
 @return The visual state block.
 */
+(MMDrawerControllerDrawerVisualStateBlock)swingingDoorVisualStateBlock;

/**
 Creates a parallax experience that slides the side drawer view controller at a different rate than the center view controller during animation. For every parallaxFactor of points moved by the center view controller, the side drawer view controller will move 1 point. Passing in 1.0 is the  equivalent of a applying a sliding animation, while passing in MAX_FLOAT is the equivalent of having no animation at all.
 
 @param parallaxFactor The amount of parallax applied to the side drawer conroller. This value must be greater than 1.0. The closer the value is to 1.0, the faster the side drawer view controller will be parallaxing.
 
 @return The visual state block.
 */
+(MMDrawerControllerDrawerVisualStateBlock)parallaxVisualStateBlockWithParallaxFactor:(CGFloat)parallaxFactor;

@end
