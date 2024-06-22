// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "uwp_power.h"

UwpPower::UwpPower() :
    nsecs_left(-1),
    percent_left(-1),
    power_state(OS::POWERSTATE_UNKNOWN) {}

UwpPower::~UwpPower() {}

bool UwpPower::UpdatePowerInfo() {
    // TODO, WinRT: Battery info is available on at least one WinRT platform
    // (Windows Phone 8).  Implement UpdatePowerInfo as appropriate. */
    /* Notes from SDL:
     * - the Win32 function, GetSystemPowerStatus, is not available for use on
     * WinRT
     * - Windows Phone 8 has a 'Battery' class, which is documented as available
     * for C++
     * - More info on WP8's Battery class can be found at
     * http://msdn.microsoft.com/library/windowsphone/develop/jj207231
     */
    return false;
}

OS::PowerState UwpPower::get_power_state() {
    if (UpdatePowerInfo()) {
        return power_state;
    } else {
        WARN_PRINT(
            "Power management is not implemented on this platform, defaulting "
            "to POWERSTATE_UNKNOWN"
        );
        return OS::POWERSTATE_UNKNOWN;
    }
}

int UwpPower::get_power_seconds_left() {
    if (UpdatePowerInfo()) {
        return nsecs_left;
    } else {
        WARN_PRINT(
            "Power management is not implemented on this platform, defaulting "
            "to -1"
        );
        return -1;
    }
}

int UwpPower::get_power_percent_left() {
    if (UpdatePowerInfo()) {
        return percent_left;
    } else {
        WARN_PRINT(
            "Power management is not implemented on this platform, defaulting "
            "to -1"
        );
        return -1;
    }
}
