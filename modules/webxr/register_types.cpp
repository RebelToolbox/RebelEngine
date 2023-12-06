// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "webxr_interface.h"
#include "webxr_interface_js.h"

void register_webxr_types() {
    ClassDB::register_virtual_class<WebXRInterface>();

#ifdef JAVASCRIPT_ENABLED
    Ref<WebXRInterfaceJS> webxr;
    webxr.instance();
    ARVRServer::get_singleton()->add_interface(webxr);
#endif
}

void unregister_webxr_types() {}
