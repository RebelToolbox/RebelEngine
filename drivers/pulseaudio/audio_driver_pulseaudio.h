// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_DRIVER_PULSEAUDIO_H
#define AUDIO_DRIVER_PULSEAUDIO_H

#ifdef PULSEAUDIO_ENABLED

#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "pulse-so_wrap.h"
#include "servers/audio_server.h"

class AudioDriverPulseAudio : public AudioDriver {
    Thread thread;
    Mutex mutex;

    pa_mainloop* pa_ml;
    pa_context* pa_ctx;
    pa_stream* pa_str;
    pa_stream* pa_rec_str;
    pa_channel_map pa_map;
    pa_channel_map pa_rec_map;

    String device_name;
    String new_device;
    String default_device;

    String capture_device_name;
    String capture_new_device;
    String capture_default_device;

    Vector<int32_t> samples_in;
    Vector<int16_t> samples_out;

    unsigned int mix_rate;
    unsigned int buffer_frames;
    unsigned int pa_buffer_size;
    int channels;
    int pa_ready;
    int pa_status;
    Array pa_devices;
    Array pa_rec_devices;

    bool active;
    bool thread_exited;
    mutable bool exit_thread;

    float latency;

    static void pa_state_cb(pa_context* c, void* userdata);
    static void pa_sink_info_cb(
        pa_context* c,
        const pa_sink_info* l,
        int eol,
        void* userdata
    );
    static void pa_source_info_cb(
        pa_context* c,
        const pa_source_info* l,
        int eol,
        void* userdata
    );
    static void pa_server_info_cb(
        pa_context* c,
        const pa_server_info* i,
        void* userdata
    );
    static void pa_sinklist_cb(
        pa_context* c,
        const pa_sink_info* l,
        int eol,
        void* userdata
    );
    static void pa_sourcelist_cb(
        pa_context* c,
        const pa_source_info* l,
        int eol,
        void* userdata
    );

    Error init_device();
    void finish_device();

    Error capture_init_device();
    void capture_finish_device();

    Error detect_channels(bool capture = false);

    static void thread_func(void* p_udata);

public:
    const char* get_name() const {
        return "PulseAudio";
    };

    virtual Error init();
    virtual void start();
    virtual int get_mix_rate() const;
    virtual SpeakerMode get_speaker_mode() const;

    virtual Array get_device_list();
    virtual String get_device();
    virtual void set_device(String device);

    virtual Array capture_get_device_list();
    virtual void capture_set_device(const String& p_name);
    virtual String capture_get_device();

    virtual void lock();
    virtual void unlock();
    virtual void finish();

    virtual float get_latency();

    virtual Error capture_start();
    virtual Error capture_stop();

    AudioDriverPulseAudio();
    ~AudioDriverPulseAudio();
};

#endif // PULSEAUDIO_ENABLED

#endif // AUDIO_DRIVER_PULSEAUDIO_H
