// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CRASH_HANDLER_WINDOWS_H
#define CRASH_HANDLER_WINDOWS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Crash handler exception only enabled with MSVC
#if defined(DEBUG_ENABLED) && defined(MSVC)
#define CRASH_HANDLER_EXCEPTION 1

extern DWORD CrashHandlerException(EXCEPTION_POINTERS* ep);
#endif

class CrashHandler {
    bool disabled;

public:
    void initialize();

    void disable();

    bool is_disabled() const {
        return disabled;
    };

    CrashHandler();
    ~CrashHandler();
};

#endif // CRASH_HANDLER_WINDOWS_H
