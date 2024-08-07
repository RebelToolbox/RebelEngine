// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "app_delegate.h"

#include "core/project_settings.h"
#include "drivers/coreaudio/audio_driver_coreaudio.h"
#include "ios_os.h"
#include "main/main.h"
#import "rebel_view.h"
#import "view_controller.h"

#import <AudioToolbox/AudioServices.h>

#define kRenderingFrequency 60

extern int gargc;
extern char** gargv;

extern int ios_main(int, char**, String);
extern void ios_finish();

@implementation AppDelegate

static ViewController* mainViewController = nil;

+ (ViewController*)viewController {
    return mainViewController;
}

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    // Create a full-screen window
    CGRect windowBounds = [[UIScreen mainScreen] bounds];
    self.window         = [[UIWindow alloc] initWithFrame:windowBounds];

    NSArray* paths = NSSearchPathForDirectoriesInDomains(
        NSDocumentDirectory,
        NSUserDomainMask,
        YES
    );
    NSString* documentsDirectory = [paths objectAtIndex:0];

    int err =
        ios_main(gargc, gargv, String::utf8([documentsDirectory UTF8String]));
    if (err != 0) {
        // bail, things did not go very well for us, should probably output a
        // message on screen with our error code...
        exit(0);
        return FALSE;
    }

    // WARNING: We must *always* create the RebelView after we have constructed
    // the OS with ios_main. This allows the RebelView to access project
    // settings so it can properly initialize the OpenGL context

    ViewController* viewController = [[ViewController alloc] init];
    viewController.rebelView.useCADisplayLink =
        bool(GLOBAL_DEF("display.iOS/use_cadisplaylink", true)) ? YES : NO;
    viewController.rebelView.renderingInterval = 1.0 / kRenderingFrequency;

    self.window.rootViewController = viewController;

    // Show the window
    [self.window makeKeyAndVisible];

    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onAudioInterruption:)
               name:AVAudioSessionInterruptionNotification
             object:[AVAudioSession sharedInstance]];

    mainViewController = viewController;

    // prevent to stop music in another background app
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryAmbient
                                           error:nil];

    bool keep_screen_on =
        bool(GLOBAL_DEF("display/window/energy_saving/keep_screen_on", true));
    IosOS::get_singleton()->set_keep_screen_on(keep_screen_on);

    return TRUE;
}

- (void)onAudioInterruption:(NSNotification*)notification {
    if ([notification.name
            isEqualToString:AVAudioSessionInterruptionNotification]) {
        if ([[notification.userInfo
                valueForKey:AVAudioSessionInterruptionTypeKey]
                isEqualToNumber:
                    [NSNumber
                        numberWithInt:AVAudioSessionInterruptionTypeBegan]]) {
            NSLog(@"Audio interruption began");
            IosOS::get_singleton()->on_focus_out();
        } else if ([[notification.userInfo
                       valueForKey:AVAudioSessionInterruptionTypeKey]
                       isEqualToNumber:
                           [NSNumber
                               numberWithInt:
                                   AVAudioSessionInterruptionTypeEnded]]) {
            NSLog(@"Audio interruption ended");
            IosOS::get_singleton()->on_focus_in();
        }
    }
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication*)application {
    if (OS::get_singleton()->get_main_loop()) {
        OS::get_singleton()->get_main_loop()->notification(
            MainLoop::NOTIFICATION_OS_MEMORY_WARNING
        );
    }
}

- (void)applicationWillTerminate:(UIApplication*)application {
    ios_finish();
}

// When application goes to background (e.g. user switches to another app or
// presses Home), then applicationWillResignActive ->
// applicationDidEnterBackground are called. When user opens the inactive app
// again, applicationWillEnterForeground -> applicationDidBecomeActive are
// called.

// There are cases when applicationWillResignActive ->
// applicationDidBecomeActive sequence is called without the app going to
// background. For example, that happens if you open the app list without
// switching to another app or open/close the notification panel by swiping from
// the upper part of the screen.

- (void)applicationWillResignActive:(UIApplication*)application {
    IosOS::get_singleton()->on_focus_out();
}

- (void)applicationDidBecomeActive:(UIApplication*)application {
    IosOS::get_singleton()->on_focus_in();
}

- (void)dealloc {
    self.window = nil;
}

@end
