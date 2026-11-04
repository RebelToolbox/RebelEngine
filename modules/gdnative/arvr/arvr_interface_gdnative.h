// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ARVR_INTERFACE_GDNATIVE_H
#define ARVR_INTERFACE_GDNATIVE_H

#include "modules/gdnative/gdnative.h"
#include "servers/arvr/arvr_interface.h"

class ARVRInterfaceGDNative : public ARVRInterface {
    GDCLASS(ARVRInterfaceGDNative, ARVRInterface);

    void cleanup();

protected:
    const rebel_arvr_interface_gdnative* interface;
    void* data;

    static void _bind_methods();

public:
    /** general interface information **/
    ARVRInterfaceGDNative();
    ~ARVRInterfaceGDNative() override;

    void set_interface(const rebel_arvr_interface_gdnative* p_interface);

    StringName get_name() const override;
    int get_capabilities() const override;

    bool is_initialized() const override;
    bool initialize() override;
    void uninitialize() override;

    /** specific to AR **/
    bool get_anchor_detection_is_enabled() const override;
    void set_anchor_detection_is_enabled(bool p_enable) override;
    int get_camera_feed_id() override;

    /** rendering and internal **/
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
    unsigned int get_external_depth_for_eye(ARVRInterface::Eyes p_eye) override;
    void commit_for_eye(
        ARVRInterface::Eyes p_eye,
        RID p_render_target,
        const Rect2& p_screen_rect
    ) override;

    void process() override;
    void notification(int p_what) override;
};

#endif // ARVR_INTERFACE_GDNATIVE_H
