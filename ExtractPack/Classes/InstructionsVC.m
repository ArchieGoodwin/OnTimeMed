//
//  InstructionsVC.m
//  ExtractPack
//
//  Created by Nero Wolfe on 29/10/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "InstructionsVC.h"
#import "ExtractPackAppDelegate.h"
#import "MEDNetworkHelper.h"
#import "MedPackage.h"
@interface InstructionsVC ()

@end

@implementation InstructionsVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    _txtInstructions.text = _package.instruction;
    // Do any additional setup after loading the view from its nib.
}

- (IBAction)btnTookMedication:(id)sender {
    
    [[MEDNetworkHelper sharedInstance] postEvent:EVENTTakeMedication packageid:_package.packageid.integerValue completionBlock:^(BOOL result, NSError *error) {
        NSLog(@"EVENTTakeMedication sent");
        if(result)
        {
        }
        else
        {
            dispatch_async(dispatch_get_main_queue(),^{
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.localizedDescription delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                [alert show];
                
            });
        }
        
        
    }];
    
    
    
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    appDelegate.m_fCapturing = true;
	[_parentController toggleView: VMODE_CAMERA nState:0];
}


- (IBAction)btnCancel:(id)sender {
    
    
    [[MEDNetworkHelper sharedInstance] postEvent:EVENTSkipMedication packageid:_package.packageid.integerValue completionBlock:^(BOOL result, NSError *error) {
        NSLog(@"EVENTSkipMedication sent");
        if(result)
        {
        }
        else
        {
            dispatch_async(dispatch_get_main_queue(),^{
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:error.localizedDescription delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                [alert show];
                
            });
        }
        
        
    }];
    
    
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    appDelegate.m_fCapturing = false;
	[_parentController toggleView: VMODE_FIRST nState:0];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
