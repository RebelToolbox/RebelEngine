// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VIDEO_STREAM_WEBM_H
#define VIDEO_STREAM_WEBM_H

#include "core/io/resource_loader.h"
#include "scene/resources/video_stream.h"

class WebMFrame;
class WebMDemuxer;
class VPXDecoder;
class OpusVorbisDecoder;

class VideoStreamPlaybackWebm : public VideoStreamPlayback {
    GDCLASS(VideoStreamPlaybackWebm, VideoStreamPlayback);

    String file_name;
    int audio_track;

    WebMDemuxer* webm;
    VPXDecoder* video;
    OpusVorbisDecoder* audio;

    WebMFrame **video_frames, *audio_frame;
    int video_frames_pos, video_frames_capacity;

    int num_decoded_samples, samples_offset;
    AudioMixCallback mix_callback;
    void* mix_udata;

    bool playing, paused;
    double delay_compensation;
    double time, video_frame_delay, video_pos;

    PoolVector<uint8_t> frame_data;
    Ref<ImageTexture> texture;

    float* pcm;

public:
    VideoStreamPlaybackWebm();
    ~VideoStreamPlaybackWebm();

    bool open_file(const String& p_file);

    virtual void stop();
    virtual void play();

    virtual bool is_playing() const;

    virtual void set_paused(bool p_paused);
    virtual bool is_paused() const;

    virtual void set_loop(bool p_enable);
    virtual bool has_loop() const;

    virtual float get_length() const;

    virtual float get_playback_position() const;
    virtual void seek(float p_time);

    virtual void set_audio_track(int p_idx);

    virtual Ref<Texture> get_texture() const;
    virtual void update(float p_delta);

    virtual void set_mix_callback(
        AudioMixCallback p_callback,
        void* p_userdata
    );
    virtual int get_channels() const;
    virtual int get_mix_rate() const;

private:
    inline bool has_enough_video_frames() const;
    bool should_process(WebMFrame& video_frame);

    void delete_pointers();
};

/**/

class VideoStreamWebm : public VideoStream {
    GDCLASS(VideoStreamWebm, VideoStream);

    String file;
    int audio_track;

protected:
    static void _bind_methods();

public:
    VideoStreamWebm();

    virtual Ref<VideoStreamPlayback> instance_playback();

    virtual void set_file(const String& p_file);
    String get_file();
    virtual void set_audio_track(int p_track);
};

class ResourceFormatLoaderWebm : public ResourceFormatLoader {
public:
    virtual RES load(
        const String& p_path,
        const String& p_original_path = "",
        Error* r_error                = nullptr
    );
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual bool handles_type(const String& p_type) const;
    virtual String get_resource_type(const String& p_path) const;
};

#endif // VIDEO_STREAM_WEBM_H
