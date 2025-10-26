// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_DRIVER_ALSA_H
#define AUDIO_DRIVER_ALSA_H

#ifdef ALSA_ENABLED

#include "asound-so_wrap.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "servers/audio_server.h"

class AudioDriverALSA : public AudioDriver {
public:
    Error init() override;
    void start() override;

    const char* get_name() const override;

    String get_device() override;
    void set_device(String new_name) override;
    Array get_device_list() override;

    int get_mix_rate() const override;
    SpeakerMode get_speaker_mode() const override;

    void lock() override;
    void unlock() override;
    void finish() override;

private:
    Mutex mutex;
    Thread thread;

    Vector<int32_t> internal_buffer;
    Vector<int16_t> output_buffer;

    String device_name{"Default"};
    String new_device_name{"Default"};

    snd_pcm_t* pcm_handle = nullptr;
    int buffer_frames     = 0;
    int buffer_size       = 0;
    int period_size       = 0;
    int channels          = 0;
    int mix_rate          = 0;

    bool active         = false;
    bool thread_running = false;

    Error initialize_device();
    Error initialize_error(int status, const String& message);
    void disconnect_device();

    void playback_sound();
    void get_output_bytes();
    void clear_output_buffer();
    void fill_internal_buffer();
    void copy_internal_buffer_to_output_buffer();
    void write_output_bytes();
    Error change_output_device();

    static void thread_function(void* user_data);
};

#endif // ALSA_ENABLED

#endif // AUDIO_DRIVER_ALSA_H
