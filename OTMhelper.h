//
//  OTMhelper.h
//  ExtractPack
//
//  Created by Nero Wolfe on 6/11/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <Foundation/Foundation.h>
@class User;
@class MedPackage;

typedef void (^HelperResult)  (BOOL result, NSError *error);

@interface OTMhelper : NSObject


+(OTMhelper *)sharedInstance;
-(User *)getCurrentUser;
-(MedPackage *)getPackageById:(NSString *)packageId;
-(MedPackage *)createPackage:(NSDictionary *)dict;
-(void)createNotifications:(NSDate *)date package:(MedPackage *)package completeBlock:(HelperResult)completeBlock;
-(NSMutableArray *)getMedicationdates:(MedPackage *)package date:(NSDate *)date;
@end
