// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_keys_utils.h"

unsigned int android_get_keysym(unsigned int p_code) {
    for (int i = 0; _ak_to_keycode[i].keysym != KEY_UNKNOWN; i++) {
        if (_ak_to_keycode[i].keycode == p_code) {
            return _ak_to_keycode[i].keysym;
        }
    }

    return KEY_UNKNOWN;
}
