//
//  OTMSettingsVC.m
//  ExtractPack
//
//  Created by Nero Wolfe on 28/10/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMSettingsVC.h"
#import "User.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "OTMhelper.h"
#import "ExtractPackAppDelegate.h"
@interface OTMSettingsVC ()

@end

@implementation OTMSettingsVC

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

    
    self.navigationItem.title = @"Settings";
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
    return 1;
}

-(NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    switch (section) {
        case 0:
            return @"Account Settings";
            
        default:
            break;
    }
    return @"";
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    switch (section) {
        case 0:
            return 1;
            break;
            
        default:
            break;
    }
    return 0;
    
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"LogOutCell"];
    
    switch (indexPath.section) {
        case 0:
        {
            UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"LogOutCell"];
            return cell;
        }
            
        default:
            break;
    }
    // Configure the cell...
    
    return cell;
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView.tag == 1001)
    {
        if(buttonIndex == 1)
        {
            [self logOut];
        }
    }
}

-(void)logOut
{
    [[NSUserDefaults standardUserDefaults] setObject:nil forKey:@"login"];
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    User *user = [[OTMhelper sharedInstance] getCurrentUser];
    
    if(user != nil)
    {
        [User deleteInContext:user];
        [User saveDefaultContext];
    }
    
    [[OTMhelper sharedInstance] deleteAll];
    
    ExtractPackAppDelegate *appDelegate =  (ExtractPackAppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDelegate resetWindowToInitialView];
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.section) {
        case 0:
        {
            if(indexPath.row == 0)
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Warning!" message:@"Are you sure you want to log out? You data will deleted." delegate:self cancelButtonTitle:@"NO" otherButtonTitles:@"YES", nil];
                alert.tag = 1001;
                [alert show];
            }
            
            break;
        }
            
        default:
            break;
    }

}

/*
#pragma mark - Navigation

// In a story board-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}

 */

@end
