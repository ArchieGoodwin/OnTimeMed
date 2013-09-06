//
//  Schedule.h
//  ExtractPack
//
//  Created by Nero Wolfe on 9/6/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MedPackage.h"
@interface Schedule : NSObject

@property (strong, nonatomic) MedPackage *package;
@property (strong, nonatomic) NSDate *date;

@end
