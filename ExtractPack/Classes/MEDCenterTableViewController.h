


#import <UIKit/UIKit.h>

@interface MEDCenterTableViewController : UITableViewController

@property (nonatomic, strong) NSArray *packages;

-(void)checkBarcodeOnServer:(NSString *)barcodeId;
-(void)showLogin;
@end
