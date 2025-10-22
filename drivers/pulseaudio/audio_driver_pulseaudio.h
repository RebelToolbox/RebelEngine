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
public:
    Error init() override;
    void start() override;

    const char* get_name() const override;

    String get_device() override;
    void set_device(String device_name) override;
    Array get_device_list() override;

    String capture_get_device() override;
    void capture_set_device(const String& device_name) override;
    Array capture_get_device_list() override;

    float get_latency() override;
    int get_mix_rate() const override;
    SpeakerMode get_speaker_mode() const override;

    Error capture_start() override;
    Error capture_stop() override;

    void lock() override;
    void unlock() override;
    void finish() override;

private:
    Mutex mutex;
    Thread thread;

    Vector<int32_t> internal_buffer;
    Vector<int16_t> output_buffer;

    Array output_devices;
    Array input_devices;

    String output_device_name{"Default"};
    String new_output_device_name{"Default"};
    String default_output_device;
    String input_device_name;
    String new_input_device_name;
    String default_input_device;

    pa_channel_map output_channel_map = {};
    pa_channel_map input_channel_map  = {};
    pa_mainloop* mainloop             = nullptr;
    pa_context* context               = nullptr;
    pa_stream* output_stream          = nullptr;
    pa_stream* input_stream           = nullptr;

    size_t available_bytes = 0;
    size_t write_offset    = 0;
    int channels           = 0;
    int mix_rate           = 0;
    int pa_ready           = 0;
    int pa_status          = 0;
    float latency          = 0.f;

    bool active        = false;
    bool thread_exited = false;
    bool exit_thread   = false;

    Error initialize_output_device();
    Error initialize_input_device();

    Error detect_output_channels();
    Error detect_input_channels();

    void disconnect_output_device();
    void disconnect_input_device();

    Error get_default_device_names();

    size_t playback_sound();
    void get_output_bytes();
    void clear_output_buffer();
    void fill_internal_buffer();
    void copy_internal_buffer_to_output_buffer();
    void apply_pulse_audio_updates() const;
    size_t stream_output_bytes();
    size_t record_sound();
    Error change_output_device();
    Error change_input_device();
    Error check_default_device();

    static void thread_function(void* user_data);

    static void context_state_callback(pa_context* context, void* user_data);
    static void get_server_info_callback(
        pa_context* context,
        const pa_server_info* server_info,
        void* user_data
    );
    static void get_sink_info_list_callback(
        pa_context* context,
        const pa_sink_info* sink_info,
        int eol,
        void* user_data
    );
    static void get_source_info_list_callback(
        pa_context* context,
        const pa_source_info* source_info,
        int eol,
        void* user_data
    );
    static void get_sink_info_by_name_callback(
        pa_context* context,
        const pa_sink_info* sink_info,
        int eol,
        void* user_data
    );
    static void get_source_info_by_name_callback(
        pa_context* context,
        const pa_source_info* source_info,
        int eol,
        void* user_data
    );
};

#endif // PULSEAUDIO_ENABLED

#endif // AUDIO_DRIVER_PULSEAUDIO_H
