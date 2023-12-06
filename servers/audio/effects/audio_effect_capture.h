// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_EFFECT_CAPTURE_H
#define AUDIO_EFFECT_CAPTURE_H

#include "core/engine.h"
#include "core/math/audio_frame.h"
#include "core/pool_vector.h"
#include "core/reference.h"
#include "core/ring_buffer.h"
#include "servers/audio/audio_effect.h"
#include "servers/audio_server.h"

class AudioEffectCapture;

class AudioEffectCaptureInstance : public AudioEffectInstance {
    GDCLASS(AudioEffectCaptureInstance, AudioEffectInstance);
    friend class AudioEffectCapture;
    Ref<AudioEffectCapture> base;

public:
    virtual void process(
        const AudioFrame* p_src_frames,
        AudioFrame* p_dst_frames,
        int p_frame_count
    );
    virtual bool process_silence() const;
};

class AudioEffectCapture : public AudioEffect {
    GDCLASS(AudioEffectCapture, AudioEffect)
    friend class AudioEffectCaptureInstance;

    RingBuffer<AudioFrame> buffer;
    uint64_t discarded_frames;
    uint64_t pushed_frames;
    float buffer_length_seconds;
    bool buffer_initialized;

protected:
    static void _bind_methods();

public:
    AudioEffectCapture() {
        discarded_frames      = 0;
        pushed_frames         = 0;
        buffer_length_seconds = 0.1f;
        buffer_initialized    = false;
    }

    virtual Ref<AudioEffectInstance> instance();

    void set_buffer_length(float p_buffer_length_seconds);
    float get_buffer_length();

    bool can_get_buffer(int p_frames) const;
    PoolVector2Array get_buffer(int p_len);
    void clear_buffer();

    int get_frames_available() const;
    int64_t get_discarded_frames() const;
    int get_buffer_length_frames() const;
    int64_t get_pushed_frames() const;
};

#endif // AUDIO_EFFECT_CAPTURE_H
