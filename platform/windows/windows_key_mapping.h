// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WINDOWS_KEY_MAPPING_H
#define WINDOWS_KEY_MAPPING_H

#include "core/os/keyboard.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// Windows system includes come after <windows.h>
#include <winuser.h>

class WindowsKeyMapping {
    WindowsKeyMapping(){};

public:
    static unsigned int get_keysym(unsigned int p_code);
    static unsigned int get_scansym(unsigned int p_code, bool p_extended);
    static bool is_extended_key(unsigned int p_code);
};

#endif // WINDOWS_KEY_MAPPING_H
