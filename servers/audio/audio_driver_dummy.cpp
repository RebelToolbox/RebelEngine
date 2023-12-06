// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "audio_driver_dummy.h"

#include "core/os/os.h"
#include "core/project_settings.h"

Error AudioDriverDummy::init() {
    active        = false;
    thread_exited = false;
    exit_thread   = false;
    samples_in    = nullptr;

    mix_rate     = GLOBAL_GET("audio/mix_rate");
    speaker_mode = SPEAKER_MODE_STEREO;
    channels     = 2;

    int latency   = GLOBAL_GET("audio/output_latency");
    buffer_frames = closest_power_of_2(latency * mix_rate / 1000);

    samples_in = memnew_arr(int32_t, buffer_frames * channels);

    thread.start(AudioDriverDummy::thread_func, this);

    return OK;
};

void AudioDriverDummy::thread_func(void* p_udata) {
    AudioDriverDummy* ad = (AudioDriverDummy*)p_udata;

    uint64_t usdelay = (ad->buffer_frames / float(ad->mix_rate)) * 1000000;

    while (!ad->exit_thread) {
        if (ad->active) {
            ad->lock();

            ad->audio_server_process(ad->buffer_frames, ad->samples_in);

            ad->unlock();
        };

        OS::get_singleton()->delay_usec(usdelay);
    };

    ad->thread_exited = true;
};

void AudioDriverDummy::start() {
    active = true;
};

int AudioDriverDummy::get_mix_rate() const {
    return mix_rate;
};

AudioDriver::SpeakerMode AudioDriverDummy::get_speaker_mode() const {
    return speaker_mode;
};

void AudioDriverDummy::lock() {
    mutex.lock();
};

void AudioDriverDummy::unlock() {
    mutex.unlock();
};

void AudioDriverDummy::finish() {
    exit_thread = true;
    thread.wait_to_finish();

    if (samples_in) {
        memdelete_arr(samples_in);
    };
};

AudioDriverDummy::AudioDriverDummy(){

};

AudioDriverDummy::~AudioDriverDummy(){

};
