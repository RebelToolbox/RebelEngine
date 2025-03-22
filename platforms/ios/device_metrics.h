// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <Foundation/Foundation.h>

@interface RebelDeviceMetrics : NSObject

@property(nonatomic, class, readonly, strong)
    NSDictionary<NSArray*, NSNumber*>* dpiList;

@end
