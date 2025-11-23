// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_STREAM_STB_VORBIS_H
#define AUDIO_STREAM_STB_VORBIS_H

#include "core/io/resource_loader.h"
#include "servers/audio/audio_stream.h"
#include "third-party/misc/stb_vorbis.h"

class AudioStreamOGGVorbis;

class AudioStreamPlaybackOGGVorbis : public AudioStreamPlaybackResampled {
    GDCLASS(AudioStreamPlaybackOGGVorbis, AudioStreamPlaybackResampled);

    stb_vorbis* ogg_stream;
    stb_vorbis_alloc ogg_alloc;
    uint32_t frames_mixed;
    bool active;
    int loops;

    friend class AudioStreamOGGVorbis;

    Ref<AudioStreamOGGVorbis> vorbis_stream;

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

    AudioStreamPlaybackOGGVorbis() {}

    ~AudioStreamPlaybackOGGVorbis();
};

class AudioStreamOGGVorbis : public AudioStream {
    GDCLASS(AudioStreamOGGVorbis, AudioStream);
    OBJ_SAVE_TYPE(AudioStream
    ); // Saves derived classes with common type so they can be interchanged.
    RES_BASE_EXTENSION("oggstr");

    friend class AudioStreamPlaybackOGGVorbis;

    void* data;
    uint32_t data_len;

    int decode_mem_size;
    float sample_rate;
    int channels;
    float length;
    bool loop;
    float loop_offset;
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

    virtual float get_length() const; // if supported, otherwise return 0

    AudioStreamOGGVorbis();
    virtual ~AudioStreamOGGVorbis();
};

#endif
