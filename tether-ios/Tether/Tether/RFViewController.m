//
//  RFViewController.m
//  Tether
//
//  Created by farmer on 12/11/13.
//  Copyright (c) 2013 Rick Farmer. All rights reserved.
//

#import "RFViewController.h"

@interface RFViewController ()

@end

@implementation RFViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)switch:(id)sender {
    
    int isProxyOn = 0;
    
    if(switchOutlet.on) {
        isProxyOn = proxy(0);
        label.text = @"Proxy Stopped";
    }else{
        isProxyOn = proxy(1);
        label.text = @"Proxy Started";
        
    }
}
@end
