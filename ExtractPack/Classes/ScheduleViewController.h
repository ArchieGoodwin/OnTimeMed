//
//  ScheduleViewController.h
//  ExtractPack
//
//  Created by Nero Wolfe on 9/6/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ScheduleViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *table;

@end
