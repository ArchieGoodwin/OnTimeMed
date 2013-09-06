//
//  MEDAddEditPrescriptionViewController.m
//  MED
//
//  Created by Nero Wolfe on 5/23/13.
//  Copyright (c) 2013 Incoding. All rights reserved.
//

#import "MEDAddEditPrescriptionViewController.h"
#import "FormKit.h"
//#import "Prescription.h"
#import "BPBarButtonItem.h"
#import "MEDNetworkHelper.h"
#import "OTMhelper.h"
#import "NSManagedObject+NWCoreDataHelper.h"
@interface MEDAddEditPrescriptionViewController ()
{
    FKFormModel *formModel;
}
@end

@implementation MEDAddEditPrescriptionViewController



- (id)init {
    self = [super initWithStyle:UITableViewStyleGrouped];
    if (self) {
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    self.navigationItem.leftBarButtonItem = [BPBarButtonItem barButtonItemWithStyle:BPBarButtonItemStyleAction title:@"Cancel" target:self action:@selector(back)];

    
    self.navigationItem.rightBarButtonItem = [BPBarButtonItem barButtonItemWithStyle:BPBarButtonItemStyleAction title:@"Edit" target:self action:@selector(editPackage)];

    
    UIView *backView = [[UIView alloc] init];
    [backView setBackgroundColor:[UIColor colorWithRed:208.0/255.0
                                                 green:218.0/255.0
                                                  blue:228.0/255.0
                                                 alpha:1.0]];
    [self.tableView setBackgroundView:backView];


    formModel = [FKFormModel formTableModelForTableView:self.tableView
                                        navigationController:self.navigationController];
    
       
    [self loadModel];
    
    
}



-(void)back
{
    
    if(self.isNew)
    {
        
        
        
        //[Prescription deleteInContext:_prescription];
        //_prescription = nil;
        //[Prescription saveDefaultContext];
        
        
       // [Prescription resetDefaultContext];
        //[Prescription saveDefaultContext];
        
    }
    else
    {
       // [Prescription resetDefaultContext];
        //[Prescription saveDefaultContext];
    }
   
    [self.navigationController popToRootViewControllerAnimated:YES];
}

-(void)editPackage
{
    //[Prescription saveDefaultContext];

}

-(void)save
{
    //[Prescription saveDefaultContext];
    [self.navigationController popToRootViewControllerAnimated:YES];
}

-(void)loadModel
{
    
    
    [FKFormMapping mappingForClass:[MedPackage class] block:^(FKFormMapping *formMapping) {
        
        
        
        
        
        [formMapping sectionWithTitle:@"Doctor and Pharmacy" footer:@"" identifier:@"d01"];
        
        [formMapping mapAttribute:@"doctorname" title:@"Doctor name" type:FKFormAttributeMappingTypeText];
        
        [formMapping mapAttribute:@"pharmacyname" title:@"Pharmacy name" type:FKFormAttributeMappingTypeText];

        
        [formMapping sectionWithTitle:@"Medication" identifier:@"d02"];
        
        /*[formMapping mapAttribute:@"drugtype"
                            title:@"Type"
                     showInPicker:YES
                selectValuesBlock:^NSArray *(id value, id object, NSInteger *selectedValueIndex){
                    *selectedValueIndex = 1;
                    return [NSArray arrayWithObjects:@"Pill", @"Spray", @"Liquid", nil];
                    
                } valueFromSelectBlock:^id(id value, id object, NSInteger selectedValueIndex) {
                    return value;
                    
                } labelValueBlock:^id(id value, id object) {
                    return value;
                    
                }];*/
        
        [formMapping mapAttribute:@"medicationname" title:@"Medication name" type:FKFormAttributeMappingTypeText];
        
        
        [formMapping mapAttribute:@"instruction" title:@"Instructions" type:FKFormAttributeMappingTypeBigText];


        [formMapping mapAttribute:@"doseperpackage" title:@"Dose per package" type:FKFormAttributeMappingTypeInteger];

        [formMapping mapAttribute:@"doseperusage" title:@"Dose per usage" type:FKFormAttributeMappingTypeInteger];

        [formMapping mapAttribute:@"dosespackcount" title:@"Dose pack count" type:FKFormAttributeMappingTypeInteger];

        [formMapping mapAttribute:@"frequency" title:@"Frequency" type:FKFormAttributeMappingTypeInteger];
        
        [formMapping mapAttribute:@"manufacturedate" title:@"Made date" type:FKFormAttributeMappingTypeDate];


        switch ([_package.status integerValue]) {
            case PS_opened:
            {
                [formMapping buttonSave:@"Start package" handler:^{

                    [[OTMhelper sharedInstance] createNotifications:[NSDate date] package:_package completeBlock:^(BOOL result, NSError *error) {
                        
                        _package.status = [NSNumber numberWithInt:PS_used];
                        [MedPackage saveDefaultContext];
                        
                        [[MEDNetworkHelper sharedInstance] postEvent:EVENTSetMedicationScheduleStartTime packageid:[_package.packageid integerValue] completionBlock:^(BOOL result, NSError *error) {
                            if(result)
                            {
                                NSLog(@"EVENTSetMedicationScheduleStartTime sent");

                            }
                            else
                            {
                                dispatch_async(dispatch_get_main_queue(),^{
                                    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.description delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                                    [alert show];
                                    
                                });
                            }
                        }];
                        
                        dispatch_async(dispatch_get_main_queue(),^{
                            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Alert!" message:[NSString stringWithFormat:@"Package with %@ started!", _package.medicationname] delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                            [alert show];
                            
                        });
                    }];
                    
                    
                    [self.navigationController popToRootViewControllerAnimated:YES];
                }];
                break;

            }
            case PS_used:
            {
                [formMapping buttonSave:@"Package started" handler:^{

                    [self.navigationController popToRootViewControllerAnimated:YES];
                }];
                break;
                
            }
                
            default:
                break;
        }
        
        
        [formModel registerMapping:formMapping];
    }];
    
    [formModel setDidChangeValueWithBlock:^(id object, id value, NSString *keyPath) {
        NSLog(@"did change model value");
    }];
    
    [formModel loadFieldsWithObject:_package];
    formModel.editable = NO;
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
