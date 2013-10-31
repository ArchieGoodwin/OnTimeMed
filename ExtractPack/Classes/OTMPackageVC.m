//
//  OTMPackageVC.m
//  ExtractPack
//
//  Created by Nero Wolfe on 28/10/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMPackageVC.h"
#import "MedPackage.h"
#import "MBProgressHUD.h"
#import "OTMhelper.h"
#import "MainViewController.h"
#import "NSManagedObject+NWCoreDataHelper.h"
@interface OTMPackageVC ()
{
    UIDatePicker *picker;
    UIDatePicker *pickerSkip;

    BOOL pickerShown;
    BOOL pickerShownSkip;
}
@end

@implementation OTMPackageVC

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
    
    pickerShown = NO;
    pickerShownSkip = NO;
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)btnSkipCancel:(id)sender {
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:6]];
    
    pickerShownSkip = NO;
    [self showHidePickerSkipControls:cell];

    [self.tableView beginUpdates];
    [self.tableView endUpdates];
    
}
- (IBAction)btnDoneSkip:(id)sender {
    
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:6]];
    
    pickerShownSkip = NO;
    [self showHidePickerSkipControls:cell];

    [self.tableView beginUpdates];
    [self.tableView endUpdates];
    
    
    [self skipTakingMedStep];
}

- (IBAction)btnDoneAlreadyTake:(id)sender {
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:5]];

    pickerShown = NO;
    [self showHidePickerControls:cell];

    [self.tableView beginUpdates];
    [self.tableView endUpdates];
    
    
    [self alreadyTakingMedStep];
    
}


-(void)alreadyTakingMedStep
{
    
    [[MEDNetworkHelper sharedInstance] postEvent:EVENTTakeMedication packageid:_package.packageid.integerValue completionBlock:^(BOOL result, NSError *error) {
        NSLog(@"EVENTTakeMedication sent");
        if(result)
        {
            if(_package.startdate == nil)
            {
                _package.status = [NSNumber numberWithInt:PS_used];
                _package.startdate = [NSDate date];
                [MedPackage saveDefaultContext];
                
                [[MEDNetworkHelper sharedInstance] postEvent:EVENTSetMedicationScheduleStartTime packageid:[_package.packageid integerValue] completionBlock:^(BOOL result, NSError *error) {
                    if(result)
                    {
                        NSLog(@"EVENTSetMedicationScheduleStartTime sent");
                        [self.navigationController popToRootViewControllerAnimated:YES];
                        
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
                [self.navigationController popToRootViewControllerAnimated:YES];
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

-(void)skipTakingMedStep
{
    
    [[MEDNetworkHelper sharedInstance] postEvent:EVENTSkipMedication packageid:_package.packageid.integerValue completionBlock:^(BOOL result, NSError *error) {
        NSLog(@"EVENTTakeMedication sent");
        if(result)
        {

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




- (IBAction)btnCancelAlreadyTake:(id)sender {
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:5]];
    
    pickerShown = NO;
    [self showHidePickerControls:cell];

    [self.tableView beginUpdates];
    [self.tableView endUpdates];
}

-(void)showPickerFromTextField:(UITableViewCell *)cell
{
    
    picker = [[UIDatePicker alloc] init];
    
    
    picker.datePickerMode = UIDatePickerModeDateAndTime;
	picker.date = [NSDate date];
    
    [picker setFrame: CGRectMake(0, 35, cell.contentView.frame.size.width, 180)];
    

    
    picker.tag = 1001;
        
    
    [cell.contentView addSubview: picker];
}

-(void)showPickerSkip:(UITableViewCell *)cell
{
    
    pickerSkip = [[UIDatePicker alloc] init];
    
    
    pickerSkip.datePickerMode = UIDatePickerModeDateAndTime;
	pickerSkip.date = [NSDate date];
    
    [pickerSkip setFrame: CGRectMake(0, 35, cell.contentView.frame.size.width, 180)];
    
    
    
    pickerSkip.tag = 1002;
    
    
    [cell.contentView addSubview: pickerSkip];
}

-(IBAction)changeDateInLabel:(id)sender
{
    
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 7;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return 1;
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 3;
}


-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.section) {
        case 0:
            return 50;
        case 1:
            return 60;
        case 2:
            return 100;
        case 3:
            return 44;

        case 4:
            return 44;

        case 5:
        {
            if(pickerShown)
            {
                return 224;
            }
            return 44;
        }
        case 6:
        {
            if(pickerShownSkip)
            {
                return 224;
            }
            return 44;
        }
        default:
            break;
    }
    
    return 44;
}

-(void)showHidePickerControls:(UITableViewCell *)cell
{
    UIButton *btnCancel = (UIButton *)[cell.contentView viewWithTag:202];
    UIButton *btnDone = (UIButton *)[cell.contentView viewWithTag:203];
    UILabel *lblLabel = (UILabel *)[cell.contentView viewWithTag:201];
    btnCancel.hidden = !pickerShown;
    btnDone.hidden = !pickerShown;
    lblLabel.hidden = pickerShown;
    if(!pickerShown)
    {
        UIView *v = [cell.contentView viewWithTag:1001];
        if(v)
        {
            [v removeFromSuperview];
        }
    }
}

-(void)showHidePickerSkipControls:(UITableViewCell *)cell
{
    UIButton *btnCancel = (UIButton *)[cell.contentView viewWithTag:202];
    UIButton *btnDone = (UIButton *)[cell.contentView viewWithTag:203];
    UILabel *lblLabel = (UILabel *)[cell.contentView viewWithTag:201];
    btnCancel.hidden = !pickerShownSkip;
    btnDone.hidden = !pickerShownSkip;
    lblLabel.hidden = pickerShownSkip;
    if(!pickerShownSkip)
    {
        UIView *v = [cell.contentView viewWithTag:1002];
        if(v)
        {
            [v removeFromSuperview];
        }
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"PackageNameCell"];
    
    if(indexPath.section == 0)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"PackageNameCell"];
        UILabel *lblTitle = (UILabel *)[cell.contentView viewWithTag:101];
        lblTitle.text = _package.medicationname;
        
        UILabel *lblDose = (UILabel *)[cell.contentView viewWithTag:102];
        lblDose.text =  [NSString stringWithFormat:@"%@ Pill(s)", _package.doseperusage];
        return cell;

    }
    if(indexPath.section == 1)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"TimeTakenCell"];
        UILabel *lblTitle = (UILabel *)[cell.contentView viewWithTag:103];
        lblTitle.text = [NSString stringWithFormat:@"%@", _package.startdate == nil ? @"Not started yet" : _package.startdate];
        
        return cell;

    }
    if(indexPath.section == 2)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"NotesCell"];
        UITextView *lblTitle = (UITextView *)[cell.contentView viewWithTag:104];
        lblTitle.text = _package.instruction;
        return cell;

        
    }
    if(indexPath.section == 3)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"MoreInfoCell"];
        return cell;

    }
    if(indexPath.section == 4)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"TakingNowCell"];
        return cell;

    }
    if(indexPath.section == 5)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"AlreadyTakenCell"];
        cell.selectionStyle  = UITableViewCellSelectionStyleNone;
        
        [self showHidePickerControls:cell];
        
        
        return cell;

    }
    if(indexPath.section == 6)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"SkipCell"];
        
        cell.selectionStyle  = UITableViewCellSelectionStyleNone;
        
        [self showHidePickerSkipControls:cell];

        return cell;

    }
    
    
    return cell;
}

-(void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:5]];
    pickerShown = NO;
    pickerShownSkip = NO;

    [self.tableView beginUpdates];
    [self.tableView endUpdates];
    [self showHidePickerControls:cell];
    [self showHidePickerSkipControls:cell];

}


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    if(indexPath.section == 4)
    {
        
        [UIView animateWithDuration:0.4 animations:^{
            
            self.tabBarController.tabBar.hidden = YES;
            
            self.navigationController.navigationBarHidden = YES;
            
            MainViewController *controller = [[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil];
            
            controller.package = _package;
            
            [self.navigationController pushViewController:controller animated:YES];
        }];

        
        /*[MBProgressHUD showHUDAddedTo:self.view animated:YES];
        [[OTMhelper sharedInstance] startPackage:_package completionBlock:^(BOOL result, NSError *error) {
            [MBProgressHUD hideAllHUDsForView:self.view animated:YES];
            if(!error)
            {
                dispatch_async(dispatch_get_main_queue(),^{
                    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Success!" message:@"Medication packaging taking started!" delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                    [alert show];
                    
                });
                
                [self.tableView reloadData];

            }
            else
            {
                dispatch_async(dispatch_get_main_queue(),^{
                    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.localizedDescription delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                    [alert show];
                    
                });
            }
        }];*/
        
        return;
    }
    if(indexPath.section == 5)
    {
        if(!pickerShown)
        {
            
            UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
            pickerShown = YES;
            [tableView beginUpdates];
            [tableView endUpdates];
            [self showPickerFromTextField:cell];
            [self showHidePickerControls:cell];
            [tableView scrollToRowAtIndexPath:indexPath atScrollPosition:UITableViewScrollPositionBottom animated:YES];
        }
        return;
    }
    if(indexPath.section == 6)
    {
        if(!pickerShownSkip)
        {
            
            UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
            pickerShownSkip = YES;
            [tableView beginUpdates];
            [tableView endUpdates];
            [self showPickerSkip:cell];
            [self showHidePickerSkipControls:cell];
            [tableView scrollToRowAtIndexPath:indexPath atScrollPosition:UITableViewScrollPositionBottom animated:YES];
        }
        return;
    }
    /*if(pickerShown)
    {
        UITableViewCell *cell = [tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:5]];

        UIView *v = [cell.contentView viewWithTag:1001];
        if(v)
        {
            [v removeFromSuperview];
        }
        pickerShown = NO;
        [tableView beginUpdates];
        [tableView endUpdates];
        [self showHidePickerControls:cell];

    }*/
   
    
}

@end
