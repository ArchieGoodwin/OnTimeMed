//
//  OTMHistoryVC.m
//  ExtractPack
//
//  Created by Nero Wolfe on 28/10/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMHistoryVC.h"
#import "History.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "OTMhelper.h"
#import "User.h"
@interface OTMHistoryVC ()
{
    NSMutableArray *history;
}
@end

@implementation OTMHistoryVC

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
    
    self.tableView.separatorInset = UIEdgeInsetsMake(0, 10, 0, 10);

    
    history = [NSMutableArray new];
    

}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    NSPredicate *predicate = [[NSPredicate alloc] init];
    predicate = [NSPredicate predicateWithFormat:@"userId = %@", [[OTMhelper sharedInstance] getCurrentUser].userId];
    
    history = [History getFilteredRecordsWithSortedPredicate:predicate key:@"date" ascending:NO];
    
    [self.tableView reloadData];
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

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return history.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"HistoryCell"];

    
    UILabel *lblTitle = (UILabel *)[cell.contentView viewWithTag:101];
    UILabel *lblDate = (UILabel *)[cell.contentView viewWithTag:102];

    History *his = [history objectAtIndex:indexPath.row];
    
    
    lblTitle.text = [[OTMhelper sharedInstance] returnEvent:his.event.integerValue];
    
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"MM-dd-yyyy hh:mma"];
    NSString *dateStr = [dateFormat stringFromDate:his.date];
    
    lblDate.text = dateStr;
    return cell;
}



@end
