//
//  AppDelegate.h
//  AEngineTest
//
//  Created by Lucas Ellis on 12/25/12.
//  Copyright (c) 2012 Lucas Ellis. All rights reserved.
//

#import <GLKit/GLKit.h>
#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate,GLKViewControllerDelegate,GLKViewDelegate>
{
    double _curRed;
    int    _increasing;
}

@property (strong, nonatomic) UIWindow *window;

@end
