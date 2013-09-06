//
//  History.h
//  ExtractPack
//
//  Created by Nero Wolfe on 6/11/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>


@interface History : NSManagedObject

@property (nonatomic, retain) NSString * userId;
@property (nonatomic, retain) NSNumber * packageId;
@property (nonatomic, retain) NSNumber * event;
@property (nonatomic, retain) NSDate * date;
@property (nonatomic, retain) NSManagedObject *package;

@end
