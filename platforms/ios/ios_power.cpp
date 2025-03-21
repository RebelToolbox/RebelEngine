// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "ios_power.h"

bool IosPower::UpdatePowerInfo() {
    return false;
}

OS::PowerState IosPower::get_power_state() {
    if (UpdatePowerInfo()) {
        return power_state;
    } else {
        return OS::POWERSTATE_UNKNOWN;
    }
}

int IosPower::get_power_seconds_left() {
    if (UpdatePowerInfo()) {
        return nsecs_left;
    } else {
        return -1;
    }
}

int IosPower::get_power_percent_left() {
    if (UpdatePowerInfo()) {
        return percent_left;
    } else {
        return -1;
    }
}

IosPower::IosPower() :
    nsecs_left(-1),
    percent_left(-1),
    power_state(OS::POWERSTATE_UNKNOWN) {
    // TODO Auto-generated constructor stub
}

IosPower::~IosPower() {
    // TODO Auto-generated destructor stub
}
