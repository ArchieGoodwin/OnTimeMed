//
//  MedPackage.h
//  ExtractPack
//
//  Created by Nero Wolfe on 6/11/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class History;

@interface MedPackage : NSManagedObject

@property (nonatomic, retain) NSString * barcode;
@property (nonatomic, retain) NSString * doctorname;
@property (nonatomic, retain) NSNumber * doseperpackage;
@property (nonatomic, retain) NSNumber * doseperusage;
@property (nonatomic, retain) NSNumber * dosespackcount;
@property (nonatomic, retain) NSDate * enddate;
@property (nonatomic, retain) NSDate * expdate;
@property (nonatomic, retain) NSNumber * frequency;
@property (nonatomic, retain) NSString * instruction;
@property (nonatomic, retain) NSDate * manufacturedate;
@property (nonatomic, retain) NSString * medicationname;
@property (nonatomic, retain) NSNumber * packageid;
@property (nonatomic, retain) NSString * pharmacyname;
@property (nonatomic, retain) NSNumber * reorder;
@property (nonatomic, retain) NSDate * startdate;
@property (nonatomic, retain) NSNumber * status;
@property (nonatomic, retain) NSString * takedrug;
@property (nonatomic, retain) NSString * username;
@property (nonatomic, retain) NSDate * verificationdate;
@property (nonatomic, retain) NSNumber * verifiedbyid;
@property (nonatomic, retain) NSManagedObject *user;
@property (nonatomic, retain) NSSet *events;
@end

@interface MedPackage (CoreDataGeneratedAccessors)

- (void)addEventsObject:(History *)value;
- (void)removeEventsObject:(History *)value;
- (void)addEvents:(NSSet *)values;
- (void)removeEvents:(NSSet *)values;

@end
