// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "arvr_interface_gdnative.h"

void register_arvr_types() {
    ClassDB::register_class<ARVRInterfaceGDNative>();
}

void unregister_arvr_types() {}
