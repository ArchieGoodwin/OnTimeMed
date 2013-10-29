//
//  ExtractPackViewController.m
//  ExtractPack
//
//
//  Copyright (c) 2013 A. Ahmadi All rights reserved.
//

#import "ExtractPackViewController.h"
#import "CaptureViewController.h"
#import "ExtractPackAppDelegate.h"
#import "FrameViewController.h"
#import "MEDNetworkHelper.h"

@interface ExtractPackViewController ()
{
}

@end

@implementation ExtractPackViewController
//@synthesize resultText;

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
    
    if(_package)
    {
        _lblMessage.text = [NSString stringWithFormat:@"Taking medication %@", _package.medicationname];
    }
    else
    {
        _lblMessage.text = @"";

    }
    
	// Do any additional setup after loading the view, typically from a nib.
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    
    CVLib::PackageInfo& packageInfo=appDelegate->m_packageInfo;
    /*  if (packageInfo.isValid())
     {
     // EXAMPLE: do something useful with the barcode data
     NSString* packageID=[[NSString alloc] initWithCString:packageInfo.szID encoding:NSUTF8StringEncoding];
     
     self.resultText.text=[NSString stringWithFormat:@"Package Baracode ID %@ was scanned.", packageID];
     //            [packageID release];
     }
     else{
     self.resultText.text=[NSString stringWithFormat:@"No Baracode scanned."];
     }*/
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (IBAction) scanButtonTapped
{
    
    
        //self.hidesBottomBarWhenPushed = NO;
        
        // ADD: present a barcode reader that scans from the camera feed
        ZBarReaderViewController *reader = [ZBarReaderViewController new];
        reader.readerDelegate = self;
        reader.supportedOrientationsMask = ZBarOrientationMaskAll;
        
        ZBarImageScanner *scanner = reader.scanner;
        // TODO: (optional) additional reader configuration here
        
        // EXAMPLE: disable rarely used I2/5 to improve performance
        [scanner setSymbology: ZBAR_I25
                       config: ZBAR_CFG_ENABLE
                           to: 0];
        
        // present and release the controller
        //[self presentModalViewController:reader animated:NO];
        [self presentViewController:reader animated:YES completion:^{
            
        }];
    
    
    
    
}
- (IBAction)btnCancel:(id)sender {
    
    [_parentController closeMe];
}

- (void) imagePickerController: (UIImagePickerController*) reader
 didFinishPickingMediaWithInfo: (NSDictionary*) info
{
    // ADD: get the decode results
    id<NSFastEnumeration> results =
    [info objectForKey: ZBarReaderControllerResults];
    ZBarSymbol *symbol = nil;
    for(symbol in results){
        // EXAMPLE: just grab the first barcode
        break;
    }
    
    
    //NSLog (@"%@", symbol.data);
    const char* szBuf=[symbol.data UTF8String];
    
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    
    CVLib::PackageInfo& packageInfo=appDelegate->m_packageInfo;
    packageInfo.decode(szBuf);
    // EXAMPLE: do something useful with the barcode data
    NSString* packageID=[[NSString alloc] initWithCString:packageInfo.szID encoding:NSUTF8StringEncoding];
    
    
    _resultText.text=[NSString stringWithFormat:@"Package Baracode ID %@ was scanned.", packageID];
    NSLog(@"%@", [NSString stringWithFormat:@"Package Baracode ID %@ was scanned. _package.packageid = %@", packageID, _package.barcode]);
    
    if(_package)
    {
        if([_package.barcode isEqual:packageID])
        {
            _parentController.keptBarCode = packageID;
            
            [[MEDNetworkHelper sharedInstance] postEvent:EVENTScanBarcode packageid:_package.packageid.integerValue completionBlock:^(BOOL result, NSError *error) {
                NSLog(@"EVENTScanBarcode sent");
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

            
            [self dismissViewControllerAnimated:YES completion:^{
                [_parentController toggleView: VMODE_INSTRUCTIONS nState:0];
                
            }];
        }
        else
        {
            [self dismissViewControllerAnimated:YES completion:^{
                _lblMessage.text = [NSString stringWithFormat:@"Wrong medications scanned! Please scan bar code for %@", _package.medicationname];

                [[MEDNetworkHelper sharedInstance] postEvent:EVENTDeviceAlarmWrongMedScanned packageid:_package.packageid.integerValue completionBlock:^(BOOL result, NSError *error) {
                    NSLog(@"EVENTDeviceAlarmWrongMedScanned sent");
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
            }];
        }
    }
    else
    {
        _parentController.keptBarCode = packageID;
        
        [self dismissViewControllerAnimated:YES completion:^{
            [self presentCaptureViewController];
            
        }];
    }
   


}

- (void)presentCaptureViewController
{
    
    ExtractPackAppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
    appDelegate.m_fCapturing = true;
	[_parentController toggleView: VMODE_CAMERA nState:0];
    
    
     
}


@end
