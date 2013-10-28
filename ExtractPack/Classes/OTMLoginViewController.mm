//
//  OTMLoginViewController.m
//  ExtractPack
//
//  Created by Nero Wolfe on 6/11/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMLoginViewController.h"
#import "User.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "MEDNetworkHelper.h"
#import "MBProgressHUD.h"
@interface OTMLoginViewController ()
{

    UITextField *txtLogin;
    UITextField *txtPwd;
    User *user;
}
@end

@implementation OTMLoginViewController

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

    if([[NSUserDefaults standardUserDefaults] objectForKey:@"login"] != nil)
    {
        [self performSegueWithIdentifier:@"mainEntrance" sender:user];

    }
    

    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    user = [User createEntityInContext];
    
    UIView *backView = [[UIView alloc] init];
    [backView setBackgroundColor:[UIColor colorWithRed:208.0/255.0
                                                 green:218.0/255.0
                                                  blue:228.0/255.0
                                                 alpha:1.0]];
    [self.tableView setBackgroundView:backView];
    
    
    
}

-(void)loginMe
{

    
    
    if(![txtLogin.text isEqualToString:@""] && ![txtPwd.text isEqualToString:@""])
    {
        [MBProgressHUD showHUDAddedTo:self.view animated:YES];

        
        [[MEDNetworkHelper sharedInstance] loginUser:txtLogin.text password:txtPwd.text completionBlock:^(NSDictionary *result, NSError *error) {
            
            [MBProgressHUD hideAllHUDsForView:self.view animated:YES];
            
            if(!error)
            {
                if(result)
                {
                    
                    [[MEDNetworkHelper sharedInstance] postEvent:EVENTRegisterUserOnDevice packageid:0 completionBlock:^(BOOL result, NSError *error) {
                        
                    }];
                    
                    
                    user.userId = [NSString stringWithFormat:@"%@",[result objectForKey:@"Id"]];
                    user.name = [result objectForKey:@"Name"];
                    [[NSUserDefaults standardUserDefaults] setObject:@"done" forKey:@"login"];
                    [[NSUserDefaults standardUserDefaults] synchronize];
                    [User saveDefaultContext];
                    
                    [self performSegueWithIdentifier:@"mainEntrance" sender:user];
                }
                else
                {
                    [User deleteInContext:user];
                    [User saveDefaultContext];
                }
            }
            else
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.localizedDescription delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
                [alert show];
            }
            
            
        }];
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Warning!" message:@"Please enter valid login and password to continue!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
        [alert show];
    }
    
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return 4;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.row) {
        case 0:
            return 95;
        case 1:
        case 2:
            return 70;
        case 3:
            return 44;
                default:
            break;
    }
    
    return 44;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"LogoCell"];
    
    if(indexPath.row == 0)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"LogoCell"];
        
        return cell;
        
    }
    
    if(indexPath.row == 1)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"LoginCell"];
        txtLogin = (UITextField *)[cell.contentView viewWithTag:101];
        
        return cell;
        
    }
    
    if(indexPath.row == 2)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"PwdCell"];
        txtPwd = (UITextField *)[cell.contentView viewWithTag:102];
        
        return cell;
        
    }
    
    if(indexPath.row == 3)
    {
        UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"BtnCell"];
        

        
        return cell;
        
    }
    
    
    
    // Configure the cell...
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(indexPath.row == 3)
    {
        [self loginMe];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
