


#import <UIKit/UIKit.h>
#import "UIViewController+MMDrawerController.h"

typedef NS_ENUM(NSInteger, MMDrawerSection){
    MMDrawerSectionViewSelection,
    MMDrawerSectionDrawerWidth,
    MMDrawerSectionShadowToggle,
    MMDrawerSectionOpenDrawerGestures,
    MMDrawerSectionCloseDrawerGestures,
    MMDrawerSectionCenterHiddenInteraction,
    MMDrawerSectionStretchDrawer,
};


typedef NS_ENUM(NSInteger, NWMenuSection){
    NWMenuSectionMain = 0,
    NWMenuSectionSettings = 1,
    NWMenuSectionAbout = 2,
    NWMenuSectionLogout = 3

};

@interface MEDSideDrawerViewController : UIViewController <UITableViewDataSource,UITableViewDelegate>
@property (nonatomic, strong) UITableView * tableView;
@property (nonatomic,strong) NSArray * drawerWidths;
@property (nonatomic, strong) NSArray *menuItems;

@end
