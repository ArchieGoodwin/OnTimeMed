


#import "MEDSideDrawerTableViewCell.h"

@implementation MEDSideDrawerTableViewCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        [self setAccessoryCheckmarkColor:[UIColor whiteColor]];

        UIView * backgroundView = [[UIView alloc] initWithFrame:self.bounds];
        [backgroundView setAutoresizingMask:UIViewAutoresizingFlexibleHeight|UIViewAutoresizingFlexibleWidth];
        [backgroundView setBackgroundColor:[UIColor colorWithRed:77.0/255.0
                                                           green:79.0/255.0
                                                            blue:80.0/255.0
                                                           alpha:1.0]];
        [self setBackgroundView:backgroundView];
        
        [self.textLabel setBackgroundColor:[UIColor clearColor]];
        [self.textLabel setTextColor:[UIColor
                                      colorWithRed:230.0/255.0
                                      green:236.0/255.0
                                      blue:242.0/255.0
                                      alpha:1.0]];
        [self.textLabel setShadowColor:[[UIColor blackColor] colorWithAlphaComponent:.5]];
        [self.textLabel setShadowOffset:CGSizeMake(0, 1)];
        [self.textLabel setFont:[UIFont boldSystemFontOfSize:16.0]];
    }
    return self;
}


@end
