// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_STREAM_MP3_H
#define AUDIO_STREAM_MP3_H

#include "core/io/resource_loader.h"
#include "minimp3_ex.h"
#include "servers/audio/audio_stream.h"

class AudioStreamMP3;

class AudioStreamPlaybackMP3 : public AudioStreamPlaybackResampled {
    GDCLASS(AudioStreamPlaybackMP3, AudioStreamPlaybackResampled);

    mp3dec_ex_t* mp3d     = nullptr;
    uint32_t frames_mixed = 0;
    bool active           = false;
    int loops             = 0;

    friend class AudioStreamMP3;

    Ref<AudioStreamMP3> mp3_stream;

protected:
    virtual void _mix_internal(AudioFrame* p_buffer, int p_frames);
    virtual float get_stream_sampling_rate();

public:
    virtual void start(float p_from_pos = 0.0);
    virtual void stop();
    virtual bool is_playing() const;

    virtual int get_loop_count() const; // times it looped

    virtual float get_playback_position() const;
    virtual void seek(float p_time);

    AudioStreamPlaybackMP3() {}

    ~AudioStreamPlaybackMP3();
};

class AudioStreamMP3 : public AudioStream {
    GDCLASS(AudioStreamMP3, AudioStream);
    OBJ_SAVE_TYPE(AudioStream
    ) // children are all saved as AudioStream, so they can be exchanged
    RES_BASE_EXTENSION("mp3str");

    friend class AudioStreamPlaybackMP3;

    void* data        = nullptr;
    uint32_t data_len = 0;

    float sample_rate = 1;
    int channels      = 1;
    float length      = 0;
    bool loop         = false;
    float loop_offset = 0;
    void clear_data();

protected:
    static void _bind_methods();

public:
    void set_loop(bool p_enable);
    bool has_loop() const;

    void set_loop_offset(float p_seconds);
    float get_loop_offset() const;

    virtual Ref<AudioStreamPlayback> instance_playback();
    virtual String get_stream_name() const;

    void set_data(const PoolVector<uint8_t>& p_data);
    PoolVector<uint8_t> get_data() const;

    virtual float get_length() const;

    AudioStreamMP3();
    virtual ~AudioStreamMP3();
};

#endif // AUDIO_STREAM_MP3_H
