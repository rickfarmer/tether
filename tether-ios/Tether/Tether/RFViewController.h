//
//  RFViewController.h
//  Tether
//
//  Created by farmer on 12/11/13.
//  Copyright (c) 2013 Rick Farmer. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RFViewController : UIViewController {
    
    IBOutlet UILabel *label;
    IBOutlet UISwitch *switchOutlet;
}

- (IBAction)switch:(id)sender;

@end
