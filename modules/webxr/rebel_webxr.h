// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REBEL_WEBXR_H
#define REBEL_WEBXR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

typedef void (*RebelWebXRSupportedCallback)(
    char* p_session_mode,
    int p_supported
);
typedef void (*RebelWebXRStartedCallback)(char* p_reference_space_type);
typedef void (*RebelWebXREndedCallback)();
typedef void (*RebelWebXRFailedCallback)(char* p_message);
typedef void (*RebelWebXRControllerCallback)();
typedef void (*RebelWebXRInputEventCallback)(
    char* p_signal_name,
    int p_controller_id
);
typedef void (*RebelWebXRSimpleEventCallback)(char* p_signal_name);

extern int rebel_webxr_is_supported();
extern void rebel_webxr_is_session_supported(
    const char* p_session_mode,
    RebelWebXRSupportedCallback p_callback
);

extern void rebel_webxr_initialize(
    const char* p_session_mode,
    const char* p_required_features,
    const char* p_optional_features,
    const char* p_requested_reference_space_types,
    RebelWebXRStartedCallback p_on_session_started,
    RebelWebXREndedCallback p_on_session_ended,
    RebelWebXRFailedCallback p_on_session_failed,
    RebelWebXRControllerCallback p_on_controller_changed,
    RebelWebXRInputEventCallback p_on_input_event,
    RebelWebXRSimpleEventCallback p_on_simple_event
);
extern void rebel_webxr_uninitialize();

extern int rebel_webxr_get_view_count();
extern int* rebel_webxr_get_render_targetsize();
extern float* rebel_webxr_get_transform_for_eye(int p_eye);
extern float* rebel_webxr_get_projection_for_eye(int p_eye);
extern int rebel_webxr_get_external_texture_for_eye(int p_eye);
extern void rebel_webxr_commit_for_eye(int p_eye);

extern void rebel_webxr_sample_controller_data();
extern int rebel_webxr_get_controller_count();
extern int rebel_webxr_is_controller_connected(int p_controller);
extern float* rebel_webxr_get_controller_transform(int p_controller);
extern int* rebel_webxr_get_controller_buttons(int p_controller);
extern int* rebel_webxr_get_controller_axes(int p_controller);

extern char* rebel_webxr_get_visibility_state();
extern int* rebel_webxr_get_bounds_geometry();

#ifdef __cplusplus
}
#endif

#endif // REBEL_WEBXR_H
