// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef REBEL_AUDIO_H
#define REBEL_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

extern int rebel_audio_is_available();
extern int rebel_audio_has_worklet();
extern int rebel_audio_has_script_processor();
extern int rebel_audio_init(
    int* p_mix_rate,
    int p_latency,
    void (*_state_cb)(int),
    void (*_latency_cb)(float)
);
extern void rebel_audio_resume();

extern int rebel_audio_capture_start();
extern void rebel_audio_capture_stop();

// Worklet
typedef int32_t AudioState[4];
extern int rebel_audio_worklet_create(int p_channels);
extern void rebel_audio_worklet_start(
    float* p_in_buf,
    int p_in_size,
    float* p_out_buf,
    int p_out_size,
    AudioState p_state
);
extern void rebel_audio_worklet_start_no_threads(
    float* p_out_buf,
    int p_out_size,
    void (*p_out_cb)(int p_pos, int p_frames),
    float* p_in_buf,
    int p_in_size,
    void (*p_in_cb)(int p_pos, int p_frames)
);
extern int rebel_audio_worklet_state_add(
    AudioState p_state,
    int p_idx,
    int p_value
);
extern int rebel_audio_worklet_state_get(AudioState p_state, int p_idx);
extern int rebel_audio_worklet_state_wait(
    int32_t* p_state,
    int p_idx,
    int32_t p_expected,
    int p_timeout
);

// Script
extern int rebel_audio_script_create(int* p_buffer_size, int p_channels);
extern void rebel_audio_script_start(
    float* p_in_buf,
    int p_in_size,
    float* p_out_buf,
    int p_out_size,
    void (*p_cb)()
);

#ifdef __cplusplus
}
#endif

#endif // REBEL_AUDIO_H
