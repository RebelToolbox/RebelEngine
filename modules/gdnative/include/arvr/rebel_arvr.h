// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_ARVR_H
#define GDNATIVE_ARVR_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

// For future versions of the API we should only add new functions at the end of
// the structure and use the version info to detect whether a call is available

// Use these to populate version in your plugin
#define REBEL_VR_API_MAJOR 1
#define REBEL_VR_API_MINOR 2

typedef struct {
    rebel_gdnative_api_version version; /* version of our API */
    void* (*constructor)(rebel_object*);
    void (*destructor)(void*);
    rebel_string (*get_name)(const void*);
    rebel_int (*get_capabilities)(const void*);
    rebel_bool (*get_anchor_detection_is_enabled)(const void*);
    void (*set_anchor_detection_is_enabled)(void*, rebel_bool);
    rebel_bool (*is_stereo)(const void*);
    rebel_bool (*is_initialized)(const void*);
    rebel_bool (*initialize)(void*);
    void (*uninitialize)(void*);
    rebel_vector2 (*get_render_targetsize)(const void*);
    rebel_transform (*get_transform_for_eye)(void*, rebel_int, rebel_transform*);
    void (*fill_projection_for_eye)(
        void*,
        rebel_real*,
        rebel_int,
        rebel_real,
        rebel_real,
        rebel_real
    );
    void (*commit_for_eye)(void*, rebel_int, rebel_rid*, rebel_rect2*);
    void (*process)(void*);
    // only in 1.1 onwards
    rebel_int (*get_external_texture_for_eye)(void*, rebel_int);
    void (*notification)(void*, rebel_int);
    rebel_int (*get_camera_feed_id)(void*);
    // only in 1.2 onwards
    rebel_int (*get_external_depth_for_eye)(void*, rebel_int);
} rebel_arvr_interface_gdnative;

void GDAPI
rebel_arvr_register_interface(const rebel_arvr_interface_gdnative* p_interface);

// helper functions to access ARVRServer data
rebel_real GDAPI rebel_arvr_get_worldscale();
rebel_transform GDAPI rebel_arvr_get_reference_frame();

// helper functions for rendering
void GDAPI rebel_arvr_blit(
    rebel_int p_eye,
    rebel_rid* p_render_target,
    rebel_rect2* p_rect
);
rebel_int GDAPI rebel_arvr_get_texid(rebel_rid* p_render_target);

// helper functions for updating ARVR controllers
rebel_int GDAPI rebel_arvr_add_controller(
    char* p_device_name,
    rebel_int p_hand,
    rebel_bool p_tracks_orientation,
    rebel_bool p_tracks_position
);
void GDAPI rebel_arvr_remove_controller(rebel_int p_controller_id);
void GDAPI rebel_arvr_set_controller_transform(
    rebel_int p_controller_id,
    rebel_transform* p_transform,
    rebel_bool p_tracks_orientation,
    rebel_bool p_tracks_position
);
void GDAPI rebel_arvr_set_controller_button(
    rebel_int p_controller_id,
    rebel_int p_button,
    rebel_bool p_is_pressed
);
void GDAPI rebel_arvr_set_controller_axis(
    rebel_int p_controller_id,
    rebel_int p_axis,
    rebel_real p_value,
    rebel_bool p_can_be_negative
);
rebel_real GDAPI rebel_arvr_get_controller_rumble(rebel_int p_controller_id);

// ARVR 1.2 functions
void GDAPI rebel_arvr_set_interface(
    rebel_object* p_arvr_interface,
    const rebel_arvr_interface_gdnative* p_gdn_interface
);
rebel_int GDAPI rebel_arvr_get_depthid(rebel_rid* p_render_target);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_ARVR_H
