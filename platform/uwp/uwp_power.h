// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef UWP_POWER_H
#define UWP_POWER_H

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"

class UwpPower {
private:
    int nsecs_left;
    int percent_left;
    OS::PowerState power_state;

    bool UpdatePowerInfo();

public:
    UwpPower();
    virtual ~UwpPower();

    OS::PowerState get_power_state();
    int get_power_seconds_left();
    int get_power_percent_left();
};

#endif // UWP_POWER_H
