// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef KEY_MAPPING_X11_H
#define KEY_MAPPING_X11_H

#include <X11/XF86keysym.h>
#include <X11/Xlib.h>
#define XK_MISCELLANY
#define XK_LATIN1
#define XK_XKB_KEYS
#include "core/os/keyboard.h"

#include <X11/keysymdef.h>

class KeyMappingX11 {
    KeyMappingX11(){};

public:
    static unsigned int get_keycode(KeySym p_keysym);
    static unsigned int get_scancode(unsigned int p_code);
    static KeySym get_keysym(unsigned int p_code);
    static unsigned int get_unicode_from_keysym(KeySym p_keysym);
    static KeySym get_keysym_from_unicode(unsigned int p_unicode);
};

#endif
