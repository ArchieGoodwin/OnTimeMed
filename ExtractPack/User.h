//
//  User.h
//  ExtractPack
//
//  Created by Nero Wolfe on 6/12/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class MedPackage;

@interface User : NSManagedObject

@property (nonatomic, retain) NSString * login;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSString * password;
@property (nonatomic, retain) NSString * userId;
@property (nonatomic, retain) NSSet *medpackages;
@end

@interface User (CoreDataGeneratedAccessors)

- (void)addMedpackagesObject:(MedPackage *)value;
- (void)removeMedpackagesObject:(MedPackage *)value;
- (void)addMedpackages:(NSSet *)values;
- (void)removeMedpackages:(NSSet *)values;

@end
