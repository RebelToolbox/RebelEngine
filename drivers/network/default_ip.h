// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef DEFAULT_IP_H
#define DEFAULT_IP_H

#include "core/io/ip.h"

class DefaultIP : public IP {
    GDCLASS(DefaultIP, IP);

    virtual void _resolve_hostname(
        List<IP_Address>& r_addresses,
        const String& p_hostname,
        Type p_type = TYPE_ANY
    ) const;

    static IP* _create_default();

public:
    virtual void get_local_interfaces(Map<String, Interface_Info>* r_interfaces
    ) const;

    static void make_default();
    DefaultIP();
};

#endif // DEFAULT_IP_H
