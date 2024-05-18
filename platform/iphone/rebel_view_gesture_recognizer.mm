// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "rebel_view_gesture_recognizer.h"

#include "core/project_settings.h"
#import "rebel_view.h"

// Minimum distance for touches to move to fire
// a delay timer before scheduled time.
// Should be the low enough to not cause issues with dragging
// but big enough to allow click to work.
const CGFloat kGLGestureMovementDistance = 0.5;

@interface RebelViewGestureRecognizer ()

@property(nonatomic, readwrite, assign) NSTimeInterval delayTimeInterval;

@end

@implementation RebelViewGestureRecognizer

- (RebelView*)rebelView {
    return (RebelView*)self.view;
}

- (instancetype)init {
    self = [super init];

    self.cancelsTouchesInView       = YES;
    self.delaysTouchesBegan         = YES;
    self.delaysTouchesEnded         = YES;
    self.requiresExclusiveTouchType = NO;

    self.delayTimeInterval =
        GLOBAL_GET("input_devices/pointing/ios/touch_delay");

    return self;
}

- (void)delayTouches:(NSSet*)touches andEvent:(UIEvent*)event {
    [delayTimer fire];

    delayedTouches = touches;
    delayedEvent   = event;

    delayTimer =
        [NSTimer scheduledTimerWithTimeInterval:self.delayTimeInterval
                                         target:self
                                       selector:@selector(fireDelayedTouches:)
                                       userInfo:nil
                                        repeats:NO];
}

- (void)fireDelayedTouches:(id)timer {
    [delayTimer invalidate];
    delayTimer = nil;

    if (delayedTouches) {
        [self.rebelView rebelTouchesBegan:delayedTouches
                                withEvent:delayedEvent];
    }

    delayedTouches = nil;
    delayedEvent   = nil;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
    NSSet* cleared = [self copyClearedTouches:touches phase:UITouchPhaseBegan];
    [self delayTouches:cleared andEvent:event];

    [super touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {
    NSSet* cleared = [self copyClearedTouches:touches phase:UITouchPhaseMoved];

    if (delayTimer) {
        // We should check if movement was significant enough to fire an event
        // for dragging to work correctly.
        for (UITouch* touch in cleared) {
            CGPoint from      = [touch locationInView:self.rebelView];
            CGPoint to        = [touch previousLocationInView:self.rebelView];
            CGFloat xDistance = from.x - to.x;
            CGFloat yDistance = from.y - to.y;

            CGFloat distance =
                sqrt(xDistance * xDistance + yDistance * yDistance);

            // Early exit, since one of touches has moved enough to fire a drag
            // event.
            if (distance > kGLGestureMovementDistance) {
                [delayTimer fire];
                [self.rebelView rebelTouchesMoved:cleared withEvent:event];
                return;
            }
        }
        return;
    }

    [self.rebelView rebelTouchesMoved:cleared withEvent:event];

    [super touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
    [delayTimer fire];

    NSSet* cleared = [self copyClearedTouches:touches phase:UITouchPhaseEnded];
    [self.rebelView rebelTouchesEnded:cleared withEvent:event];

    [super touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event {
    [delayTimer fire];
    [self.rebelView rebelTouchesCancelled:touches withEvent:event];

    [super touchesCancelled:touches withEvent:event];
}

- (NSSet*)copyClearedTouches:(NSSet*)touches phase:(UITouchPhase)phaseToSave {
    NSMutableSet* cleared = [touches mutableCopy];

    for (UITouch* touch in touches) {
        if (touch.view != self.view || touch.phase != phaseToSave) {
            [cleared removeObject:touch];
        }
    }

    return cleared;
}

@end
