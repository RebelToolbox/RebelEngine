// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CRASH_HANDLER_OSX_H
#define CRASH_HANDLER_OSX_H

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

#endif // CRASH_HANDLER_OSX_H
