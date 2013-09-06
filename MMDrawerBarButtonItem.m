


#import "MMDrawerBarButtonItem.h"

@interface MMDrawerMenuButtonView : UIButton
@property (nonatomic,strong) UIColor * menuButtonNormalColor;
@property (nonatomic,strong) UIColor * menuButtonHighlightedColor;

@property (nonatomic,strong) UIColor * shadowNormalColor;
@property (nonatomic,strong) UIColor * shadowHighlightedColor;

-(UIColor *)menuButtonColorForState:(UIControlState)state;
-(void)setMenuButtonColor:(UIColor *)color forState:(UIControlState)state;

-(UIColor *)shadowColorForState:(UIControlState)state;
-(void)setShadowColor:(UIColor *)color forState:(UIControlState)state;

@end

@implementation MMDrawerMenuButtonView

-(id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if(self){
        [self setMenuButtonNormalColor:[[UIColor whiteColor] colorWithAlphaComponent:0.9f]];
        [self setMenuButtonHighlightedColor:[UIColor colorWithRed:139.0/255.0
                                                            green:135.0/255.0
                                                             blue:136.0/255.0
                                                            alpha:0.9f]];
        
        [self setShadowNormalColor:[[UIColor blackColor] colorWithAlphaComponent:0.5f]];
        [self setShadowHighlightedColor:[[UIColor blackColor] colorWithAlphaComponent:0.2f]];
    }
    return self;
}

-(UIColor *)menuButtonColorForState:(UIControlState)state{
    UIColor * color;
    switch (state) {
        case UIControlStateNormal:
            color = self.menuButtonNormalColor;
            break;
        case UIControlStateHighlighted:
            color = self.menuButtonHighlightedColor;
            break;
        default:
            break;
    }
    return color;
}

-(void)setMenuButtonColor:(UIColor *)color forState:(UIControlState)state{
    switch (state) {
        case UIControlStateNormal:
            [self setMenuButtonNormalColor:color];
            break;
        case UIControlStateHighlighted:
            [self setMenuButtonHighlightedColor:color];
            break;
        default:
            break;
    }
    [self setNeedsDisplay];
}

-(UIColor *)shadowColorForState:(UIControlState)state{
    UIColor * color;
    switch (state) {
        case UIControlStateNormal:
            color = self.shadowNormalColor;
            break;
        case UIControlStateHighlighted:
            color = self.shadowHighlightedColor;
            break;
        default:
            break;
    }
    return color;
}

-(void)setShadowColor:(UIColor *)color forState:(UIControlState)state{
    switch (state) {
        case UIControlStateNormal:
            [self setShadowNormalColor:color];
            break;
        case UIControlStateHighlighted:
            [self setShadowHighlightedColor:color];
            break;
        default:
            break;
    }
    [self setNeedsDisplay];
}

-(void)drawRect:(CGRect)rect{
    //// General Declarations
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    //Sizes
    CGFloat buttonWidth = CGRectGetWidth(self.bounds)*.80;
    CGFloat buttonHeight = CGRectGetHeight(self.bounds)*.16;
    CGFloat xOffset = CGRectGetWidth(self.bounds)*.10;
    CGFloat yOffset = CGRectGetHeight(self.bounds)*.12;
    CGFloat cornerRadius = 1.0;
    
    //// Color Declarations
    UIColor*  buttonColor = [self menuButtonColorForState:self.state];
    UIColor*  shadowColor = [self shadowColorForState:self.state];

    
    //// Shadow Declarations
    UIColor* shadow =  shadowColor;
    CGSize shadowOffset = CGSizeMake(0.0, 1.0);
    CGFloat shadowBlurRadius = 0;
    
    //// Top Bun Drawing
    UIBezierPath* topBunPath = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(xOffset, yOffset, buttonWidth, buttonHeight) cornerRadius:cornerRadius];
    CGContextSaveGState(context);
    CGContextSetShadowWithColor(context, shadowOffset, shadowBlurRadius, shadow.CGColor);
    [buttonColor setFill];
    [topBunPath fill];
    CGContextRestoreGState(context);
    
    //// Meat Drawing
    UIBezierPath* meatPath = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(xOffset, yOffset*2 + buttonHeight, buttonWidth, buttonHeight) cornerRadius:cornerRadius];
    CGContextSaveGState(context);
    CGContextSetShadowWithColor(context, shadowOffset, shadowBlurRadius, shadow.CGColor);
    [buttonColor setFill];
    [meatPath fill];
    CGContextRestoreGState(context);
    
    //// Bottom Bun Drawing
    UIBezierPath* bottomBunPath = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(xOffset, yOffset*3 + buttonHeight*2, buttonWidth, buttonHeight) cornerRadius:cornerRadius];
    CGContextSaveGState(context);
    CGContextSetShadowWithColor(context, shadowOffset, shadowBlurRadius, shadow.CGColor);
    [buttonColor setFill];
    [bottomBunPath fill];
    CGContextRestoreGState(context);
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesBegan:touches withEvent:event];
    [self setNeedsDisplay];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesEnded:touches withEvent:event];
    [self setNeedsDisplay];
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
    [super touchesCancelled:touches withEvent:event];
    [self setNeedsDisplay];
}

- (void)setSelected:(BOOL)selected{
    [super setSelected:selected];
    [self setNeedsDisplay];
}

- (void)setHighlighted:(BOOL)highlighted{
    [super setHighlighted:highlighted];
    [self setNeedsDisplay];
}
@end

@interface MMDrawerBarButtonItem ()
@property (nonatomic,strong) MMDrawerMenuButtonView * buttonView;

@end

@implementation MMDrawerBarButtonItem

-(id)initWithTarget:(id)target action:(SEL)action{
    MMDrawerMenuButtonView * buttonView = [[MMDrawerMenuButtonView alloc] initWithFrame:CGRectMake(0, 0, 26, 26)];
    [buttonView addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
    self = [self initWithCustomView:buttonView];
    if(self){
        [self setButtonView:buttonView];
    }
    return self;
}

-(UIColor *)menuButtonColorForState:(UIControlState)state{
    return [self.buttonView menuButtonColorForState:state];
}

-(void)setMenuButtonColor:(UIColor *)color forState:(UIControlState)state{
    [self.buttonView setMenuButtonColor:color forState:state];
}

-(UIColor *)shadowColorForState:(UIControlState)state{
    return [self.buttonView shadowColorForState:state];
}

-(void)setShadowColor:(UIColor *)color forState:(UIControlState)state{
    [self.buttonView setShadowColor:color forState:state];
}

@end
