//
//  MEDAddEditPrescriptionViewController.h
//  MED
//
//  Created by Nero Wolfe on 5/23/13.
//  Copyright (c) 2013 Incoding. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MedPackage.h"
//@class Prescription;
@interface MEDAddEditPrescriptionViewController : UITableViewController


@property (nonatomic, assign) BOOL isNew;
@property (nonatomic, strong)    MedPackage  *package;

-(void)loadModel;
@end
