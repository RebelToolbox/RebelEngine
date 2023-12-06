// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

// GodotViewGestureRecognizer allows iOS gestures to work currectly by
// emulating UIScrollView's UIScrollViewDelayedTouchesBeganGestureRecognizer.
// It catches all gestures incoming to UIView and delays them for 150ms
// (the same value used by UIScrollViewDelayedTouchesBeganGestureRecognizer)
// If touch cancellation or end message is fired it fires delayed
// begin touch immediately as well as last touch signal

#import <UIKit/UIKit.h>

@interface GodotViewGestureRecognizer : UIGestureRecognizer {
@private

    // Timer used to delay begin touch message.
    // Should work as simple emulation of UIDelayedAction
    NSTimer* delayTimer;

    // Delayed touch parameters
    NSSet* delayedTouches;
    UIEvent* delayedEvent;
}

@property(nonatomic, readonly, assign) NSTimeInterval delayTimeInterval;

- (instancetype)init;

@end
