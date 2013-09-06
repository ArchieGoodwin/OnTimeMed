

#import "MEDCenterTableViewController.h"
#import "MEDDrawerVisualStateManager.h"
#import "UIViewController+MMDrawerController.h"
#import "MMDrawerBarButtonItem.h"
#import "MEDCenterTableViewCell.h"
#import "MEDLeftSideDrawerViewController.h"
#import "MEDRightSideDrawerViewController.h"
#import "BPBarButtonItem.h"
#import <QuartzCore/QuartzCore.h>
#import "MEDAddEditPrescriptionViewController.h"
#import "MEDNetworkHelper.h"
#import "OTMLoginViewController.h"
#import "OTMhelper.h"
#import "User.h"
#import "MedPackage.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "NSDate-Utilities.h"
typedef NS_ENUM(NSInteger, MMCenterViewControllerSection){
    MMCenterViewControllerSectionLeftViewState,
    MMCenterViewControllerSectionLeftDrawerAnimation,
    MMCenterViewControllerSectionRightViewState,
    MMCenterViewControllerSectionRightDrawerAnimation,
};

@interface MEDCenterTableViewController ()
{
    
}
@end

@implementation MEDCenterTableViewController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getPackages) name:@"logindone" object:nil];

    
    _packages = [NSMutableArray new];
    
    [self.mm_drawerController setRightDrawerViewController:nil];
    [self.navigationItem setRightBarButtonItem:nil animated:YES];
    
    
    self.navigationItem.rightBarButtonItem = [BPBarButtonItem barButtonItemWithStyle:BPBarButtonItemStyleStandardLight title:@"Refresh" target:self action:@selector(getPackages)];
    
    [[MEDDrawerVisualStateManager sharedManager] setLeftDrawerAnimationType:3];
    
    UITapGestureRecognizer * doubleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTap:)];
    [doubleTap setNumberOfTapsRequired:2];
    [self.view addGestureRecognizer:doubleTap];
    
    UITapGestureRecognizer * twoFingerDoubleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(twoFingerDoubleTap:)];
    [twoFingerDoubleTap setNumberOfTapsRequired:2];
    [twoFingerDoubleTap setNumberOfTouchesRequired:2];
    [self.view addGestureRecognizer:twoFingerDoubleTap];
    

    [self setupLeftMenuButton];
    //[self setupRightMenuButton];
    
    [self.navigationController.navigationBar setTintColor:[UIColor
                                                           colorWithRed:130.0/255.0
                                                           green:156.0/255.0
                                                           blue:206.0/255.0
                                                           alpha:1.0]];
    

    
    [self.navigationController.view.layer setCornerRadius:10.0f];
    
    
    UIView *backView = [[UIView alloc] init];
    [backView setBackgroundColor:[UIColor colorWithRed:208.0/255.0
                                                 green:218.0/255.0
                                                  blue:228.0/255.0
                                                 alpha:1.0]];
    [self.tableView setBackgroundView:backView];
    
    self.navigationItem.title = @"Prescriptions";
    
    [self getPackages];
    
    [self showLogin];
}


-(void)getPackages
{
    
    User *user = [[OTMhelper sharedInstance] getCurrentUser];

    _packages = [user.medpackages allObjects];
    
    [self.tableView reloadData];

    /*if(user != nil)
    {
        [[MEDNetworkHelper sharedInstance] getPackagesByUserId:user.userId completionBlock:^(NSArray *result, NSError *error) {
            
            for(NSDictionary *dict in result)
            {
                
            }
            
            
        }];
    }*/
}

-(void)showLogin
{
    if([[NSUserDefaults standardUserDefaults] objectForKey:@"login"] == nil)
    {
        OTMLoginViewController *controller = [[OTMLoginViewController alloc] initWithNibName:@"OTMLoginViewController" bundle:nil];
        [self.mm_drawerController presentViewController:controller animated:YES completion:^{
            
        }];
    }
}

-(void)checkBarcodeOnServer:(NSString *)barcodeId
{
    [[MEDNetworkHelper sharedInstance] getMedPackageByBarcodeId:barcodeId completionBlock:^(NSDictionary *result, NSError *error) {
        NSLog(@"success getMedPackageByBarcodeId");
        
        if(result != nil)
        {
            if([[OTMhelper sharedInstance] getPackageById:[result objectForKey:@"Id"]] == nil)
            {
                [[OTMhelper sharedInstance] createPackage:result];
                
                int packageId = [[result objectForKey:@"Id"] integerValue];
                [[MEDNetworkHelper sharedInstance] postEvent:EVENTScanBarcode packageid:packageId completionBlock:^(BOOL result, NSError *error) {
                    NSLog(@"EVENTScanBarcode sent");
                    if(result)
                    {
                        [[MEDNetworkHelper sharedInstance] postEvent:EVENTTakePictureBefore packageid:packageId completionBlock:^(BOOL result, NSError *error) {
                            NSLog(@"EVENTTakePictureBefore sent");
                        }];
                        
                    }
                    else
                    {
                        dispatch_async(dispatch_get_main_queue(),^{
                            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.description delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                            [alert show];
                            
                        });
                    }
                   
                    
                }];
                

                [self getPackages];
            }
            else
            {
                int packageId = [[result objectForKey:@"Id"] integerValue];
                [[MEDNetworkHelper sharedInstance] postEvent:EVENTDeviceAlarmWrongMedScanned packageid:packageId completionBlock:^(BOOL result, NSError *error) {
                    
                    if(result)
                    {
                        NSLog(@"EVENTTakePictureBefore sent");

                        
                    }
                    else
                    {
                        dispatch_async(dispatch_get_main_queue(),^{
                            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.description delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                            [alert show];
                            
                        });
                    }
                }];
                
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Warning" message:@"Package with this Id is already in list" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
                [alert  show];
            }
        }
        else
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:error.localizedDescription delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
            [alert  show];
        }
       
        

        
        
    }];
}

-(void)addPrescription
{
    MEDAddEditPrescriptionViewController *controller = [[MEDAddEditPrescriptionViewController alloc] init];
    controller.isNew = YES;
    [controller loadModel];

    self.navigationItem.backBarButtonItem = nil;
    [self.navigationController pushViewController:controller animated:YES];
}

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    NSLog(@"Center will appear");

    [self.tableView reloadData];
}

-(void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    NSLog(@"Center did appear");
    
    
}

-(void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    NSLog(@"Center will disappear");
}

-(void)viewDidDisappear:(BOOL)animated{
    [super viewDidDisappear:animated];
    NSLog(@"Center did disappear");
}

-(void)setupLeftMenuButton{
    MMDrawerBarButtonItem * leftDrawerButton = [[MMDrawerBarButtonItem alloc] initWithTarget:self action:@selector(leftDrawerButtonPress:)];
    [self.navigationItem setLeftBarButtonItem:leftDrawerButton animated:YES];
}

-(void)setupRightMenuButton{
    MMDrawerBarButtonItem * rightDrawerButton = [[MMDrawerBarButtonItem alloc] initWithTarget:self action:@selector(rightDrawerButtonPress:)];
    [self.navigationItem setRightBarButtonItem:rightDrawerButton animated:YES];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return _packages.count;
}


-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{


    return 70;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        
        cell = [[MEDCenterTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
        [cell setSelectionStyle:UITableViewCellSelectionStyleGray];
    }
    
    UIColor * selectedColor = [UIColor
                               colorWithRed:1.0/255.0
                               green:15.0/255.0
                               blue:25.0/255.0
                               alpha:1.0];
    UIColor * unselectedColor = [UIColor
                                 colorWithRed:79.0/255.0
                                 green:93.0/255.0
                                 blue:102.0/255.0
                                 alpha:1.0];
    
    
    MedPackage *p = _packages[indexPath.row];
    
    
    
    UILabel *lblTitle = [[UILabel alloc] initWithFrame:CGRectMake(10, 5, 300, 25)];
    lblTitle.backgroundColor = [UIColor clearColor];
    lblTitle.numberOfLines = 0;
    lblTitle.lineBreakMode = NSLineBreakByWordWrapping;
    lblTitle.font = [UIFont boldSystemFontOfSize:16];
    lblTitle.textColor = [UIColor blackColor];
    
    lblTitle.text =  p.medicationname;
    
    [cell.contentView addSubview:lblTitle];
    
    
    if(p.startdate != nil)
    {
        NSMutableArray *dates = [[OTMhelper sharedInstance] getMedicationdates:p date:p.startdate];
        int index = 0;
        for (NSDate *date in dates) {
            if([date isInPast])
            {
                index++;
            }
        }
        
        NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
        [dateFormat setTimeZone:[NSTimeZone timeZoneWithName:@"GMT"]];
        [dateFormat setDateFormat:@"MM-dd-yyyy hh:mma"];
        NSString *dateStr = [dateFormat stringFromDate:dates[index + 1]];
        NSString *str = [NSString stringWithFormat:@"%i doses left. \nNext dose time is %@", dates.count - index - 1, dateStr];
        
        UILabel *lblSub = [[UILabel alloc] initWithFrame:CGRectMake(10, 22, 300, 50)];
        lblSub.backgroundColor = [UIColor clearColor];
        lblSub.numberOfLines = 0;
        lblSub.lineBreakMode = NSLineBreakByWordWrapping;
        lblSub.font = [UIFont systemFontOfSize:13];
        lblSub.textColor = [UIColor grayColor];
        
        lblSub.text = str;
        
        [cell.contentView addSubview:lblSub];
    }
    else
    {
        UILabel *lblSub = [[UILabel alloc] initWithFrame:CGRectMake(10, 22, 300, 20)];
        lblSub.backgroundColor = [UIColor clearColor];
        lblSub.numberOfLines = 0;
        lblSub.lineBreakMode = NSLineBreakByWordWrapping;
        lblSub.font = [UIFont systemFontOfSize:13];
        lblSub.textColor = [UIColor grayColor];
        NSString *str = @"Did not started yet.";

        lblSub.text = str;
        
        [cell.contentView addSubview:lblSub];
    }
    
    return cell;
}

-(NSString*)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section{
    return @"";
}

-(UIView*)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    UIView * containerView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(self.tableView.bounds), 30)];
    
    UILabel * titleLabel = [[UILabel alloc] initWithFrame:CGRectInset(containerView.bounds, 14, 0)];
    
    [titleLabel setBackgroundColor:[UIColor clearColor]];
    [titleLabel setText:[tableView.dataSource tableView:tableView titleForHeaderInSection:section]];
    [titleLabel setFont:[UIFont boldSystemFontOfSize:18.0]];
    [titleLabel setTextColor:[UIColor colorWithRed:3.0/255.0
                                             green:48.0/255.0
                                              blue:77.0/255.0
                                             alpha:1.0]];
    [titleLabel setShadowColor:[[UIColor whiteColor] colorWithAlphaComponent:.5]];
    [titleLabel setShadowOffset:CGSizeMake(0, 1)];
    
    [containerView addSubview:titleLabel];
    
    return containerView;
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    return 30;
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    MedPackage *p = [_packages objectAtIndex:indexPath.row];
    
    MEDAddEditPrescriptionViewController *controller = [[MEDAddEditPrescriptionViewController alloc] init];
    controller.isNew = NO;
    controller.package = p;
    [controller loadModel];
    self.navigationItem.backBarButtonItem = nil;
    [self.navigationController pushViewController:controller animated:YES];
}

#pragma mark - Button Handlers
-(void)leftDrawerButtonPress:(id)sender{
    [self.mm_drawerController toggleDrawerSide:MMDrawerSideLeft animated:YES completion:nil];
}

-(void)rightDrawerButtonPress:(id)sender{
    [self.mm_drawerController toggleDrawerSide:MMDrawerSideRight animated:YES completion:nil];
}

-(void)doubleTap:(UITapGestureRecognizer*)gesture{
    [self.mm_drawerController bouncePreviewForDrawerSide:MMDrawerSideLeft completion:nil];
}

-(void)twoFingerDoubleTap:(UITapGestureRecognizer*)gesture{
    [self.mm_drawerController bouncePreviewForDrawerSide:MMDrawerSideRight completion:nil];
}

// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}




// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
		NSLog(@"delete");
    }
    
}


@end
