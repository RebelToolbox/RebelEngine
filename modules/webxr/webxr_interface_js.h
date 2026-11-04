// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEBXR_INTERFACE_JS_H
#define WEBXR_INTERFACE_JS_H

#ifdef WEB_ENABLED

#include "webxr_interface.h"

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
    void is_session_supported(const String& p_session_mode) override;
    void set_session_mode(String p_session_mode) override;
    String get_session_mode() const override;
    void set_required_features(String p_required_features) override;
    String get_required_features() const override;
    void set_optional_features(String p_optional_features) override;
    String get_optional_features() const override;
    void set_requested_reference_space_types(
        String p_requested_reference_space_types
    ) override;
    String get_requested_reference_space_types() const override;
    void _set_reference_space_type(String p_reference_space_type);
    String get_reference_space_type() const override;
    Ref<ARVRPositionalTracker> get_controller(
        int p_controller_id
    ) const override;
    String get_visibility_state() const override;
    PoolVector3Array get_bounds_geometry() const override;

    StringName get_name() const override;
    int get_capabilities() const override;

    bool is_initialized() const override;
    bool initialize() override;
    void uninitialize() override;

    Size2 get_render_targetsize() override;
    bool is_stereo() override;
    Transform get_transform_for_eye(
        ARVRInterface::Eyes p_eye,
        const Transform& p_cam_transform
    ) override;
    CameraMatrix get_projection_for_eye(
        ARVRInterface::Eyes p_eye,
        real_t p_aspect,
        real_t p_z_near,
        real_t p_z_far
    ) override;
    unsigned int get_external_texture_for_eye(
        ARVRInterface::Eyes p_eye
    ) override;
    void commit_for_eye(
        ARVRInterface::Eyes p_eye,
        RID p_render_target,
        const Rect2& p_screen_rect
    ) override;

    void process() override;
    void notification(int p_what) override;

    void _on_controller_changed();

    WebXRInterfaceJS();
    ~WebXRInterfaceJS() override;
};

#endif // WEB_ENABLED

#endif // WEBXR_INTERFACE_JS_H
