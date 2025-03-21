// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "rebel_view_renderer.h"

#include "core/os/keyboard.h"
#include "core/project_settings.h"
#include "ios_os.h"
#include "main/main.h"
#include "servers/audio_server.h"

#import <AudioToolbox/AudioServices.h>
#import <CoreMotion/CoreMotion.h>
#import <GameController/GameController.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

@interface RebelViewRenderer ()

@property(assign, nonatomic) BOOL hasFinishedProjectDataSetup;
@property(assign, nonatomic) BOOL hasStartedMain;
@property(assign, nonatomic) BOOL hasFinishedSetup;

@end

@implementation RebelViewRenderer

- (BOOL)setupView:(UIView*)view {
    if (self.hasFinishedSetup) {
        return NO;
    }

    if (!OS::get_singleton()) {
        exit(0);
    }

    if (!self.hasFinishedProjectDataSetup) {
        [self setupProjectData];
        return YES;
    }

    if (!self.hasStartedMain) {
        self.hasStartedMain = YES;
        IosOS::get_singleton()->start();
        return YES;
    }

    self.hasFinishedSetup = YES;

    return NO;
}

- (void)setupProjectData {
    self.hasFinishedProjectDataSetup = YES;

    Main::setup2();

    // this might be necessary before here
    NSDictionary* dict = [[NSBundle mainBundle] infoDictionary];
    for (NSString* key in dict) {
        NSObject* value = [dict objectForKey:key];
        String ukey     = String::utf8([key UTF8String]);

        // we need a NSObject to Variant conversor

        if ([value isKindOfClass:[NSString class]]) {
            NSString* str = (NSString*)value;
            String uval   = String::utf8([str UTF8String]);

            ProjectSettings::get_singleton()->set("Info.plist/" + ukey, uval);

        } else if ([value isKindOfClass:[NSNumber class]]) {
            NSNumber* n = (NSNumber*)value;
            double dval = [n doubleValue];

            ProjectSettings::get_singleton()->set("Info.plist/" + ukey, dval);
        };
        // do stuff
    }
}

- (void)renderOnView:(UIView*)view {
    if (!IosOS::get_singleton()) {
        return;
    }

    IosOS::get_singleton()->iterate();
}

@end
