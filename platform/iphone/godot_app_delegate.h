// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <UIKit/UIKit.h>

typedef NSObject<UIApplicationDelegate> ApplicationDelegateService;

@interface GodotApplicalitionDelegate : NSObject <UIApplicationDelegate>

@property(class, readonly, strong)
    NSArray<ApplicationDelegateService*>* services;

+ (void)addService:(ApplicationDelegateService*)service;

@end
