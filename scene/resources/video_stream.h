// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#include "scene/resources/texture.h"

class VideoStreamPlayback : public Resource {
    GDCLASS(VideoStreamPlayback, Resource);

public:
    typedef int (*AudioMixCallback)(
        void* p_udata,
        const float* p_data,
        int p_frames
    );

    virtual void stop() = 0;
    virtual void play() = 0;

    virtual bool is_playing() const = 0;

    virtual void set_paused(bool p_paused) = 0;
    virtual bool is_paused() const         = 0;

    virtual void set_loop(bool p_enable) = 0;
    virtual bool has_loop() const        = 0;

    virtual float get_length() const = 0;

    virtual float get_playback_position() const = 0;
    virtual void seek(float p_time)             = 0;

    virtual void set_audio_track(int p_idx) = 0;

    virtual Ref<Texture> get_texture() const = 0;
    virtual void update(float p_delta)       = 0;

    virtual void set_mix_callback(
        AudioMixCallback p_callback,
        void* p_userdata
    )                                = 0;
    virtual int get_channels() const = 0;
    virtual int get_mix_rate() const = 0;
};

class VideoStream : public Resource {
    GDCLASS(VideoStream, Resource);
    OBJ_SAVE_TYPE(VideoStream
    ); // Saves derived classes with common type so they can be interchanged.

public:
    virtual void set_audio_track(int p_track)            = 0;
    virtual Ref<VideoStreamPlayback> instance_playback() = 0;
};

#endif
