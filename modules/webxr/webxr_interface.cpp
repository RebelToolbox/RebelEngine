// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "webxr_interface.h"

#include <stdlib.h>

void WebXRInterface::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("is_session_supported", "session_mode"),
        &WebXRInterface::is_session_supported
    );
    ClassDB::bind_method(
        D_METHOD("set_session_mode", "session_mode"),
        &WebXRInterface::set_session_mode
    );
    ClassDB::bind_method(
        D_METHOD("get_session_mode"),
        &WebXRInterface::get_session_mode
    );
    ClassDB::bind_method(
        D_METHOD("set_required_features", "required_features"),
        &WebXRInterface::set_required_features
    );
    ClassDB::bind_method(
        D_METHOD("get_required_features"),
        &WebXRInterface::get_required_features
    );
    ClassDB::bind_method(
        D_METHOD("set_optional_features", "optional_features"),
        &WebXRInterface::set_optional_features
    );
    ClassDB::bind_method(
        D_METHOD("get_optional_features"),
        &WebXRInterface::get_optional_features
    );
    ClassDB::bind_method(
        D_METHOD("get_reference_space_type"),
        &WebXRInterface::get_reference_space_type
    );
    ClassDB::bind_method(
        D_METHOD(
            "set_requested_reference_space_types",
            "requested_reference_space_types"
        ),
        &WebXRInterface::set_requested_reference_space_types
    );
    ClassDB::bind_method(
        D_METHOD("get_requested_reference_space_types"),
        &WebXRInterface::get_requested_reference_space_types
    );
    ClassDB::bind_method(
        D_METHOD("get_controller", "controller_id"),
        &WebXRInterface::get_controller
    );
    ClassDB::bind_method(
        D_METHOD("get_visibility_state"),
        &WebXRInterface::get_visibility_state
    );
    ClassDB::bind_method(
        D_METHOD("get_bounds_geometry"),
        &WebXRInterface::get_bounds_geometry
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "session_mode", PROPERTY_HINT_NONE),
        "set_session_mode",
        "get_session_mode"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "required_features", PROPERTY_HINT_NONE),
        "set_required_features",
        "get_required_features"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "optional_features", PROPERTY_HINT_NONE),
        "set_optional_features",
        "get_optional_features"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::STRING,
            "requested_reference_space_types",
            PROPERTY_HINT_NONE
        ),
        "set_requested_reference_space_types",
        "get_requested_reference_space_types"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::STRING,
            "reference_space_type",
            PROPERTY_HINT_NONE
        ),
        "",
        "get_reference_space_type"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "visibility_state", PROPERTY_HINT_NONE),
        "",
        "get_visibility_state"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::POOL_VECTOR3_ARRAY,
            "bounds_geometry",
            PROPERTY_HINT_NONE
        ),
        "",
        "get_bounds_geometry"
    );

    ADD_SIGNAL(MethodInfo(
        "session_supported",
        PropertyInfo(Variant::STRING, "session_mode"),
        PropertyInfo(Variant::BOOL, "supported")
    ));
    ADD_SIGNAL(MethodInfo("session_started"));
    ADD_SIGNAL(MethodInfo("session_ended"));
    ADD_SIGNAL(
        MethodInfo("session_failed", PropertyInfo(Variant::STRING, "message"))
    );

    ADD_SIGNAL(
        MethodInfo("selectstart", PropertyInfo(Variant::INT, "controller_id"))
    );
    ADD_SIGNAL(MethodInfo("select", PropertyInfo(Variant::INT, "controller_id"))
    );
    ADD_SIGNAL(
        MethodInfo("selectend", PropertyInfo(Variant::INT, "controller_id"))
    );
    ADD_SIGNAL(
        MethodInfo("squeezestart", PropertyInfo(Variant::INT, "controller_id"))
    );
    ADD_SIGNAL(
        MethodInfo("squeeze", PropertyInfo(Variant::INT, "controller_id"))
    );
    ADD_SIGNAL(
        MethodInfo("squeezeend", PropertyInfo(Variant::INT, "controller_id"))
    );

    ADD_SIGNAL(MethodInfo("visibility_state_changed"));
    ADD_SIGNAL(MethodInfo("reference_space_reset"));
}
