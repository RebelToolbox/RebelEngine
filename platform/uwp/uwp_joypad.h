// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef UWP_JOYPAD_H
#define UWP_JOYPAD_H

#include "main/input_default.h"

ref class UwpJoypad sealed {
    /** clang-format breaks this, it does not understand this token. */
    /* clang-format off */
internal:
	void register_events();
	void process_controllers();
    /* clang-format on */

    UwpJoypad();
    UwpJoypad(InputDefault* p_input);

private:
    enum {
        MAX_CONTROLLERS = 4,
    };

    enum ControllerType {
        GAMEPAD_CONTROLLER,
        ARCADE_STICK_CONTROLLER,
        RACING_WHEEL_CONTROLLER,
    };

    struct ControllerDevice {
        Windows::Gaming::Input::IGameController ^ controller_reference;

        int id;
        bool connected;
        ControllerType type;
        float ff_timestamp;
        float ff_end_timestamp;
        bool vibrating;

        ControllerDevice() {
            id               = -1;
            connected        = false;
            type             = ControllerType::GAMEPAD_CONTROLLER;
            ff_timestamp     = 0.0f;
            ff_end_timestamp = 0.0f;
            vibrating        = false;
        }
    };

    ControllerDevice controllers[MAX_CONTROLLERS];

    InputDefault* input;

    void OnGamepadAdded(
        Platform::Object ^ sender,
        Windows::Gaming::Input::Gamepad ^ value
    );
    void OnGamepadRemoved(
        Platform::Object ^ sender,
        Windows::Gaming::Input::Gamepad ^ value
    );

    InputDefault::JoyAxis axis_correct(
        double p_val,
        bool p_negate  = false,
        bool p_trigger = false
    ) const;
    void joypad_vibration_start(
        int p_device,
        float p_weak_magnitude,
        float p_strong_magnitude,
        float p_duration,
        uint64_t p_timestamp
    );
    void joypad_vibration_stop(int p_device, uint64_t p_timestamp);
};

#endif // UWP_JOYPAD_H
