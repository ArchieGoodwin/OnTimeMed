//
//  ScheduleViewController.m
//  ExtractPack
//
//  Created by Nero Wolfe on 9/6/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "ScheduleViewController.h"
#import "User.h"
#import "MedPackage.h"
#import "OTMhelper.h"
#import "Schedule.h"
#import "CKCalendarView.h"
#import "NSDate-Utilities.h"
#import "DKADefines.h"
#import "OTMPackageVC.h"
@interface ScheduleViewController () <CKCalendarDelegate>
{
    NSMutableArray *dates;
}
@property(nonatomic, weak) CKCalendarView *calendar;
@property(nonatomic, strong) NSDateFormatter *dateFormatter;
@property(nonatomic, strong) NSDate *minimumDate;
@property (nonatomic, strong) NSArray *packages;

@end

@implementation ScheduleViewController



- (void)viewDidLoad
{
    [super viewDidLoad];

    

    
    
    self.navigationItem.title = @"Medication Schedule";
    
    self.table.separatorInset = UIEdgeInsetsMake(0, 10, 0, 10);

    
    User *user = [[OTMhelper sharedInstance] getCurrentUser];
    
    _packages = [user.medpackages allObjects];
    
    NSMutableArray *temp = [NSMutableArray new];
    
    for(MedPackage *package in _packages)
    {
        
        if(package.startdate != nil)
        {
            NSMutableArray *dosesDates = [[OTMhelper sharedInstance] getMedicationdates:package date:package.startdate];
            
            for(NSDate *date in dosesDates)
            {
                Schedule *sc = [[Schedule alloc] init];
                sc.date = date;
                sc.package = package;
                [temp addObject:sc];
            }
        }
        
    }
    
    NSSortDescriptor *descriptor = [[NSSortDescriptor alloc] initWithKey:@"date" ascending:YES];
    NSArray *descriptors = [NSArray arrayWithObject: descriptor];
    dates = [[temp sortedArrayUsingDescriptors:descriptors] mutableCopy];

    
    [self createCalendar];
}

-(void)createCalendar
{
    CKCalendarView *calendar = [[CKCalendarView alloc] initWithStartDay:startMonday];
    self.calendar = calendar;
    calendar.delegate = self;
    
    self.dateFormatter = [[NSDateFormatter alloc] init];
    [self.dateFormatter setDateFormat:@"dd/MM/yyyy"];
    self.minimumDate = [self.dateFormatter dateFromString:@"20/09/2012"];
    
    /*self.disabledDates = @[
     [self.dateFormatter dateFromString:@"05/01/2013"],
     [self.dateFormatter dateFromString:@"06/01/2013"],
     [self.dateFormatter dateFromString:@"07/01/2013"]
     ];*/
    
    calendar.onlyShowCurrentMonth = NO;
    calendar.adaptHeightToNumberOfWeeksInMonth = NO;
    
    calendar.frame = CGRectMake(0, 60, 320, 217);
    
    [self.view addSubview:calendar];
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
    return dates.count;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 50;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    cell = nil;
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    Schedule *sc = [dates objectAtIndex:indexPath.row];
    
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
    //[dateFormat setTimeZone:[NSTimeZone timeZoneWithName:@"GMT"]];
    [dateFormat setDateFormat:@"MM-dd-yyyy hh:mma"];
    NSString *dateStr = [dateFormat stringFromDate:sc.date];
    NSString *str = [NSString stringWithFormat:@"Package %@", sc.package.medicationname];
    
    
    UILabel *lblTitle = [[UILabel alloc] initWithFrame:CGRectMake(10, 5, 300, 25)];
    lblTitle.backgroundColor = [UIColor clearColor];
    lblTitle.numberOfLines = 0;
    lblTitle.lineBreakMode = NSLineBreakByWordWrapping;
    lblTitle.font = [UIFont boldSystemFontOfSize:16];
    lblTitle.textColor = [UIColor blackColor];
    
    lblTitle.text =  str;
    
    [cell.contentView addSubview:lblTitle];
    
    UILabel *lblSub = [[UILabel alloc] initWithFrame:CGRectMake(10, 25, 300, 25)];
    lblSub.backgroundColor = [UIColor clearColor];
    lblSub.numberOfLines = 0;
    lblSub.lineBreakMode = NSLineBreakByWordWrapping;
    lblSub.font = [UIFont boldSystemFontOfSize:13];
    lblSub.textColor = [UIColor grayColor];
    
    lblSub.text =  dateStr;
    
    [cell.contentView addSubview:lblSub];
    
    
    // Configure the cell...
    
    return cell;
}



#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    Schedule *sc = [dates objectAtIndex:indexPath.row];

    
    
    [self performSegueWithIdentifier:@"showPackageFromSchedule" sender:sc.package];
}


-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if([segue.identifier isEqualToString:@"showPackageFromSchedule"])
    {
        MedPackage *pack = (MedPackage *)sender;
        OTMPackageVC *controller = (OTMPackageVC *)segue.destinationViewController;
        controller.package = pack;
    }
}


#pragma mark - CKCalendarDelegate

- (void)calendar:(CKCalendarView *)calendar configureDateItem:(CKDateItem *)dateItem forDate:(NSDate *)date {
    
    
    for(Schedule *sc in dates)
    {
        if([date isEqualToDateIgnoringTime:sc.date])
        {
            dateItem.backgroundColor = MAIN_BACK_COLOR;
            dateItem.textColor = [UIColor darkTextColor];
            
            
        }
        
    }
    
}

- (BOOL)calendar:(CKCalendarView *)calendar willSelectDate:(NSDate *)date {
    return YES;
}


- (void)calendar:(CKCalendarView *)calendar didSelectDate:(NSDate *)date {
    //self.dateLabel.text = [self.dateFormatter stringFromDate:date];
    
    int i = 0;
    for(Schedule *sc in dates)
    {
        if([date isEqualToDateIgnoringTime:sc.date])
        {
            
            CGPoint offset = CGPointMake(0, 50 * i - 50);
            [self.table setContentOffset:offset animated:YES];
            
            return;
        }
        i++;
    }
    
    
}

- (BOOL)calendar:(CKCalendarView *)calendar willChangeToMonth:(NSDate *)date {
    if ([date laterDate:self.minimumDate] == date) {
        self.calendar.backgroundColor = MAIN_BACK_COLOR;
        return YES;
    } else {
        self.calendar.backgroundColor = MAIN_BACK_COLOR;
        return NO;
    }
}

- (void)calendar:(CKCalendarView *)calendar didLayoutInRect:(CGRect)frame {
    NSLog(@"calendar layout: %@", NSStringFromCGRect(frame));
}


@end
