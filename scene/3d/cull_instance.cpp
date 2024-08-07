// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "cull_instance.h"

VARIANT_ENUM_CAST(CullInstance::PortalMode);

void CullInstance::set_portal_mode(CullInstance::PortalMode p_mode) {
    _portal_mode = p_mode;
    _refresh_portal_mode();
}

CullInstance::PortalMode CullInstance::get_portal_mode() const {
    return _portal_mode;
}

void CullInstance::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_portal_mode", "mode"),
        &CullInstance::set_portal_mode
    );
    ClassDB::bind_method(
        D_METHOD("get_portal_mode"),
        &CullInstance::get_portal_mode
    );

    ClassDB::bind_method(
        D_METHOD("set_include_in_bound"),
        &CullInstance::set_include_in_bound
    );
    ClassDB::bind_method(
        D_METHOD("get_include_in_bound"),
        &CullInstance::get_include_in_bound
    );

    ClassDB::bind_method(
        D_METHOD("set_portal_autoplace_priority", "priority"),
        &CullInstance::set_portal_autoplace_priority
    );
    ClassDB::bind_method(
        D_METHOD("get_portal_autoplace_priority"),
        &CullInstance::get_portal_autoplace_priority
    );

    ADD_GROUP("Portals", "");

    BIND_ENUM_CONSTANT(PORTAL_MODE_STATIC);
    BIND_ENUM_CONSTANT(PORTAL_MODE_DYNAMIC);
    BIND_ENUM_CONSTANT(PORTAL_MODE_ROAMING);
    BIND_ENUM_CONSTANT(PORTAL_MODE_GLOBAL);
    BIND_ENUM_CONSTANT(PORTAL_MODE_IGNORE);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "portal_mode",
            PROPERTY_HINT_ENUM,
            "Static,Dynamic,Roaming,Global,Ignore"
        ),
        "set_portal_mode",
        "get_portal_mode"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "include_in_bound"),
        "set_include_in_bound",
        "get_include_in_bound"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "autoplace_priority",
            PROPERTY_HINT_RANGE,
            "-16,16,1",
            PROPERTY_USAGE_DEFAULT
        ),
        "set_portal_autoplace_priority",
        "get_portal_autoplace_priority"
    );
}

CullInstance::CullInstance() {
    _portal_mode               = PORTAL_MODE_STATIC;
    _include_in_bound          = true;
    _portal_autoplace_priority = 0;
}
