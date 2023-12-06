// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <GameController/GameController.h>

@interface JoypadIPhoneObserver : NSObject

- (void)startObserving;
- (void)startProcessing;
- (void)finishObserving;

@end

class JoypadIPhone {
private:
    JoypadIPhoneObserver* observer;

public:
    JoypadIPhone();
    ~JoypadIPhone();

    void start_processing();
};
