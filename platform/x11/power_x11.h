// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef POWER_X11_H
#define POWER_X11_H

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"

class PowerX11 {
private:
    int nsecs_left;
    int percent_left;
    OS::PowerState power_state;

    FileAccessRef open_power_file(
        const char* base,
        const char* node,
        const char* key
    );
    bool read_power_file(
        const char* base,
        const char* node,
        const char* key,
        char* buf,
        size_t buflen
    );
    bool make_proc_acpi_key_val(char** _ptr, char** _key, char** _val);
    void check_proc_acpi_battery(
        const char* node,
        bool* have_battery,
        bool* charging
    );
    void check_proc_acpi_ac_adapter(const char* node, bool* have_ac);
    bool GetPowerInfo_Linux_proc_acpi();
    bool next_string(char** _ptr, char** _str);
    bool int_string(char* str, int* val);
    bool GetPowerInfo_Linux_proc_apm();
    bool GetPowerInfo_Linux_sys_class_power_supply();
    bool UpdatePowerInfo();

public:
    PowerX11();
    virtual ~PowerX11();

    OS::PowerState get_power_state();
    int get_power_seconds_left();
    int get_power_percent_left();
};

#endif // POWER_X11_H
