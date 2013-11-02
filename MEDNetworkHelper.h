//
//  MEDNetworkHelper.h
//  ExtractPack
//
//  Created by Nero Wolfe on 6/7/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <Foundation/Foundation.h>
typedef void (^RCCompleteBlockWithDictResult)  (NSDictionary *result, NSError *error);
typedef void (^RCCompleteBlockWithArrayResult)  (NSArray *result, NSError *error);
typedef void (^RCCompleteBlockWithBOOLResult)  (BOOL result, NSError *error);

typedef enum EventOfType
{
    EVENTRegisterUserOnDevice = 1,
    EVENTPullPrescriptionData = 2,
    EVENTTakeMedication = 3,
    EVENTScanBarcode = 4,
    EVENTTakePictureBefore = 5,
    EVENTTakePictureAfter = 6,
    EVENTSkipMedication = 7,
    EVENTSetMedicationScheduleStartTime = 8,
    EVENTSetMedicationScheduleStopTime = 9,
    EVENTHitSnooze = 10,
    EVENTUsageWarning = 11,
    EVENTDeviceAlarmTakeMed = 12,
    EVENTDeviceAlarmNotTakeMed = 13,
    EVENTDeviceAlarmWrongMedScanned = 14,
    EVENTDeviceAlarmBadImage = 15,
} EventOfType;

typedef enum PackageStatus
{
    PS_notready = 0,
    PS_packaged = 1,
    PS_readyship = 2,
    PS_received = 3,
    PS_opened = 4,
    PS_used = 5,

} PackageStatus;
@interface MEDNetworkHelper : NSObject


+(MEDNetworkHelper *)sharedInstance;

-(void)getMedPackageByBarcodeId:(NSString *)barcodeId completionBlock:(RCCompleteBlockWithDictResult)completionBlock;
-(void)loginUser:(NSString *)login password:(NSString *)password completionBlock:(RCCompleteBlockWithDictResult)completionBlock;
-(void)getPackagesByUserId:(NSString *)userId completionBlock:(RCCompleteBlockWithArrayResult)completionBlock;
-(void)postEvent:(EventOfType)eventType packageid:(NSInteger)packageid xmlString:(NSString *)xmlString obj:(id)obj completionBlock:(RCCompleteBlockWithBOOLResult)completionBlock;
@end
