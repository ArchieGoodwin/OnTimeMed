//
//  OTMhelper.m
//  ExtractPack
//
//  Created by Nero Wolfe on 6/11/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "OTMhelper.h"
#import "User.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "MedPackage.h"
#import "MEDNetworkHelper.h"
#import "History.h"
@implementation OTMhelper


-(void)deleteAll
{
    for(MedPackage *pack in [MedPackage getAllRecords])
    {
        [MedPackage deleteInContext:pack];
    }
    for(History *pack in [History getAllRecords])
    {
        [History deleteInContext:pack];
    }
    
    [MedPackage saveDefaultContext];
}

-(User *)getCurrentUser
{
    NSMutableArray *array = [User getAllRecords];
    if(array.count > 0)
    {
        return array[0];
    }
    return nil;

}


-(NSString *)returnEventXML:(NSInteger)eventType
{
    switch (eventType) {
        case EVENTDeviceAlarmWrongMedScanned:
            return @"<FAILEDSCAN><time>%@</time><date>%@</date><correct_code>%@</correct_code><incorrect_code>%@</incorrect_code><userid>%@</userid></FAILEDSCAN>";
        case EVENTScanBarcode:
            return @"<GOODSCAN><time>%@</time><date>%@</date><order_number>%@</order_number><correct_code>%@</correct_code><userid>%@</userid></GOODSCAN>";
        case EVENTHitSnooze:
            return @"<TAKEX><time>%@</time><date>%@</date><order_number>%@</order_number><correct_code>%@</correct_code><userid>%@</userid></TAKEX>";
        case EVENTTakeMedication:
            return @"<TOOK><time>%@</time><date>%@</date><order_number>%@</order_number><correct_code>%@</correct_code><userid>%@</userid></TOOK>";
        case EVENTDeviceAlarmTakeMed:
            return @"<TOOKO><time>%@</time><date>%@</date><entered_time>%@</entered_time><entered_date>%@</entered_date><scheduled_time>%@</scheduled_time><scheduled_date>%@</scheduled_date><order_number>%@</order_number><userid>%@</userid></TOOKO>";
        case EVENTSkipMedication:
            return @"<SKIP><time>%@</time><date>%@</date><entered_time>%@</entered_time><entered_date>%@</entered_date><scheduled_time>%@</scheduled_time><scheduled_date>%@</scheduled_date><order_number>%@</order_number><userid>%@</userid></SKIP>";
        case EVENTTakePictureAfter:
            return @"<TOOKPICT><time>%@</time><date>%@</date><order_number>%@</order_number><correct_code>%@</correct_code><userid>%@</userid></TOOKPICT>";
        case EVENTTakePictureBefore:
            return @"<TOOKPICTBEFORE><time>%@</time><date>%@</date><order_number>%@</order_number><correct_code>%@</correct_code><userid>%@</userid></TOOKPICTBEFORE>";
        case EVENTSetMedicationScheduleStartTime:
            return @"<START><time>%@</time><date>%@</date><order_number>%@</order_number><correct_code>%@</correct_code><userid>%@</userid></START>";
        case EVENTRegisterUserOnDevice:
            return @"<REGISTER><time>%@</time><date>%@</date><userid>%@</userid></REGISTER>";

            
        default:
            break;
    }
    
    return @"";
}





-(NSString *)returnEvent:(NSInteger)eventType
{
    

    switch (eventType) {
        case EVENTRegisterUserOnDevice:
            return @"User logged in";
        case EVENTPullPrescriptionData:
            return @"Medication info received from server";
        case EVENTTakeMedication:
            return @"Medication taking";
        case EVENTScanBarcode:
            return @"Bar code scanned";
        case EVENTTakePictureBefore:
            return @"Picture taken before start medication";
        case EVENTTakePictureAfter:
            return @"Picture taken after start medication";
        case EVENTSkipMedication:
            return @"Medication skipped";
        case EVENTSetMedicationScheduleStartTime:
            return @"Medication started. Start time set";
        case EVENTSetMedicationScheduleStopTime:
            return @"Medication stopped. Start time set";
        case EVENTHitSnooze:
            return @"Medication snoozed";
        case EVENTUsageWarning:
            return @"Usage warning";
        case EVENTDeviceAlarmTakeMed:
            return @"Medication taken after alarm";
        case EVENTDeviceAlarmNotTakeMed:
            return @"Medication not taken after alarm";
        case EVENTDeviceAlarmWrongMedScanned:
            return @"Wrong medication scanned";
        case EVENTDeviceAlarmBadImage:
            return @"Bad image scanned";
        default:
            break;
    }
}

-(void)startPackage:(MedPackage *)package completionBlock:(HelperResult)completionBlock
{
    switch ([package.status integerValue]) {
        case PS_opened:
        {
            
                [self createNotifications:[NSDate date] package:package completeBlock:^(BOOL result, NSError *error) {
                    
                    package.status = [NSNumber numberWithInt:PS_used];
                    [MedPackage saveDefaultContext];
                    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
                    [formatter setDateFormat:@"hh:mm:ss a"];
                    NSString *time = [formatter stringFromDate:[NSDate date]];
                    [formatter setDateFormat:@"MM/dd/yyyy"];
                    NSString *date = [formatter stringFromDate:[NSDate date]];
                    
                    NSString *xml = [self returnEventXML:EVENTSetMedicationScheduleStartTime];
                    xml = [NSString stringWithFormat:xml, time, date, package.packageid, package.barcode, [self getCurrentUser].userId];
                    
                    
                    [[MEDNetworkHelper sharedInstance] postEvent:EVENTSetMedicationScheduleStartTime packageid:[package.packageid integerValue] xmlString:xml obj:nil completionBlock:^(BOOL result, NSError *error) {
                        if(result)
                        {
                            NSLog(@"EVENTSetMedicationScheduleStartTime sent");
                            completionBlock(YES, nil);
                        }
                        else
                        {
                           /* dispatch_async(dispatch_get_main_queue(),^{
                                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.description delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                                [alert show];
                                
                            });*/
                            
                           completionBlock(NO, error);
                        }
                    }];
                    
                   
                }];
                
                
            break;
            
        }
        case PS_used:
        {

            break;
            
        }
            
        default:
            break;
    }
}

-(MedPackage *)createPackage:(NSDictionary *)dict
{
    MedPackage *md = [MedPackage createEntityInContext];
    md.barcode = [NSString stringWithFormat:@"%@",[dict objectForKey:@"Barcode"]];
    md.doctorname = [dict objectForKey:@"DoctorName"];
    md.doseperpackage = [dict objectForKey:@"DosePerPackage"];
    md.doseperusage = [dict objectForKey:@"DosePerUsage"];
    md.dosespackcount = [dict objectForKey:@"DosesPackCount"];
    md.frequency = [dict objectForKey:@"Frequency"];
    md.packageid = [dict objectForKey:@"Id"];
    md.instruction = [dict objectForKey:@"Instructions"] == [NSNull null] ? @"" : [dict objectForKey:@"Instructions"] ;
    
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"MM/dd/yyyy hh:mm:ss a"];
    NSDate *date = [formatter dateFromString:[dict objectForKey:@"ManufactureDate"]];
    md.manufacturedate = date;
    
    md.medicationname = [dict objectForKey:@"MedicationName"];
    md.pharmacyname = [dict objectForKey:@"PharmacyName"];
    md.reorder = [dict objectForKey:@"ReOrder"];
    md.status = [dict objectForKey:@"Status"];
    md.takedrug = [dict objectForKey:@"TakeDrug"];
    md.username = [dict objectForKey:@"UserName"];
    
    formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"MM/dd/yyyy hh:mm:ss a"];
    date = [formatter dateFromString:[dict objectForKey:@"VerificationDate"]];
    md.verificationdate = date;
    
    md.verifiedbyid = [dict objectForKey:@"VerifiedByID"];
    
    md.user = [self getCurrentUser];
    
    [MedPackage saveDefaultContext];
    
    return md;
}


-(NSMutableArray *)getNumberOfDays:(NSInteger)number_of_days  freq:(int)freq fromdate:(NSDate *)fromdate
{
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    
    NSDate *today = fromdate;
    
    NSCalendar *gregorian = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
    
    for (int i =  0; i < number_of_days; i++)
    {
        NSDateComponents *offsetComponents = [[NSDateComponents alloc] init];
        [offsetComponents setHour:i * freq];
        NSDate *nextDate = [gregorian dateByAddingComponents:offsetComponents toDate:today options:0];
        
        [arr addObject:nextDate];
        NSLog(@"%@ round %i", nextDate, i);
    }

    return arr;
}

- (NSDate *)dateToGMT:(NSDate *)sourceDate {
    NSTimeZone* destinationTimeZone = [NSTimeZone systemTimeZone];
    NSInteger destinationGMTOffset = [destinationTimeZone secondsFromGMTForDate:sourceDate];
    NSDate* destinationDate = [[NSDate alloc] initWithTimeInterval:destinationGMTOffset sinceDate:sourceDate];
    return destinationDate;
	//return sourceDate;
}


-(NSMutableArray *)getMedicationdates:(MedPackage *)package date:(NSDate *)date
{
    int totaldose = [package.doseperpackage integerValue];
    int doseperusage = [package.doseperusage integerValue];
    int frequency = [package.frequency integerValue];
    int days = totaldose / doseperusage;
    
    NSMutableArray *daysArr = [self getNumberOfDays:days freq:frequency fromdate:[self dateToGMT:date]];
    
    return daysArr;
}

-(void)createNotifications:(NSDate *)date package:(MedPackage *)package completeBlock:(HelperResult)completeBlock
{
    package.startdate = date;
    [MedPackage saveDefaultContext];
    //ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    //[appDelegate clearNotifications];
    //[[UIApplication sharedApplication] setApplicationIconBadgeNumber: 1];
    //[[UIApplication sharedApplication] setApplicationIconBadgeNumber: 0];
    //[[UIApplication sharedApplication] cancelAllLocalNotifications];
    
    
       
    NSMutableArray *daysArr = [self getMedicationdates:package date:date];
    
    for(int i = 1; i < daysArr.count; i++)
    {
        /*NSCalendar *calendar = [NSCalendar autoupdatingCurrentCalendar];

        NSDate *pickerDate = [NSDate date];
        NSDateComponents *dateComponents = [calendar components:( NSYearCalendarUnit | NSMonthCalendarUnit |  NSDayCalendarUnit )
                                                       fromDate:pickerDate];
        NSDateComponents *timeComponents = [calendar components:( NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit )
                                                       fromDate:pickerDate];
        
        // Set up the fire time
        NSDateComponents *dateComps = [[NSDateComponents alloc] init];
        [dateComps setDay:[dateComponents day]];
        [dateComps setMonth:[dateComponents month]];
        [dateComps setYear:[dateComponents year]];
        [dateComps setHour:[timeComponents hour]];
        [dateComps setMinute:[timeComponents minute]];
        [dateComps setSecond:[timeComponents second] + 10 + i];
        NSDate *itemDate = [calendar dateFromComponents:dateComps];*/



        UILocalNotification *localNotif = [[UILocalNotification alloc] init];
        if (localNotif == nil)
            return;
        localNotif.fireDate = daysArr[i];
        localNotif.timeZone = [NSTimeZone defaultTimeZone];
        
        // Notification details
        localNotif.alertBody = [NSString stringWithFormat:@"It is time to take %@", package.medicationname];
        // Set the action button
        localNotif.alertAction = @"View";
        
        localNotif.soundName = UILocalNotificationDefaultSoundName;
        localNotif.applicationIconBadgeNumber = 1;
        
        // Specify custom data for the notification
        NSDictionary *infoDict = [NSDictionary dictionaryWithObject:[NSString stringWithFormat:@"%i",[package.packageid integerValue]] forKey:@"packageId"];
        localNotif.userInfo = infoDict;
        
        [[UIApplication sharedApplication] scheduleLocalNotification:localNotif];
    }
    
    if(completeBlock)
    {
        completeBlock(YES,nil);
    }
    
}


-(MedPackage *)getPackageById:(NSString *)packageId
{
    NSPredicate *predicate = [[NSPredicate alloc] init];
    predicate = [NSPredicate predicateWithFormat:@"packageid = %@", packageId];
    MedPackage *md = [MedPackage getSingleObjectByPredicate:predicate];
    if(md != nil)
    {
        return md;
    }
    return nil;
    
}


#pragma mark - init methods

- (id)init {
    self = [super init];
    
    
#if !(TARGET_IPHONE_SIMULATOR)
    
    
#else
    
    
#endif
    
    return self;
    
}

+(id)sharedInstance
{
    static dispatch_once_t pred;
    static OTMhelper *sharedInstance = nil;
    dispatch_once(&pred, ^{
        sharedInstance = [[OTMhelper alloc] init];
    });
    return sharedInstance;
}

- (void)dealloc
{
    
    abort();
}

@end
