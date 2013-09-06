


#import "MEDSideDrawerViewController.h"
#import "MEDCenterTableViewController.h"
#import "MEDSideDrawerTableViewCell.h"
#import "MEDSideDrawerSectionHeaderView.h"
#import "MainViewController.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "User.h"
#import "OTMhelper.h"
#import "ScheduleViewController.h"
//#import "ExtractPackAppDelegate.h"
@implementation MEDSideDrawerViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    _menuItems = @[@"Medical Packages", @"Scan code", @"Schedule", @"Drugs Reference"];
    
    _tableView = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStylePlain];
    [self.tableView setDelegate:self];
    [self.tableView setDataSource:self];
    [self.view addSubview:self.tableView];
    [self.tableView setAutoresizingMask:UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight];
    [self.tableView setSeparatorColor:[UIColor colorWithRed:49.0/255.0
                                                      green:54.0/255.0
                                                       blue:57.0/255.0
                                                      alpha:1.0]];
    [self.tableView setBackgroundColor:[UIColor colorWithRed:77.0/255.0
                                                       green:79.0/255.0
                                                        blue:80.0/255.0
                                                       alpha:1.0]];
    
    [self.view setBackgroundColor:[UIColor colorWithRed:66.0/255.0
                                                  green:69.0/255.0
                                                   blue:71.0/255.0
                                                  alpha:1.0]];
    
    self.drawerWidths = @[@(160),@(200),@(240),@(280),@(320)];
    
    
    [self.view setBackgroundColor:[UIColor clearColor]];
}

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    [self.tableView reloadSections:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, self.tableView.numberOfSections-1)] withRowAnimation:UITableViewRowAnimationNone];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 4;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    switch (section) {
        case NWMenuSectionMain:
            return 4;
        case NWMenuSectionSettings:
            return 1;
        case NWMenuSectionAbout:
            return 1;
        case NWMenuSectionLogout:
            return 1;
        default:
            return 0;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = (UITableViewCell*)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        
        cell = [[MEDSideDrawerTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
        [cell setSelectionStyle:UITableViewCellSelectionStyleBlue];
    }
    
    switch (indexPath.section) {
        case NWMenuSectionMain:
            cell.textLabel.text = _menuItems[indexPath.row];
            [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
            break;
        case NWMenuSectionSettings:{
            [cell.textLabel setText:@"Settings"];
            [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];

            break;
        }
        case NWMenuSectionAbout:{
            [cell.textLabel setText:@"About"];
            [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
            
            break;
        }
        case NWMenuSectionLogout:{
            [cell.textLabel setText:@"Log Out"];
            [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
            break;
        }
        /*case MMDrawerSectionOpenDrawerGestures:{
            switch (indexPath.row) {
                case 0:
                    [cell.textLabel setText:@"Pan Nav Bar"];
                    if((self.mm_drawerController.openDrawerGestureModeMask&MMOpenDrawerGestureModePanningNavigationBar)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 1:
                    [cell.textLabel setText:@"Pan Center View"];
                    if((self.mm_drawerController.openDrawerGestureModeMask&MMOpenDrawerGestureModePanningCenterView)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 2:
                    [cell.textLabel setText:@"Bezel Pan Center View"];
                    if((self.mm_drawerController.openDrawerGestureModeMask&MMOpenDrawerGestureModeBezelPanningCenterView)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                default:
                    break;
            }
            break;
        }
        case MMDrawerSectionCloseDrawerGestures:{
            switch (indexPath.row) {
                case 0:
                    [cell.textLabel setText:@"Pan Nav Bar"];
                    if((self.mm_drawerController.closeDrawerGestureModeMask&MMCloseDrawerGestureModePanningNavigationBar)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 1:
                    [cell.textLabel setText:@"Pan Center View"];
                    if((self.mm_drawerController.closeDrawerGestureModeMask&MMCloseDrawerGestureModePanningCenterView)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 2:
                    [cell.textLabel setText:@"Bezel Pan Center View"];
                    if((self.mm_drawerController.closeDrawerGestureModeMask&MMCloseDrawerGestureModeBezelPanningCenterView)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 3:
                    [cell.textLabel setText:@"Tap Nav Bar"];
                    if((self.mm_drawerController.closeDrawerGestureModeMask&MMCloseDrawerGestureModeTapNavigationBar)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 4:
                    [cell.textLabel setText:@"Tap Center View"];
                    if((self.mm_drawerController.closeDrawerGestureModeMask&MMCloseDrawerGestureModeTapCenterView)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 5:
                    [cell.textLabel setText:@"Pan Drawer View"];
                    if((self.mm_drawerController.closeDrawerGestureModeMask&MMCloseDrawerGestureModePanningDrawerView)>0)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                default:
                    break;
            }
            break;
        }
        case MMDrawerSectionCenterHiddenInteraction:{
            [cell setSelectionStyle:UITableViewCellSelectionStyleBlue];
            switch (indexPath.row) {
                case 0:
                    [cell.textLabel setText:@"None"];
                    if(self.mm_drawerController.centerHiddenInteractionMode == MMDrawerOpenCenterInteractionModeNone)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 1:
                    [cell.textLabel setText:@"Full"];
                    if(self.mm_drawerController.centerHiddenInteractionMode == MMDrawerOpenCenterInteractionModeFull)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                case 2:
                    [cell.textLabel setText:@"Nav Bar Only"];
                    if(self.mm_drawerController.centerHiddenInteractionMode == MMDrawerOpenCenterInteractionModeNavigationBarOnly)
                        [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
                    else
                        [cell setAccessoryType:UITableViewCellAccessoryNone];
                    break;
                    
                default:
                    break;
            }
            break;
        }
        case MMDrawerSectionStretchDrawer:{
            [cell.textLabel setText:@"Stretch Drawer"];
            if(self.mm_drawerController.shouldStretchDrawer)
                [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
            else
                [cell setAccessoryType:UITableViewCellAccessoryNone];
            break;
        }*/
        default:
            break;
    }
    
    return cell;
}

-(NSString*)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
    {
    switch (section) {
        case NWMenuSectionMain:
            return @"";
        case NWMenuSectionSettings:
            return @"";
        case NWMenuSectionAbout:
            return @"";
        case NWMenuSectionLogout:
            return @"";
        default:
            return nil;
    }
}

-(UIView*)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    MEDSideDrawerSectionHeaderView * headerView =  [[MEDSideDrawerSectionHeaderView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(tableView.bounds), 20.0f)];
    [headerView setAutoresizingMask:UIViewAutoresizingFlexibleHeight|UIViewAutoresizingFlexibleWidth];
    [headerView setTitle:[tableView.dataSource tableView:tableView titleForHeaderInSection:section]];
    return headerView;
}

-(CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section
{
    if(section == NWMenuSectionLogout)
        return 1;
    return 0;
}

-(UIView *)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section
{
    if(section == NWMenuSectionLogout)
    {
        UIView *empty = [[UIView alloc] initWithFrame:CGRectZero];
        return empty;
    }
    else
    {
        UIView *empty = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 320, 0)];
        return empty;

    }

}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    return 23.0;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return 40.0;
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.section) {
        case NWMenuSectionMain:{
            if(indexPath.row == 0)
            {
                MEDCenterTableViewController * center = [[MEDCenterTableViewController alloc] initWithStyle:UITableViewStyleGrouped];
                
                UINavigationController * nav = [[UINavigationController alloc] initWithRootViewController:center];
                
                [self.mm_drawerController
                 setCenterViewController:nav
                 withCloseAnimation:YES
                 completion:nil];
            }
            if(indexPath.row == 1)
            {
               // ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;

                ////ExtractPackAppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
                //[appDelegate showCapture];
                MainViewController *controller = [[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil];
                
                
                [self.mm_drawerController setCenterViewController:controller withCloseAnimation:YES completion:nil];
                //[self.mm_drawerController presentViewController:controller animated:YES completion:nil];
                
                //[self.view addSubview:controller->activeViewController.view];
                //controller->activeViewController.view
            }
            if(indexPath.row == 2)
            {
                ScheduleViewController * center = [[ScheduleViewController alloc] initWithNibName:@"ScheduleViewController" bundle:nil];
                
                UINavigationController * nav = [[UINavigationController alloc] initWithRootViewController:center];
                
                [self.mm_drawerController
                 setCenterViewController:nav
                 withCloseAnimation:YES
                 completion:nil];
            }
            break;
        }
        case NWMenuSectionLogout:{
            MEDCenterTableViewController * center = [[MEDCenterTableViewController alloc] initWithStyle:UITableViewStyleGrouped];
            
            UINavigationController * nav = [[UINavigationController alloc] initWithRootViewController:center];
            [[NSUserDefaults standardUserDefaults] setObject:nil forKey:@"login"];
            [[NSUserDefaults standardUserDefaults] synchronize];
            
            User *user = [[OTMhelper sharedInstance] getCurrentUser];
            
            if(user != nil)
            {
                [User deleteInContext:user];
                [User saveDefaultContext];
            }
            
            [self.mm_drawerController
             setCenterViewController:nav
             withCloseAnimation:YES
             completion:nil];
            break;
        }
        default:
            break;
    }
    [tableView selectRowAtIndexPath:indexPath animated:NO scrollPosition:UITableViewScrollPositionNone];
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

@end
