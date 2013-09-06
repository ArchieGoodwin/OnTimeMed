//
//  OTMLoginViewController.m
//  ExtractPack
//
//  Created by Nero Wolfe on 6/11/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMLoginViewController.h"
#import "FormKit.h"
#import "User.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "MEDNetworkHelper.h"
@interface OTMLoginViewController ()
{
    FKFormModel *formModel;

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
    
    
    formModel = [FKFormModel formTableModelForTableView:self.tableView
                                   navigationController:self.navigationController];
    
    
    [self loadModel];
}

-(void)loadModel
{

     
     
     [FKFormMapping mappingForClass:[User class] block:^(FKFormMapping *formMapping) {
     
         [formMapping sectionWithTitle:@"   " footer:@"" identifier:@"drugname"];


         [formMapping sectionWithTitle:@"   " footer:@"" identifier:@"drugname1"];
         
         [formMapping mapAttribute:@"login" title:@"Login:" type:FKFormAttributeMappingTypeText];
         
         [formMapping sectionWithTitle:@"   " identifier:@"drugtype3"];
         
         [formMapping mapAttribute:@"password" title:@"Password:" type:FKFormAttributeMappingTypePassword];

         
         /*[formMapping mapAttribute:@"drugtype"
         title:@"Type"
         showInPicker:YES
         selectValuesBlock:^NSArray *(id value, id object, NSInteger *selectedValueIndex){
         *selectedValueIndex = 1;
         return [NSArray arrayWithObjects:@"Pill", @"Spray", @"Liquid", nil];
         
         } valueFromSelectBlock:^id(id value, id object, NSInteger selectedValueIndex) {
         return value;
         
         } labelValueBlock:^id(id value, id object) {
         return value;
         
         }];*/
         
         [formMapping sectionWithTitle:@"   " identifier:@"drugtype2"];

         
         

         [formMapping buttonSave:@"Login" handler:^{
         
             
             NSLog(@"%@ %@", user.login, user.password);
             user.login = @"User12345";
             user.password = @"pass12345";
             [[MEDNetworkHelper sharedInstance] loginUser:user.login password:user.password completionBlock:^(NSDictionary *result, NSError *error) {
                 if(result)
                 {
                     user.userId = [NSString stringWithFormat:@"%@",[result objectForKey:@"Id"]];
                     user.name = [result objectForKey:@"Name"];
                     [[NSUserDefaults standardUserDefaults] setObject:@"done" forKey:@"login"];
                     [[NSUserDefaults standardUserDefaults] synchronize];
                     [formModel save];
                     [User saveDefaultContext];
                     
                     [self dismissViewControllerAnimated:YES completion:^{
                        
                          [[NSNotificationCenter defaultCenter] postNotificationName:@"logindone" object:self userInfo:nil];
                     }];
                 }
                 else
                 {
                     [User deleteInContext:user];
                     [User saveDefaultContext];
                 }
                 
                 
             }];
             
         
         }];
         

         
         
         [formModel registerMapping:formMapping];
     }];
     
     [formModel setDidChangeValueWithBlock:^(id object, id value, NSString *keyPath) {
         NSLog(@"did change model value");
     }];
    formModel.editable = YES;
     [formModel loadFieldsWithObject:user];
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
