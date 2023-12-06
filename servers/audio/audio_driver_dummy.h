// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_DRIVER_DUMMY_H
#define AUDIO_DRIVER_DUMMY_H

#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "servers/audio_server.h"

class AudioDriverDummy : public AudioDriver {
    Thread thread;
    Mutex mutex;

    int32_t* samples_in;

    static void thread_func(void* p_udata);

    unsigned int buffer_frames;
    unsigned int mix_rate;
    SpeakerMode speaker_mode;

    int channels;

    bool active;
    bool thread_exited;
    mutable bool exit_thread;

public:
    const char* get_name() const {
        return "Dummy";
    };

    virtual Error init();
    virtual void start();
    virtual int get_mix_rate() const;
    virtual SpeakerMode get_speaker_mode() const;
    virtual void lock();
    virtual void unlock();
    virtual void finish();

    AudioDriverDummy();
    ~AudioDriverDummy();
};

#endif
