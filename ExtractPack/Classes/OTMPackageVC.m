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
@interface OTMPackageVC ()

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
        case 4:
        case 5:
        case 6:
            return 44;
        default:
            break;
    }
    
    return 44;
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
        return cell;

    }
    if(indexPath.section == 6)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"SkipCell"];
        return cell;

    }
    
    
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(indexPath.section == 4)
    {
        [MBProgressHUD showHUDAddedTo:self.view animated:YES];
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
        }];
    }
}

@end
