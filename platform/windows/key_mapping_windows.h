// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef KEY_MAPPING_WINDOWS_H
#define KEY_MAPPING_WINDOWS_H

#include "core/os/keyboard.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// Windows system includes come after <windows.h>
#include <winuser.h>

class KeyMappingWindows {
    KeyMappingWindows(){};

public:
    static unsigned int get_keysym(unsigned int p_code);
    static unsigned int get_scansym(unsigned int p_code, bool p_extended);
    static bool is_extended_key(unsigned int p_code);
};

#endif // KEY_MAPPING_WINDOWS_H
