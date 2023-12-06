// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEBXR_INTERFACE_H
#define WEBXR_INTERFACE_H

#include "servers/arvr/arvr_interface.h"
#include "servers/arvr/arvr_positional_tracker.h"

/**
    @author David Snopek <david.snopek@snopekgames.com>

    The WebXR interface is a VR/AR interface that can be used on the web.
*/

class WebXRInterface : public ARVRInterface {
    GDCLASS(WebXRInterface, ARVRInterface);

protected:
    static void _bind_methods();

public:
    virtual void is_session_supported(const String& p_session_mode) = 0;
    virtual void set_session_mode(String p_session_mode)            = 0;
    virtual String get_session_mode() const                         = 0;
    virtual void set_required_features(String p_required_features)  = 0;
    virtual String get_required_features() const                    = 0;
    virtual void set_optional_features(String p_optional_features)  = 0;
    virtual String get_optional_features() const                    = 0;
    virtual void set_requested_reference_space_types(
        String p_requested_reference_space_types
    )                                                          = 0;
    virtual String get_requested_reference_space_types() const = 0;
    virtual String get_reference_space_type() const            = 0;
    virtual Ref<ARVRPositionalTracker> get_controller(int p_controller_id
    ) const                                                    = 0;
    virtual String get_visibility_state() const                = 0;
    virtual PoolVector3Array get_bounds_geometry() const       = 0;
};

#endif // WEBXR_INTERFACE_H
