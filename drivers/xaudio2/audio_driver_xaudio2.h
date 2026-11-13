// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_DRIVER_XAUDIO2_H
#define AUDIO_DRIVER_XAUDIO2_H

#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "servers/audio_server.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// Windows system includes come after <windows.h>
#include <mmsystem.h>
#include <wrl/client.h>
#include <xaudio2.h>

class AudioDriverXAudio2 : public AudioDriver {
    enum {
        AUDIO_BUFFERS = 2
    };

    struct XAudio2DriverVoiceCallback : public IXAudio2VoiceCallback {
        HANDLE buffer_end_event;

        XAudio2DriverVoiceCallback() :
            buffer_end_event(CreateEvent(NULL, FALSE, FALSE, NULL)) {}

        void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) {
            SetEvent(buffer_end_event);
        }

        // Unused methods are stubs
        void STDMETHODCALLTYPE OnStreamEnd() {}

        void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}

        void STDMETHODCALLTYPE
        OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}

        void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {}

        void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) {}

        void STDMETHODCALLTYPE
        OnVoiceError(void* pBufferContext, HRESULT Error) {}
    };

    Thread thread;
    Mutex mutex;

    int32_t* samples_in;
    int16_t* samples_out[AUDIO_BUFFERS];

    static void thread_func(void* p_udata);
    int buffer_size;

    unsigned int mix_rate;
    SpeakerMode speaker_mode;

    int channels;

    bool active;
    bool thread_exited;
    mutable bool exit_thread;
    bool pcm_open;

    WAVEFORMATEX wave_format;
    Microsoft::WRL::ComPtr<IXAudio2> xaudio;
    int current_buffer;
    IXAudio2MasteringVoice* mastering_voice;
    XAUDIO2_BUFFER xaudio_buffer[AUDIO_BUFFERS];
    IXAudio2SourceVoice* source_voice;
    XAudio2DriverVoiceCallback voice_callback;

public:
    const char* get_name() const override;

    Error init() override;
    void start() override;
    int get_mix_rate() const override;
    SpeakerMode get_speaker_mode() const override;
    float get_latency() override;
    void lock() override;
    void unlock() override;
    void finish() override;

    AudioDriverXAudio2();
    ~AudioDriverXAudio2() override;
};

#endif
