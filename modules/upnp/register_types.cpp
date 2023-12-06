// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/error_macros.h"
#include "upnp.h"
#include "upnp_device.h"

void register_upnp_types() {
    ClassDB::register_class<UPNP>();
    ClassDB::register_class<UPNPDevice>();
}

void unregister_upnp_types() {}
