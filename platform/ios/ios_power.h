// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IOS_POWER_H
#define IOS_POWER_H

#include <os/os.h>

class IosPower {
private:
    int nsecs_left;
    int percent_left;
    OS::PowerState power_state;

    bool UpdatePowerInfo();

public:
    IosPower();
    virtual ~IosPower();

    OS::PowerState get_power_state();
    int get_power_seconds_left();
    int get_power_percent_left();
};

#endif // IOS_POWER_H
