// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef OPENSL_AUDIO_DRIVER_H
#define OPENSL_AUDIO_DRIVER_H

#include "core/os/mutex.h"
#include "servers/audio_server.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class AndroidAudioDriver : public AudioDriver {
    bool active;
    Mutex mutex;

    enum {
        BUFFER_COUNT = 2
    };

    bool pause;

    uint32_t buffer_size;
    int16_t* buffers[BUFFER_COUNT];
    int32_t* mixdown_buffer;
    int last_free;

    Vector<int16_t> rec_buffer;

    SLPlayItf playItf;
    SLRecordItf recordItf;
    SLObjectItf sl;
    SLEngineItf EngineItf;
    SLObjectItf OutputMix;
    SLVolumeItf volumeItf;
    SLObjectItf player;
    SLObjectItf recorder;
    SLAndroidSimpleBufferQueueItf bufferQueueItf;
    SLAndroidSimpleBufferQueueItf recordBufferQueueItf;
    SLDataSource audioSource;
    SLDataFormat_PCM pcm;
    SLDataSink audioSink;
    SLDataLocator_OutputMix locator_outputmix;
    SLBufferQueueState state;

    void _buffer_callback(SLAndroidSimpleBufferQueueItf queueItf);

    static void _buffer_callbacks(
        SLAndroidSimpleBufferQueueItf queueItf,
        void* pContext
    );

    void _record_buffer_callback(SLAndroidSimpleBufferQueueItf queueItf);

    static void _record_buffer_callbacks(
        SLAndroidSimpleBufferQueueItf queueItf,
        void* pContext
    );

    virtual Error capture_init_device();

public:
    void set_singleton();

    virtual const char* get_name() const;

    virtual Error init();
    virtual void start();
    virtual int get_mix_rate() const;
    virtual SpeakerMode get_speaker_mode() const;
    virtual void lock();
    virtual void unlock();
    virtual void finish();

    virtual void set_pause(bool p_pause);

    virtual Error capture_start();
    virtual Error capture_stop();

    AndroidAudioDriver();
};

#endif // OPENSL_AUDIO_DRIVER_H
