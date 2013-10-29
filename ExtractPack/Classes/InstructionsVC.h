//
//  InstructionsVC.h
//  ExtractPack
//
//  Created by Nero Wolfe on 29/10/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MedPackage.h"
#import "MainViewController.h"
@interface InstructionsVC : UIViewController

@property (nonatomic, strong) MedPackage *package;

@property (strong, nonatomic) IBOutlet UITextView *txtInstructions;
@property (nonatomic, strong) MainViewController *parentController;

@end
