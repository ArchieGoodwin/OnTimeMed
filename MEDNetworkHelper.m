//
//  MEDNetworkHelper.m
//  ExtractPack
//
//  Created by Nero Wolfe on 6/7/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#import "MEDNetworkHelper.h"
#import "AFNetworking.h"
#import "User.h"
#import "NSManagedObject+NWCoreDataHelper.h"
#import "OTMhelper.h"
#import "History.h"
@implementation MEDNetworkHelper


-(void)loginUser:(NSString *)login password:(NSString *)password completionBlock:(RCCompleteBlockWithDictResult)completionBlock
{
    RCCompleteBlockWithDictResult completeBlock = completionBlock;

    NSString *urlString = [NSString stringWithFormat:@"http://ontimemed.incoding.biz/Account/SignIn?Login=%@&Password=%@", login, password];
    NSURL *url = [NSURL URLWithString:urlString];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSLog(@"[loginUser responseData]: %@", [[NSString alloc] initWithData:responseObject encoding:NSUTF8StringEncoding]);
        NSDictionary *rO = [NSJSONSerialization JSONObjectWithData:responseObject options:kNilOptions error:nil];
        
        completeBlock([rO objectForKey:@"data"], nil);
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"error loginUser %@", [error description]);
        
        completeBlock(nil, error);
    }];
    
    [operation start];
    
    
    
}


-(void)getPackagesByUserId:(NSString *)userId completionBlock:(RCCompleteBlockWithArrayResult)completionBlock
{
    //http://ontimemed.incoding.biz/MedPackage/FetchByUser?userId=10000
    RCCompleteBlockWithArrayResult completeBlock = completionBlock;
    
    
    NSString *urlString = [NSString stringWithFormat:@"http://ontimemed.incoding.biz/MedPackage/FetchByUser?userId=%@", userId];
    
    NSURL *url = [NSURL URLWithString:urlString];
    NSLog(@"getPackagesByUserId url : %@", urlString);
    
    
    
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *rO = [NSJSONSerialization JSONObjectWithData:responseObject options:kNilOptions error:nil];
        
        NSLog(@"getPackagesByUserId %@", rO);
        if(completeBlock)
        {
            completeBlock([rO objectForKey:@"data"], nil);
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"error in getPackagesByUserId  %@", [error description]);
        NSLog(@"error: %@", [error description]);
        if(completeBlock)
        {
            completeBlock(nil, error);
        }
    }];
    
    [operation start];
}

-(void)getMedPackageByBarcodeId:(NSString *)barcodeId completionBlock:(RCCompleteBlockWithDictResult)completionBlock
{
    RCCompleteBlockWithDictResult completeBlock = completionBlock;
    
    //barcodeId = @"1234567";
    
    User *user = [[OTMhelper sharedInstance] getCurrentUser];
    
    NSString *urlString = [NSString stringWithFormat:@"http://ontimemed.incoding.biz/MedPackage/FetchByBarcode?Barcode=%@&UserId=%@", barcodeId, user.userId];

    NSURL *url = [NSURL URLWithString:urlString];
    NSLog(@"getMedPackageByBarcodeId url : %@", urlString);
    
    
    
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *rO = [NSJSONSerialization JSONObjectWithData:responseObject options:kNilOptions error:nil];

        NSLog(@"%@", rO);
        if([rO objectForKey:@"data"] != [NSNull null])
        {
            if(completeBlock)
            {
                completeBlock([rO objectForKey:@"data"], nil);
            }
        }
        else
        {
            NSMutableDictionary* details = [NSMutableDictionary dictionary];
            [details setValue:@"There is no package with this code" forKey:NSLocalizedDescriptionKey];
            // populate the error object with the details
            NSError *err = [NSError errorWithDomain:@"world" code:500 userInfo:details];
            completeBlock(nil, err);
        }
       
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"error in getMedPackageByBarcodeId  %@", [error description]);
        NSLog(@"error: %@", [error description]);
        if(completeBlock)
        {
            completeBlock(nil, error);
        }
    }];
    
    [operation start];
}


-(void)postEvent:(EventOfType)eventType packageid:(NSInteger)packageid completionBlock:(RCCompleteBlockWithBOOLResult)completionBlock
{
    
    NSString *urlString = [NSString stringWithFormat:@"http://ontimemed.incoding.biz/EventLog/"];
    
    NSURL *url = [NSURL URLWithString:urlString];
    NSLog(@"get postEvent url : %@", urlString);
    
    AFHTTPClient *client = [[AFHTTPClient alloc] initWithBaseURL:url];
    [client setParameterEncoding:AFFormURLParameterEncoding];

    NSDate *inputDate = [NSDate date];
    //
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"MM/dd/yyyy hh:mm:ssa"];
    NSString *date = [formatter stringFromDate:inputDate];
    
    NSDictionary *arr = @{@"DeviceId":[[[UIDevice currentDevice] identifierForVendor] UUIDString],@"DeviceDate":date, @"Type":[NSNumber numberWithInteger:eventType], @"PackageId":[NSNumber numberWithInteger:packageid],@"EventXml":@""};
    NSLog(@"%@", arr);
    
    [client setDefaultHeader:@"X-Requested-With" value:@"XMLHttpRequest"];
    [client postPath:@"Log" parameters:arr success:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        NSLog(@"postEvent %@", [[NSString alloc] initWithData:responseObject encoding:NSUTF8StringEncoding]);
        
        History *historyEvent = [History createEntityInContext];
        historyEvent.packageId = [NSNumber numberWithInt:packageid];
        historyEvent.date = [NSDate date];
        historyEvent.event = [NSNumber numberWithInt:eventType];
        historyEvent.userId = [[OTMhelper sharedInstance] getCurrentUser].userId;
        [History saveDefaultContext];
        
        completionBlock(YES, nil);

    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"error postEvent %@", [error description]);
        
        completionBlock(NO, error);
    }];
    
}


- (id)init {
    self = [super init];
    

    
#if !(TARGET_IPHONE_SIMULATOR)
    
    
#else
    
    
#endif
    
    return self;
    
}



+(id)sharedInstance
{
    static dispatch_once_t pred;
    static MEDNetworkHelper *sharedInstance = nil;
    dispatch_once(&pred, ^{
        sharedInstance = [[MEDNetworkHelper alloc] init];
    });
    return sharedInstance;
}

- (void)dealloc
{
    
    abort();
}

@end
