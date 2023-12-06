// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef POWER_UWP_H
#define POWER_UWP_H

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"

class PowerUWP {
private:
    int nsecs_left;
    int percent_left;
    OS::PowerState power_state;

    bool UpdatePowerInfo();

public:
    PowerUWP();
    virtual ~PowerUWP();

    OS::PowerState get_power_state();
    int get_power_seconds_left();
    int get_power_percent_left();
};

#endif // POWER_UWP_H
