// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "mobile_vr_interface.h"

void register_mobile_vr_types() {
    ClassDB::register_class<MobileVRInterface>();

    Ref<MobileVRInterface> mobile_vr;
    mobile_vr.instance();
    ARVRServer::get_singleton()->add_interface(mobile_vr);
}

void unregister_mobile_vr_types() {}
