//
//  OTMMedicationsVC.m
//  ExtractPack
//
//  Created by Nero Wolfe on 28/10/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMMedicationsVC.h"
#import "User.h"
#import "MedPackage.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "NSDate-Utilities.h"
#import "OTMhelper.h"
#import "MEDNetworkHelper.h"
#import "MainViewController.h"
#import "MBProgressHUD.h"
#import "OTMPackageVC.h"
@interface OTMMedicationsVC ()
{
     NSArray *packages;
}
@end

@implementation OTMMedicationsVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.table.separatorInset = UIEdgeInsetsMake(0, 10, 0, 10);

    self.navigationItem.title = @"Medications";
    
    packages = [NSMutableArray new];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(checkBarcodeOnServer:) name:@"checkBarcodeOnServer" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(saveTakingMedStep:) name:@"takingMed" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(alreadyTakingMedStep:) name:@"alreadyTakingMed" object:nil];


    
    //self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(getPackages)];

    
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addPackage)];

    
     [self getPackages];
	// Do any additional setup after loading the view.
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self getPackages];
    
    [UIView animateWithDuration:0.4 animations:^{
        
        self.tabBarController.tabBar.hidden = NO;
        
        self.navigationController.navigationBarHidden = NO;
        
    }];
}

-(void)getPackages
{
    
    User *user = [[OTMhelper sharedInstance] getCurrentUser];
    
    packages = [user.medpackages allObjects];
    
    [self.table reloadData];
    
   
}

-(void)addPackage
{
    
    
    [UIView animateWithDuration:0.4 animations:^{
        
        self.tabBarController.tabBar.hidden = YES;
        
        self.navigationController.navigationBarHidden = YES;

        MainViewController *controller = [[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil];
        
        
        [self.navigationController pushViewController:controller animated:YES];
    }];
    

}





-(void)saveTakingMedStep:(NSNotification *)notification
{
    MedPackage * pack = (MedPackage *) [notification object];

    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"hh:mm:ss a"];
    NSString *time = [formatter stringFromDate:[NSDate date]];
    [formatter setDateFormat:@"MM/dd/yyyy"];
    NSString *date = [formatter stringFromDate:[NSDate date]];
    
    
    
    NSString *xml = [[OTMhelper sharedInstance] returnEventXML:EVENTTakePictureAfter];
    xml = [NSString stringWithFormat:xml, time, date, pack.packageid, pack.barcode, [[OTMhelper sharedInstance] getCurrentUser].userId];
    
    
    
    [[MEDNetworkHelper sharedInstance] postEvent:EVENTTakePictureAfter packageid:pack.packageid.integerValue xmlString:xml obj:nil completionBlock:^(BOOL result, NSError *error) {
        NSLog(@"EVENTTakePictureAfter sent");
        if(result)
        {
            if(pack.startdate == nil)
            {
                pack.status = [NSNumber numberWithInt:PS_used];
                pack.startdate = [NSDate date];
                [MedPackage saveDefaultContext];
                
                NSString *xml = [[OTMhelper sharedInstance] returnEventXML:EVENTSetMedicationScheduleStartTime];
                xml = [NSString stringWithFormat:xml, time, date, pack.packageid, pack.barcode, [[OTMhelper sharedInstance] getCurrentUser].userId];
                
                [[MEDNetworkHelper sharedInstance] postEvent:EVENTSetMedicationScheduleStartTime packageid:[pack.packageid integerValue] xmlString:xml obj:nil completionBlock:^(BOOL result, NSError *error) {
                    if(result)
                    {
                        NSLog(@"EVENTSetMedicationScheduleStartTime sent");
                        [self getPackages];

                    }
                    else
                    {
                         dispatch_async(dispatch_get_main_queue(),^{
                         UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.description delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                         [alert show];
                         
                         });
                        
                    }
                }];
            }
            else
            {
                [self getPackages];
            }
            
            
        }
        else
        {
            dispatch_async(dispatch_get_main_queue(),^{
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.localizedDescription delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                [alert show];
                
            });
        }
        
        
    }];
    
}


-(void)checkBarcodeOnServer:(NSNotification *)notification
{
    
    [MBProgressHUD showHUDAddedTo:self.view animated:YES];

    
    NSString * barcodeId = (NSString *) [notification object];
    
    [[MEDNetworkHelper sharedInstance] getMedPackageByBarcodeId:barcodeId completionBlock:^(NSDictionary *result, NSError *error) {
        NSLog(@"success getMedPackageByBarcodeId");
        
        [MBProgressHUD hideAllHUDsForView:self.view animated:YES];
        
        if(result != nil)
        {
            if([[OTMhelper sharedInstance] getPackageById:[result objectForKey:@"Id"]] == nil)
            {
                MedPackage *pack = [[OTMhelper sharedInstance] createPackage:result];
                
                int packageId = [[result objectForKey:@"Id"] integerValue];
                
                NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
                [formatter setDateFormat:@"hh:mm:ss a"];
                NSString *time = [formatter stringFromDate:[NSDate date]];
                [formatter setDateFormat:@"MM/dd/yyyy"];
                NSString *date = [formatter stringFromDate:[NSDate date]];
                
                //<GOODSCAN><time>%@</time><date>%@</date><order_number>%@</order_number><correct_code>%@</correct_code><userid>%@</userid></GOODSCAN>
                
                NSString *xml = [[OTMhelper sharedInstance] returnEventXML:EVENTScanBarcode];
                xml = [NSString stringWithFormat:xml, time, date, pack.packageid, pack.barcode, [[OTMhelper sharedInstance] getCurrentUser].userId];
                
                
                [[MEDNetworkHelper sharedInstance] postEvent:EVENTScanBarcode packageid:packageId xmlString:xml obj:nil completionBlock:^(BOOL result, NSError *error) {
                    NSLog(@"EVENTScanBarcode sent");
                    if(result)
                    {
                        
                        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
                        [formatter setDateFormat:@"hh:mm:ss a"];
                        NSString *time = [formatter stringFromDate:[NSDate date]];
                        [formatter setDateFormat:@"MM/dd/yyyy"];
                        NSString *date = [formatter stringFromDate:[NSDate date]];
                        
                        
                        NSString *xml = [[OTMhelper sharedInstance] returnEventXML:EVENTTakePictureBefore];
                        xml = [NSString stringWithFormat:xml, time, date, pack.packageid, pack.barcode, [[OTMhelper sharedInstance] getCurrentUser].userId];
                        
                        
                        
                        [[MEDNetworkHelper sharedInstance] postEvent:EVENTTakePictureBefore packageid:packageId xmlString:xml obj:nil completionBlock:^(BOOL result, NSError *error) {
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
                MedPackage *pack = [[OTMhelper sharedInstance] getPackageById:[result objectForKey:@"Id"]];
                int packageId = [[result objectForKey:@"Id"] integerValue];

                
                NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
                [formatter setDateFormat:@"hh:mm:ss a"];
                NSString *time = [formatter stringFromDate:[NSDate date]];
                [formatter setDateFormat:@"MM/dd/yyyy"];
                NSString *date = [formatter stringFromDate:[NSDate date]];
                
                NSString *xml = [[OTMhelper sharedInstance] returnEventXML:EVENTDeviceAlarmWrongMedScanned];
                xml = [NSString stringWithFormat:xml, time, date, pack.barcode, barcodeId, [[OTMhelper sharedInstance] getCurrentUser].userId];
                
                
                [[MEDNetworkHelper sharedInstance] postEvent:EVENTDeviceAlarmWrongMedScanned packageid:packageId xmlString:xml obj:nil completionBlock:^(BOOL result, NSError *error) {
                    
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


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return packages.count;
}


-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    return 70;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"MedCell"];
    
    
    MedPackage *p = packages[indexPath.row];
    

    
    UILabel *lblTitle = (UILabel *)[cell.contentView viewWithTag:101];
    lblTitle.text =  p.medicationname;
    
    
    
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
        if(index < dates.count)
        {
            NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
            //[dateFormat setTimeZone:[NSTimeZone timeZoneWithName:@"GMT"]];
            [dateFormat setDateFormat:@"MM-dd-yyyy hh:mma"];
            NSString *dateStr = [dateFormat stringFromDate:dates[index]];
            NSString *str = [NSString stringWithFormat:@"%i doses left. \nNext dose time is %@", dates.count - index - 1, dateStr];
            
            
            UILabel *lblSub = (UILabel *)[cell.contentView viewWithTag:102];
            
            lblSub.text = str;
            
        }
        else
        {
            NSString *str = @"Medications period ended";
            
            UILabel *lblSub = (UILabel *)[cell.contentView viewWithTag:102];
            
            lblSub.text = str;
        }
        
    }
    else
    {

        NSString *str = @"Did not started yet.";
        
        UILabel *lblSub = (UILabel *)[cell.contentView viewWithTag:102];
        
        lblSub.text = str;
    }
    
    return cell;
}



#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    MedPackage *p = [packages objectAtIndex:indexPath.row];
    
    
    [self performSegueWithIdentifier:@"ShowPackage" sender:p];
    
    /*MEDAddEditPrescriptionViewController *controller = [[MEDAddEditPrescriptionViewController alloc] init];
    controller.isNew = NO;
    controller.package = p;
    [controller loadModel];
    self.navigationItem.backBarButtonItem = nil;
    [self.navigationController pushViewController:controller animated:YES];*/
}


-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if([segue.identifier isEqualToString:@"ShowPackage"])
    {
        MedPackage *pack = (MedPackage *)sender;
        OTMPackageVC *controller = (OTMPackageVC *)segue.destinationViewController;
        controller.package = pack;
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
