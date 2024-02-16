// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "core/os/keyboard.h"

// See https://w3c.github.io/uievents-code/#code-value-tables
int dom_code2rebel_scancode(
    EM_UTF8 const p_code[32],
    EM_UTF8 const p_key[32],
    bool p_physical
) {
#define DOM2REBEL(p_str, p_rebel_code)                                         \
    if (memcmp((const void*)p_str, (void*)p_code, strlen(p_str) + 1) == 0) {   \
        return KEY_##p_rebel_code;                                             \
    }

    // Numpad section.
    DOM2REBEL("NumLock", NUMLOCK);
    DOM2REBEL("Numpad0", KP_0);
    DOM2REBEL("Numpad1", KP_1);
    DOM2REBEL("Numpad2", KP_2);
    DOM2REBEL("Numpad3", KP_3);
    DOM2REBEL("Numpad4", KP_4);
    DOM2REBEL("Numpad5", KP_5);
    DOM2REBEL("Numpad6", KP_6);
    DOM2REBEL("Numpad7", KP_7);
    DOM2REBEL("Numpad8", KP_8);
    DOM2REBEL("Numpad9", KP_9);
    DOM2REBEL("NumpadAdd", KP_ADD);
    DOM2REBEL("NumpadBackspace", BACKSPACE);
    DOM2REBEL("NumpadClear", CLEAR);
    DOM2REBEL("NumpadClearEntry", CLEAR);
    // DOM2REBEL("NumpadComma", UNKNOWN);
    DOM2REBEL("NumpadDecimal", KP_PERIOD);
    DOM2REBEL("NumpadDivide", KP_DIVIDE);
    DOM2REBEL("NumpadEnter", KP_ENTER);
    DOM2REBEL("NumpadEqual", EQUAL);
    // DOM2REBEL("NumpadHash", UNKNOWN);
    // DOM2REBEL("NumpadMemoryAdd", UNKNOWN);
    // DOM2REBEL("NumpadMemoryClear", UNKNOWN);
    // DOM2REBEL("NumpadMemoryRecall", UNKNOWN);
    // DOM2REBEL("NumpadMemoryStore", UNKNOWN);
    // DOM2REBEL("NumpadMemorySubtract", UNKNOWN);
    DOM2REBEL("NumpadMultiply", KP_MULTIPLY);
    DOM2REBEL("NumpadParenLeft", PARENLEFT);
    DOM2REBEL("NumpadParenRight", PARENRIGHT);
    DOM2REBEL("NumpadStar", KP_MULTIPLY); // or ASTERISK ?
    DOM2REBEL("NumpadSubtract", KP_SUBTRACT);

    // Printable ASCII.
    if (!p_physical) {
        uint8_t b0 = (uint8_t)p_key[0];
        uint8_t b1 = (uint8_t)p_key[1];
        uint8_t b2 = (uint8_t)p_key[2];
        if (b1 == 0 && b0 > 0x1F && b0 < 0x7F) { // ASCII.
            if (b0 > 0x60 && b0 < 0x7B) {        // Lowercase ASCII.
                b0 -= 32;
            }
            return b0;
        }

#define _U_2BYTES_MASK 0xE0
#define _U_2BYTES      0xC0
        // Latin-1 codes.
        if (b2 == 0
            && (b0 & _U_2BYTES_MASK)
                   == _U_2BYTES) { // 2-bytes utf8, only known latin.
            uint32_t key = ((b0 & ~_U_2BYTES_MASK) << 6) | (b1 & 0x3F);
            if (key >= 0xA0 && key <= 0xDF) {
                return key;
            }
            if (key >= 0xE0 && key <= 0xFF) { // Lowercase known latin.
                key -= 0x20;
                return key;
            }
        }
#undef _U_2BYTES_MASK
#undef _U_2BYTES
    }

    // Alphanumeric section.
    DOM2REBEL("Backquote", QUOTELEFT);
    DOM2REBEL("Backslash", BACKSLASH);
    DOM2REBEL("BracketLeft", BRACKETLEFT);
    DOM2REBEL("BracketRight", BRACKETRIGHT);
    DOM2REBEL("Comma", COMMA);
    DOM2REBEL("Digit0", 0);
    DOM2REBEL("Digit1", 1);
    DOM2REBEL("Digit2", 2);
    DOM2REBEL("Digit3", 3);
    DOM2REBEL("Digit4", 4);
    DOM2REBEL("Digit5", 5);
    DOM2REBEL("Digit6", 6);
    DOM2REBEL("Digit7", 7);
    DOM2REBEL("Digit8", 8);
    DOM2REBEL("Digit9", 9);
    DOM2REBEL("Equal", EQUAL);
    DOM2REBEL("IntlBackslash", BACKSLASH);
    // DOM2REBEL("IntlRo", UNKNOWN);
    DOM2REBEL("IntlYen", YEN);

    DOM2REBEL("KeyA", A);
    DOM2REBEL("KeyB", B);
    DOM2REBEL("KeyC", C);
    DOM2REBEL("KeyD", D);
    DOM2REBEL("KeyE", E);
    DOM2REBEL("KeyF", F);
    DOM2REBEL("KeyG", G);
    DOM2REBEL("KeyH", H);
    DOM2REBEL("KeyI", I);
    DOM2REBEL("KeyJ", J);
    DOM2REBEL("KeyK", K);
    DOM2REBEL("KeyL", L);
    DOM2REBEL("KeyM", M);
    DOM2REBEL("KeyN", N);
    DOM2REBEL("KeyO", O);
    DOM2REBEL("KeyP", P);
    DOM2REBEL("KeyQ", Q);
    DOM2REBEL("KeyR", R);
    DOM2REBEL("KeyS", S);
    DOM2REBEL("KeyT", T);
    DOM2REBEL("KeyU", U);
    DOM2REBEL("KeyV", V);
    DOM2REBEL("KeyW", W);
    DOM2REBEL("KeyX", X);
    DOM2REBEL("KeyY", Y);
    DOM2REBEL("KeyZ", Z);

    DOM2REBEL("Minus", MINUS);
    DOM2REBEL("Period", PERIOD);
    DOM2REBEL("Quote", APOSTROPHE);
    DOM2REBEL("Semicolon", SEMICOLON);
    DOM2REBEL("Slash", SLASH);

    // Functional keys in the Alphanumeric section.
    DOM2REBEL("AltLeft", ALT);
    DOM2REBEL("AltRight", ALT);
    DOM2REBEL("Backspace", BACKSPACE);
    DOM2REBEL("CapsLock", CAPSLOCK);
    DOM2REBEL("ContextMenu", MENU);
    DOM2REBEL("ControlLeft", CONTROL);
    DOM2REBEL("ControlRight", CONTROL);
    DOM2REBEL("Enter", ENTER);
    DOM2REBEL("MetaLeft", SUPER_L);
    DOM2REBEL("MetaRight", SUPER_R);
    DOM2REBEL("ShiftLeft", SHIFT);
    DOM2REBEL("ShiftRight", SHIFT);
    DOM2REBEL("Space", SPACE);
    DOM2REBEL("Tab", TAB);

    // ControlPad section.
    DOM2REBEL("Delete", DELETE);
    DOM2REBEL("End", END);
    DOM2REBEL("Help", HELP);
    DOM2REBEL("Home", HOME);
    DOM2REBEL("Insert", INSERT);
    DOM2REBEL("PageDown", PAGEDOWN);
    DOM2REBEL("PageUp", PAGEUP);

    // ArrowPad section.
    DOM2REBEL("ArrowDown", DOWN);
    DOM2REBEL("ArrowLeft", LEFT);
    DOM2REBEL("ArrowRight", RIGHT);
    DOM2REBEL("ArrowUp", UP);

    // Function section.
    DOM2REBEL("Escape", ESCAPE);
    DOM2REBEL("F1", F1);
    DOM2REBEL("F2", F2);
    DOM2REBEL("F3", F3);
    DOM2REBEL("F4", F4);
    DOM2REBEL("F5", F5);
    DOM2REBEL("F6", F6);
    DOM2REBEL("F7", F7);
    DOM2REBEL("F8", F8);
    DOM2REBEL("F9", F9);
    DOM2REBEL("F10", F10);
    DOM2REBEL("F11", F11);
    DOM2REBEL("F12", F12);
    // DOM2REBEL("Fn", UNKNOWN); // never actually fired, but included in the
    // standard draft. DOM2REBEL("FnLock", UNKNOWN);
    DOM2REBEL("PrintScreen", PRINT);
    DOM2REBEL("ScrollLock", SCROLLLOCK);
    DOM2REBEL("Pause", PAUSE);

    // Media keys section.
    DOM2REBEL("BrowserBack", BACK);
    DOM2REBEL("BrowserFavorites", FAVORITES);
    DOM2REBEL("BrowserForward", FORWARD);
    DOM2REBEL("BrowserHome", OPENURL);
    DOM2REBEL("BrowserRefresh", REFRESH);
    DOM2REBEL("BrowserSearch", SEARCH);
    DOM2REBEL("BrowserStop", STOP);
    // DOM2REBEL("Eject", UNKNOWN);
    DOM2REBEL("LaunchApp1", LAUNCH0);
    DOM2REBEL("LaunchApp2", LAUNCH1);
    DOM2REBEL("LaunchMail", LAUNCHMAIL);
    DOM2REBEL("MediaPlayPause", MEDIAPLAY);
    DOM2REBEL("MediaSelect", LAUNCHMEDIA);
    DOM2REBEL("MediaStop", MEDIASTOP);
    DOM2REBEL("MediaTrackNext", MEDIANEXT);
    DOM2REBEL("MediaTrackPrevious", MEDIAPREVIOUS);
    // DOM2REBEL("Power", UNKNOWN);
    // DOM2REBEL("Sleep", UNKNOWN);
    DOM2REBEL("AudioVolumeDown", VOLUMEDOWN);
    DOM2REBEL("AudioVolumeMute", VOLUMEMUTE);
    DOM2REBEL("AudioVolumeUp", VOLUMEUP);
    // DOM2REBEL("WakeUp", UNKNOWN);
    return KEY_UNKNOWN;
#undef DOM2REBEL
}
