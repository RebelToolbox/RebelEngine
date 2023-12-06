// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GODOT_WEBXR_H
#define GODOT_WEBXR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

typedef void (*GodotWebXRSupportedCallback)(
    char* p_session_mode,
    int p_supported
);
typedef void (*GodotWebXRStartedCallback)(char* p_reference_space_type);
typedef void (*GodotWebXREndedCallback)();
typedef void (*GodotWebXRFailedCallback)(char* p_message);
typedef void (*GodotWebXRControllerCallback)();
typedef void (*GodotWebXRInputEventCallback)(
    char* p_signal_name,
    int p_controller_id
);
typedef void (*GodotWebXRSimpleEventCallback)(char* p_signal_name);

extern int godot_webxr_is_supported();
extern void godot_webxr_is_session_supported(
    const char* p_session_mode,
    GodotWebXRSupportedCallback p_callback
);

extern void godot_webxr_initialize(
    const char* p_session_mode,
    const char* p_required_features,
    const char* p_optional_features,
    const char* p_requested_reference_space_types,
    GodotWebXRStartedCallback p_on_session_started,
    GodotWebXREndedCallback p_on_session_ended,
    GodotWebXRFailedCallback p_on_session_failed,
    GodotWebXRControllerCallback p_on_controller_changed,
    GodotWebXRInputEventCallback p_on_input_event,
    GodotWebXRSimpleEventCallback p_on_simple_event
);
extern void godot_webxr_uninitialize();

extern int godot_webxr_get_view_count();
extern int* godot_webxr_get_render_targetsize();
extern float* godot_webxr_get_transform_for_eye(int p_eye);
extern float* godot_webxr_get_projection_for_eye(int p_eye);
extern int godot_webxr_get_external_texture_for_eye(int p_eye);
extern void godot_webxr_commit_for_eye(int p_eye);

extern void godot_webxr_sample_controller_data();
extern int godot_webxr_get_controller_count();
extern int godot_webxr_is_controller_connected(int p_controller);
extern float* godot_webxr_get_controller_transform(int p_controller);
extern int* godot_webxr_get_controller_buttons(int p_controller);
extern int* godot_webxr_get_controller_axes(int p_controller);

extern char* godot_webxr_get_visibility_state();
extern int* godot_webxr_get_bounds_geometry();

#ifdef __cplusplus
}
#endif

#endif /* GODOT_WEBXR_H */
