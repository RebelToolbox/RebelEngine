// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "ios_joypad.h"

#include "core/project_settings.h"
#include "drivers/coreaudio/audio_driver_coreaudio.h"
#include "ios_os.h"
#include "main/main.h"
#import "rebel_view.h"

IosJoypad::IosJoypad() {
    observer = [[IosJoypadObserver alloc] init];
    [observer startObserving];
}

IosJoypad::~IosJoypad() {
    if (observer) {
        [observer finishObserving];
        observer = nil;
    }
}

void IosJoypad::start_processing() {
    if (observer) {
        [observer startProcessing];
    }
}

@interface IosJoypadObserver ()

@property(assign, nonatomic) BOOL isObserving;
@property(assign, nonatomic) BOOL isProcessing;
@property(strong, nonatomic) NSMutableDictionary* connectedJoypads;
@property(strong, nonatomic) NSMutableArray* joypadsQueue;

@end

@implementation IosJoypadObserver

- (instancetype)init {
    self = [super init];

    if (self) {
        [self rebel_commonInit];
    }

    return self;
}

- (void)rebel_commonInit {
    self.isObserving  = NO;
    self.isProcessing = NO;
}

- (void)startProcessing {
    self.isProcessing = YES;

    for (GCController* controller in self.joypadsQueue) {
        [self addiOSJoypad:controller];
    }

    [self.joypadsQueue removeAllObjects];
}

- (void)startObserving {
    if (self.isObserving) {
        return;
    }

    self.isObserving = YES;

    self.connectedJoypads = [NSMutableDictionary dictionary];
    self.joypadsQueue     = [NSMutableArray array];

    // get told when controllers connect, this will be called right away for
    // already connected controllers
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(controllerWasConnected:)
               name:GCControllerDidConnectNotification
             object:nil];

    // get told when controllers disconnect
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(controllerWasDisconnected:)
               name:GCControllerDidDisconnectNotification
             object:nil];
}

- (void)finishObserving {
    if (self.isObserving) {
        [[NSNotificationCenter defaultCenter] removeObserver:self];
    }

    self.isObserving  = NO;
    self.isProcessing = NO;

    self.connectedJoypads = nil;
    self.joypadsQueue     = nil;
}

- (void)dealloc {
    [self finishObserving];
}

- (int)getJoyIdForController:(GCController*)controller {
    NSArray* keys = [self.connectedJoypads allKeysForObject:controller];

    for (NSNumber* key in keys) {
        int joy_id = [key intValue];
        return joy_id;
    };

    return -1;
};

- (void)addiOSJoypad:(GCController*)controller {
    // get a new id for our controller
    int joy_id = IosOS::get_singleton()->get_unused_joy_id();

    if (joy_id == -1) {
        printf("Couldn't retrieve new joy id\n");
        return;
    }

    // assign our player index
    if (controller.playerIndex == GCControllerPlayerIndexUnset) {
        controller.playerIndex = [self getFreePlayerIndex];
    };

    // Tell Rebel Engine about our new controller.
    IosOS::get_singleton()->joy_connection_changed(
        joy_id,
        true,
        String::utf8([controller.vendorName UTF8String])
    );

    // add it to our dictionary, this will retain our controllers
    [self.connectedJoypads setObject:controller
                              forKey:[NSNumber numberWithInt:joy_id]];

    // set our input handler
    [self setControllerInputHandler:controller];
}

- (void)controllerWasConnected:(NSNotification*)notification {
    // get our controller
    GCController* controller = (GCController*)notification.object;

    if (!controller) {
        printf("Couldn't retrieve new controller\n");
        return;
    }

    if ([[self.connectedJoypads allKeysForObject:controller] count] > 0) {
        printf("Controller is already registered\n");
    } else if (!self.isProcessing) {
        [self.joypadsQueue addObject:controller];
    } else {
        [self addiOSJoypad:controller];
    }
}

- (void)controllerWasDisconnected:(NSNotification*)notification {
    // find our joystick, there should be only one in our dictionary
    GCController* controller = (GCController*)notification.object;

    if (!controller) {
        return;
    }

    NSArray* keys = [self.connectedJoypads allKeysForObject:controller];
    for (NSNumber* key in keys) {
        // Tell Rebel Engine this joystick is no longer there.
        int joy_id = [key intValue];
        IosOS::get_singleton()->joy_connection_changed(joy_id, false, "");

        // and remove it from our dictionary
        [self.connectedJoypads removeObjectForKey:key];
    };
};

- (GCControllerPlayerIndex)getFreePlayerIndex {
    bool have_player_1 = false;
    bool have_player_2 = false;
    bool have_player_3 = false;
    bool have_player_4 = false;

    if (self.connectedJoypads == nil) {
        NSArray* keys = [self.connectedJoypads allKeys];
        for (NSNumber* key in keys) {
            GCController* controller = [self.connectedJoypads objectForKey:key];
            if (controller.playerIndex == GCControllerPlayerIndex1) {
                have_player_1 = true;
            } else if (controller.playerIndex == GCControllerPlayerIndex2) {
                have_player_2 = true;
            } else if (controller.playerIndex == GCControllerPlayerIndex3) {
                have_player_3 = true;
            } else if (controller.playerIndex == GCControllerPlayerIndex4) {
                have_player_4 = true;
            };
        };
    };

    if (!have_player_1) {
        return GCControllerPlayerIndex1;
    } else if (!have_player_2) {
        return GCControllerPlayerIndex2;
    } else if (!have_player_3) {
        return GCControllerPlayerIndex3;
    } else if (!have_player_4) {
        return GCControllerPlayerIndex4;
    } else {
        return GCControllerPlayerIndexUnset;
    };
}

- (void)setControllerInputHandler:(GCController*)controller {
    // Hook in the callback handler for the correct gamepad profile.
    // This is a bit of a weird design choice on Apples part.
    // You need to select the most capable gamepad profile for the
    // gamepad attached.
    if (controller.extendedGamepad != nil) {
        // The extended gamepad profile has all the input you could possibly
        // find on a gamepad but will only be active if your gamepad actually
        // has all of these...
        _weakify(self);
        _weakify(controller);

        controller.extendedGamepad.valueChangedHandler =
            ^(GCExtendedGamepad* gamepad, GCControllerElement* element) {
                _strongify(self);
                _strongify(controller);

                int joy_id = [self getJoyIdForController:controller];

                if (element == gamepad.buttonA) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_BUTTON_0,
                        gamepad.buttonA.isPressed
                    );
                } else if (element == gamepad.buttonB) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_BUTTON_1,
                        gamepad.buttonB.isPressed
                    );
                } else if (element == gamepad.buttonX) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_BUTTON_2,
                        gamepad.buttonX.isPressed
                    );
                } else if (element == gamepad.buttonY) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_BUTTON_3,
                        gamepad.buttonY.isPressed
                    );
                } else if (element == gamepad.leftShoulder) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_L,
                        gamepad.leftShoulder.isPressed
                    );
                } else if (element == gamepad.rightShoulder) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_R,
                        gamepad.rightShoulder.isPressed
                    );
                } else if (element == gamepad.leftTrigger) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_L2,
                        gamepad.leftTrigger.isPressed
                    );
                } else if (element == gamepad.rightTrigger) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_R2,
                        gamepad.rightTrigger.isPressed
                    );
                } else if (element == gamepad.dpad) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_UP,
                        gamepad.dpad.up.isPressed
                    );
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_DOWN,
                        gamepad.dpad.down.isPressed
                    );
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_LEFT,
                        gamepad.dpad.left.isPressed
                    );
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_RIGHT,
                        gamepad.dpad.right.isPressed
                    );
                };

                InputDefault::JoyAxis jx;
                jx.min = -1;
                if (element == gamepad.leftThumbstick) {
                    jx.value = gamepad.leftThumbstick.xAxis.value;
                    IosOS::get_singleton()->joy_axis(joy_id, JOY_ANALOG_LX, jx);
                    jx.value = -gamepad.leftThumbstick.yAxis.value;
                    IosOS::get_singleton()->joy_axis(joy_id, JOY_ANALOG_LY, jx);
                } else if (element == gamepad.rightThumbstick) {
                    jx.value = gamepad.rightThumbstick.xAxis.value;
                    IosOS::get_singleton()->joy_axis(joy_id, JOY_ANALOG_RX, jx);
                    jx.value = -gamepad.rightThumbstick.yAxis.value;
                    IosOS::get_singleton()->joy_axis(joy_id, JOY_ANALOG_RY, jx);
                } else if (element == gamepad.leftTrigger) {
                    jx.value = gamepad.leftTrigger.value;
                    IosOS::get_singleton()->joy_axis(joy_id, JOY_ANALOG_L2, jx);
                } else if (element == gamepad.rightTrigger) {
                    jx.value = gamepad.rightTrigger.value;
                    IosOS::get_singleton()->joy_axis(joy_id, JOY_ANALOG_R2, jx);
                }
            };
    } else if (controller.microGamepad != nil) {
        // micro gamepads were added in OS 9 and feature just 2 buttons and a
        // d-pad
        _weakify(self);
        _weakify(controller);

        controller.microGamepad.valueChangedHandler =
            ^(GCMicroGamepad* gamepad, GCControllerElement* element) {
                _strongify(self);
                _strongify(controller);

                int joy_id = [self getJoyIdForController:controller];

                if (element == gamepad.buttonA) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_BUTTON_0,
                        gamepad.buttonA.isPressed
                    );
                } else if (element == gamepad.buttonX) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_BUTTON_2,
                        gamepad.buttonX.isPressed
                    );
                } else if (element == gamepad.dpad) {
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_UP,
                        gamepad.dpad.up.isPressed
                    );
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_DOWN,
                        gamepad.dpad.down.isPressed
                    );
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_LEFT,
                        gamepad.dpad.left.isPressed
                    );
                    IosOS::get_singleton()->joy_button(
                        joy_id,
                        JOY_DPAD_RIGHT,
                        gamepad.dpad.right.isPressed
                    );
                }
            };
    }

    ///@TODO need to add support for controller.motion which gives us access to
    /// the orientation of the device (if supported)

    ///@TODO need to add support for controllerPausedHandler which should be a
    /// toggle
};

@end
