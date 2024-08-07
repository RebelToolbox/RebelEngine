// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEBXR_INTERFACE_JS_H
#define WEBXR_INTERFACE_JS_H

#ifdef WEB_ENABLED

#include "webxr_interface.h"

/**
    @author David Snopek <david.snopek@snopekgames.com>

    The WebXR interface is a VR/AR interface that can be used on the web.
*/

class WebXRInterfaceJS : public WebXRInterface {
    GDCLASS(WebXRInterfaceJS, WebXRInterface);

private:
    bool initialized;

    String session_mode;
    String required_features;
    String optional_features;
    String requested_reference_space_types;
    String reference_space_type;

    bool controllers_state[2];
    Size2 render_targetsize;

    Transform _js_matrix_to_transform(float* p_js_matrix);
    void _update_tracker(int p_controller_id);

public:
    virtual void is_session_supported(const String& p_session_mode);
    virtual void set_session_mode(String p_session_mode);
    virtual String get_session_mode() const;
    virtual void set_required_features(String p_required_features);
    virtual String get_required_features() const;
    virtual void set_optional_features(String p_optional_features);
    virtual String get_optional_features() const;
    virtual void set_requested_reference_space_types(
        String p_requested_reference_space_types
    );
    virtual String get_requested_reference_space_types() const;
    void _set_reference_space_type(String p_reference_space_type);
    virtual String get_reference_space_type() const;
    virtual Ref<ARVRPositionalTracker> get_controller(int p_controller_id
    ) const;
    virtual String get_visibility_state() const;
    virtual PoolVector3Array get_bounds_geometry() const;

    virtual StringName get_name() const;
    virtual int get_capabilities() const;

    virtual bool is_initialized() const;
    virtual bool initialize();
    virtual void uninitialize();

    virtual Size2 get_render_targetsize();
    virtual bool is_stereo();
    virtual Transform get_transform_for_eye(
        ARVRInterface::Eyes p_eye,
        const Transform& p_cam_transform
    );
    virtual CameraMatrix get_projection_for_eye(
        ARVRInterface::Eyes p_eye,
        real_t p_aspect,
        real_t p_z_near,
        real_t p_z_far
    );
    virtual unsigned int get_external_texture_for_eye(ARVRInterface::Eyes p_eye
    );
    virtual void commit_for_eye(
        ARVRInterface::Eyes p_eye,
        RID p_render_target,
        const Rect2& p_screen_rect
    );

    virtual void process();
    virtual void notification(int p_what);

    void _on_controller_changed();

    WebXRInterfaceJS();
    ~WebXRInterfaceJS();
};

#endif // WEB_ENABLED

#endif // WEBXR_INTERFACE_JS_H
