// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MACOS_POWER_H
#define MACOS_POWER_H

#include "core/os/file_access.h"
#include "core/os/os.h"
#include "macos_dir_access.h"

#include <CoreFoundation/CoreFoundation.h>

class MacOSPower {
private:
    int nsecs_left;
    int percent_left;
    OS::PowerState power_state;
    void checkps(
        CFDictionaryRef dict,
        bool* have_ac,
        bool* have_battery,
        bool* charging
    );
    bool GetPowerInfo();
    bool UpdatePowerInfo();

public:
    MacOSPower();
    virtual ~MacOSPower();

    OS::PowerState get_power_state();
    int get_power_seconds_left();
    int get_power_percent_left();
};

#endif // MACOS_POWER_H
