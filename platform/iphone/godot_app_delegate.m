// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "godot_app_delegate.h"

#import "app_delegate.h"

@interface GodotApplicalitionDelegate ()

@end

@implementation GodotApplicalitionDelegate

static NSMutableArray<ApplicationDelegateService*>* services = nil;

+ (NSArray<ApplicationDelegateService*>*)services {
    return services;
}

+ (void)load {
    services = [NSMutableArray new];
    [services addObject:[AppDelegate new]];
}

+ (void)addService:(ApplicationDelegateService*)service {
    if (!services || !service) {
        return;
    }
    [services addObject:service];
}

// UIApplicationDelegate documentation can be found here:
// https://developer.apple.com/documentation/uikit/uiapplicationdelegate

// MARK: Window

- (UIWindow*)window {
    UIWindow* result = nil;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        UIWindow* value = [service window];

        if (value) {
            result = value;
        }
    }

    return result;
}

// MARK: Initializing

- (BOOL)application:(UIApplication*)application
    willFinishLaunchingWithOptions:
        (NSDictionary<UIApplicationLaunchOptionsKey, id>*)launchOptions {
    BOOL result = NO;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:application
                willFinishLaunchingWithOptions:launchOptions]) {
            result = YES;
        }
    }

    return result;
}

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:
        (NSDictionary<UIApplicationLaunchOptionsKey, id>*)launchOptions {
    BOOL result = NO;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:application
                didFinishLaunchingWithOptions:launchOptions]) {
            result = YES;
        }
    }

    return result;
}

/* Can be handled by Info.plist. Not yet supported by Godot.

// MARK: Scene

- (UISceneConfiguration *)application:(UIApplication *)application
configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession
options:(UISceneConnectionOptions *)options {}

- (void)application:(UIApplication *)application
didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions {}

*/

// MARK: Life-Cycle

- (void)applicationDidBecomeActive:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationDidBecomeActive:application];
    }
}

- (void)applicationWillResignActive:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationWillResignActive:application];
    }
}

- (void)applicationDidEnterBackground:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationDidEnterBackground:application];
    }
}

- (void)applicationWillEnterForeground:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationWillEnterForeground:application];
    }
}

- (void)applicationWillTerminate:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationWillTerminate:application];
    }
}

// MARK: Environment Changes

- (void)applicationProtectedDataDidBecomeAvailable:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationProtectedDataDidBecomeAvailable:application];
    }
}

- (void
)applicationProtectedDataWillBecomeUnavailable:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationProtectedDataWillBecomeUnavailable:application];
    }
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationDidReceiveMemoryWarning:application];
    }
}

- (void)applicationSignificantTimeChange:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationSignificantTimeChange:application];
    }
}

// MARK: App State Restoration

- (BOOL)application:(UIApplication*)application
    shouldSaveSecureApplicationState:(NSCoder*)coder API_AVAILABLE(ios(13.2)) {
    BOOL result = NO;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:application
                shouldSaveSecureApplicationState:coder]) {
            result = YES;
        }
    }

    return result;
}

- (BOOL)application:(UIApplication*)application
    shouldRestoreSecureApplicationState:(NSCoder*)coder
    API_AVAILABLE(ios(13.2)) {
    BOOL result = NO;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:application
                shouldRestoreSecureApplicationState:coder]) {
            result = YES;
        }
    }

    return result;
}

- (UIViewController*)application:(UIApplication*)application
    viewControllerWithRestorationIdentifierPath:
        (NSArray<NSString*>*)identifierComponents
                                          coder:(NSCoder*)coder {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        UIViewController* controller = [service application:application
                viewControllerWithRestorationIdentifierPath:identifierComponents
                                                      coder:coder];

        if (controller) {
            return controller;
        }
    }

    return nil;
}

- (void)application:(UIApplication*)application
    willEncodeRestorableStateWithCoder:(NSCoder*)coder {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application
            willEncodeRestorableStateWithCoder:coder];
    }
}

- (void)application:(UIApplication*)application
    didDecodeRestorableStateWithCoder:(NSCoder*)coder {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application
            didDecodeRestorableStateWithCoder:coder];
    }
}

// MARK: Download Data in Background

- (void)application:(UIApplication*)application
    handleEventsForBackgroundURLSession:(NSString*)identifier
                      completionHandler:(void (^)(void))completionHandler {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application
            handleEventsForBackgroundURLSession:identifier
                              completionHandler:completionHandler];
    }

    completionHandler();
}

// MARK: Remote Notification

// Moved to the iOS Plugin

// MARK: User Activity and Handling Quick Actions

- (BOOL)application:(UIApplication*)application
    willContinueUserActivityWithType:(NSString*)userActivityType {
    BOOL result = NO;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:application
                willContinueUserActivityWithType:userActivityType]) {
            result = YES;
        }
    }

    return result;
}

- (BOOL)application:(UIApplication*)application
    continueUserActivity:(NSUserActivity*)userActivity
      restorationHandler:
          (void (^)(NSArray<id<UIUserActivityRestoring>>* restorableObjects)
          )restorationHandler {
    BOOL result = NO;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:application
                continueUserActivity:userActivity
                  restorationHandler:restorationHandler]) {
            result = YES;
        }
    }

    return result;
}

- (void)application:(UIApplication*)application
    didUpdateUserActivity:(NSUserActivity*)userActivity {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application didUpdateUserActivity:userActivity];
    }
}

- (void)application:(UIApplication*)application
    didFailToContinueUserActivityWithType:(NSString*)userActivityType
                                    error:(NSError*)error {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application
            didFailToContinueUserActivityWithType:userActivityType
                                            error:error];
    }
}

- (void)application:(UIApplication*)application
    performActionForShortcutItem:(UIApplicationShortcutItem*)shortcutItem
               completionHandler:(void (^)(BOOL succeeded))completionHandler {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application
            performActionForShortcutItem:shortcutItem
                       completionHandler:completionHandler];
    }
}

// MARK: WatchKit

- (void)application:(UIApplication*)application
    handleWatchKitExtensionRequest:(NSDictionary*)userInfo
                             reply:(void (^)(NSDictionary* replyInfo))reply {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application
            handleWatchKitExtensionRequest:userInfo
                                     reply:reply];
    }
}

// MARK: HealthKit

- (void
)applicationShouldRequestHealthAuthorization:(UIApplication*)application {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service applicationShouldRequestHealthAuthorization:application];
    }
}

// MARK: Opening an URL

- (BOOL)application:(UIApplication*)app
            openURL:(NSURL*)url
            options:(NSDictionary<UIApplicationOpenURLOptionsKey, id>*)options {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:app openURL:url options:options]) {
            return YES;
        }
    }

    return NO;
}

// MARK: Disallowing Specified App Extension Types

- (BOOL)application:(UIApplication*)application
    shouldAllowExtensionPointIdentifier:(UIApplicationExtensionPointIdentifier
                                        )extensionPointIdentifier {
    BOOL result = NO;

    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        if ([service application:application
                shouldAllowExtensionPointIdentifier:extensionPointIdentifier]) {
            result = YES;
        }
    }

    return result;
}

// MARK: SiriKit

- (id)application:(UIApplication*)application
    handlerForIntent:(INIntent*)intent API_AVAILABLE(ios(14.0)) {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        id result = [service application:application handlerForIntent:intent];

        if (result) {
            return result;
        }
    }

    return nil;
}

// MARK: CloudKit

- (void)application:(UIApplication*)application
    userDidAcceptCloudKitShareWithMetadata:
        (CKShareMetadata*)cloudKitShareMetadata {
    for (ApplicationDelegateService* service in services) {
        if (![service respondsToSelector:_cmd]) {
            continue;
        }

        [service application:application
            userDidAcceptCloudKitShareWithMetadata:cloudKitShareMetadata];
    }
}

/* Handled By Info.plist file for now

// MARK: Interface Geometry

- (UIInterfaceOrientationMask)application:(UIApplication *)application
supportedInterfaceOrientationsForWindow:(UIWindow *)window {}

*/

@end
