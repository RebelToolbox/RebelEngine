// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "uwp_joypad.h"

#include "core/os/os.h"

using namespace Windows::Gaming::Input;
using namespace Windows::Foundation;

void UwpJoypad::register_events() {
    Gamepad::GamepadAdded +=
        ref new EventHandler<Gamepad ^>(this, &UwpJoypad::OnGamepadAdded);
    Gamepad::GamepadRemoved +=
        ref new EventHandler<Gamepad ^>(this, &UwpJoypad::OnGamepadRemoved);
}

void UwpJoypad::process_controllers() {
    for (int i = 0; i < MAX_CONTROLLERS; i++) {
        ControllerDevice& joy = controllers[i];

        if (!joy.connected) {
            break;
        }

        switch (joy.type) {
            case ControllerType::GAMEPAD_CONTROLLER: {
                GamepadReading reading =
                    ((Gamepad ^) joy.controller_reference)->GetCurrentReading();

                int button_mask = (int)GamepadButtons::Menu;
                for (int j = 0; j < 14; j++) {
                    input->joy_button(
                        joy.id,
                        j,
                        (int)reading.Buttons & button_mask
                    );
                    button_mask *= 2;
                }

                input->joy_axis(
                    joy.id,
                    JOY_AXIS_0,
                    axis_correct(reading.LeftThumbstickX)
                );
                input->joy_axis(
                    joy.id,
                    JOY_AXIS_1,
                    axis_correct(reading.LeftThumbstickY, true)
                );
                input->joy_axis(
                    joy.id,
                    JOY_AXIS_2,
                    axis_correct(reading.RightThumbstickX)
                );
                input->joy_axis(
                    joy.id,
                    JOY_AXIS_3,
                    axis_correct(reading.RightThumbstickY, true)
                );
                input->joy_axis(
                    joy.id,
                    JOY_AXIS_4,
                    axis_correct(reading.LeftTrigger, false, true)
                );
                input->joy_axis(
                    joy.id,
                    JOY_AXIS_5,
                    axis_correct(reading.RightTrigger, false, true)
                );

                uint64_t timestamp = input->get_joy_vibration_timestamp(joy.id);
                if (timestamp > joy.ff_timestamp) {
                    Vector2 strength =
                        input->get_joy_vibration_strength(joy.id);
                    float duration = input->get_joy_vibration_duration(joy.id);
                    if (strength.x == 0 && strength.y == 0) {
                        joypad_vibration_stop(i, timestamp);
                    } else {
                        joypad_vibration_start(
                            i,
                            strength.x,
                            strength.y,
                            duration,
                            timestamp
                        );
                    }
                } else if (joy.vibrating && joy.ff_end_timestamp != 0) {
                    uint64_t current_time =
                        OS::get_singleton()->get_ticks_usec();
                    if (current_time >= joy.ff_end_timestamp) {
                        joypad_vibration_stop(i, current_time);
                    }
                }

                break;
            }
        }
    }
}

UwpJoypad::UwpJoypad() {
    for (int i = 0; i < MAX_CONTROLLERS; i++) {
        controllers[i].id = i;
    }
}

UwpJoypad::UwpJoypad(InputDefault* p_input) {
    input = p_input;

    UwpJoypad();
}

void UwpJoypad::OnGamepadAdded(
    Platform::Object ^ sender,
    Windows::Gaming::Input::Gamepad ^ value
) {
    short idx = -1;

    for (int i = 0; i < MAX_CONTROLLERS; i++) {
        if (!controllers[i].connected) {
            idx = i;
            break;
        }
    }

    ERR_FAIL_COND(idx == -1);

    controllers[idx].connected            = true;
    controllers[idx].controller_reference = value;
    controllers[idx].id                   = idx;
    controllers[idx].type                 = ControllerType::GAMEPAD_CONTROLLER;

    input->joy_connection_changed(
        controllers[idx].id,
        true,
        "Xbox Controller",
        "__UWP_GAMEPAD__"
    );
}

void UwpJoypad::OnGamepadRemoved(
    Platform::Object ^ sender,
    Windows::Gaming::Input::Gamepad ^ value
) {
    short idx = -1;

    for (int i = 0; i < MAX_CONTROLLERS; i++) {
        if (controllers[i].controller_reference == value) {
            idx = i;
            break;
        }
    }

    ERR_FAIL_COND(idx == -1);

    controllers[idx] = ControllerDevice();

    input->joy_connection_changed(idx, false, "Xbox Controller");
}

InputDefault::JoyAxis UwpJoypad::axis_correct(
    double p_val,
    bool p_negate,
    bool p_trigger
) const {
    InputDefault::JoyAxis jx;

    jx.min   = p_trigger ? 0 : -1;
    jx.value = (float)(p_negate ? -p_val : p_val);

    return jx;
}

void UwpJoypad::joypad_vibration_start(
    int p_device,
    float p_weak_magnitude,
    float p_strong_magnitude,
    float p_duration,
    uint64_t p_timestamp
) {
    ControllerDevice& joy = controllers[p_device];
    if (joy.connected) {
        GamepadVibration vibration;
        vibration.LeftMotor                               = p_strong_magnitude;
        vibration.RightMotor                              = p_weak_magnitude;
        ((Gamepad ^) joy.controller_reference)->Vibration = vibration;

        joy.ff_timestamp = p_timestamp;
        joy.ff_end_timestamp =
            p_duration == 0 ? 0
                            : p_timestamp + (uint64_t)(p_duration * 1000000.0);
        joy.vibrating = true;
    }
}

void UwpJoypad::joypad_vibration_stop(int p_device, uint64_t p_timestamp) {
    ControllerDevice& joy = controllers[p_device];
    if (joy.connected) {
        GamepadVibration vibration;
        vibration.LeftMotor                               = 0.0;
        vibration.RightMotor                              = 0.0;
        ((Gamepad ^) joy.controller_reference)->Vibration = vibration;

        joy.ff_timestamp = p_timestamp;
        joy.vibrating    = false;
    }
}
