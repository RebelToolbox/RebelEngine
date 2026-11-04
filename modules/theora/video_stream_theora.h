// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VIDEO_STREAM_THEORA_H
#define VIDEO_STREAM_THEORA_H

#include "core/io/resource_loader.h"
#include "core/os/file_access.h"
#include "core/os/semaphore.h"
#include "core/os/thread.h"
#include "core/ring_buffer.h"
#include "core/safe_refcount.h"
#include "scene/resources/video_stream.h"
#include "servers/audio_server.h"

#include <theora/theoradec.h>
#include <vorbis/codec.h>

// #define THEORA_USE_THREAD_STREAMING

class VideoStreamPlaybackTheora : public VideoStreamPlayback {
    GDCLASS(VideoStreamPlaybackTheora, VideoStreamPlayback);

    enum {
        MAX_FRAMES = 4,
    };

    // Image frames[MAX_FRAMES];
    Image::Format format;
    PoolVector<uint8_t> frame_data;
    int frames_pending;
    FileAccess* file;
    String file_name;
    int audio_frames_wrote;
    Point2i size;

    int buffer_data();
    int queue_page(ogg_page* page);
    void video_write();
    float get_time() const;

    bool theora_eos;
    bool vorbis_eos;

    ogg_sync_state oy;
    ogg_page og;
    ogg_stream_state vo;
    ogg_stream_state to;
    th_info ti;
    th_comment tc;
    th_dec_ctx* td;
    vorbis_info vi;
    vorbis_dsp_state vd;
    vorbis_block vb;
    vorbis_comment vc;
    th_pixel_fmt px_fmt;
    double videobuf_time;
    int pp_inc;

    int theora_p;
    int vorbis_p;
    int pp_level_max;
    int pp_level;
    int videobuf_ready;

    bool playing;
    bool buffering;

    double last_update_time;
    double time;
    double delay_compensation;

    Ref<ImageTexture> texture;

    AudioMixCallback mix_callback;
    void* mix_udata;
    bool paused;

#ifdef THEORA_USE_THREAD_STREAMING

    enum {
        RB_SIZE_KB = 1024
    };

    RingBuffer<uint8_t> ring_buffer;
    Vector<uint8_t> read_buffer;
    bool thread_eof;
    Semaphore thread_sem;
    Thread thread;
    SafeFlag thread_exit;

    static void _streaming_thread(void* ud);

#endif

    int audio_track;

protected:
    void clear();

public:
    void play() override;
    void stop() override;
    bool is_playing() const override;

    void set_paused(bool p_paused) override;
    bool is_paused() const override;

    void set_loop(bool p_enable) override;
    bool has_loop() const override;

    float get_length() const override;

    String get_stream_name() const;

    int get_loop_count() const;

    float get_playback_position() const override;
    void seek(float p_time) override;

    void set_file(const String& p_file);

    Ref<Texture> get_texture() const override;
    void update(float p_delta) override;

    void set_mix_callback(
        AudioMixCallback p_callback,
        void* p_userdata
    ) override;
    int get_channels() const override;
    int get_mix_rate() const override;

    void set_audio_track(int p_idx) override;

    VideoStreamPlaybackTheora();
    ~VideoStreamPlaybackTheora() override;
};

class VideoStreamTheora : public VideoStream {
    GDCLASS(VideoStreamTheora, VideoStream);

    String file;
    int audio_track;

protected:
    static void _bind_methods();

public:
    Ref<VideoStreamPlayback> instance_playback() override {
        Ref<VideoStreamPlaybackTheora> pb = memnew(VideoStreamPlaybackTheora);
        pb->set_audio_track(audio_track);
        pb->set_file(file);
        return pb;
    }

    void set_file(const String& p_file) {
        file = p_file;
    }

    String get_file() {
        return file;
    }

    void set_audio_track(int p_track) override {
        audio_track = p_track;
    }

    VideoStreamTheora() {
        audio_track = 0;
    }
};

class ResourceFormatLoaderTheora : public ResourceFormatLoader {
public:
    RES load(
        const String& p_path,
        const String& p_original_path = "",
        Error* r_error                = nullptr
    ) override;
    void get_recognized_extensions(List<String>* p_extensions) const override;
    bool handles_type(const String& p_type) const override;
    String get_resource_type(const String& p_path) const override;
};

#endif // VIDEO_STREAM_THEORA_H
