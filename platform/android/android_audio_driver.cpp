// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_audio_driver.h"

#include <string.h>

#define MAX_NUMBER_INTERFACES     3
#define MAX_NUMBER_OUTPUT_DEVICES 6

/* Structure for passing information to callback function */

void AndroidAudioDriver::_buffer_callback(SLAndroidSimpleBufferQueueItf queueItf
) {
    bool mix = true;

    if (pause) {
        mix = false;
    } else {
        mix = mutex.try_lock() == OK;
    }

    if (mix) {
        audio_server_process(buffer_size, mixdown_buffer);
    } else {
        int32_t* src_buff = mixdown_buffer;
        for (unsigned int i = 0; i < buffer_size * 2; i++) {
            src_buff[i] = 0;
        }
    }

    if (mix) {
        mutex.unlock();
    }

    const int32_t* src_buff = mixdown_buffer;

    int16_t* ptr = (int16_t*)buffers[last_free];
    last_free    = (last_free + 1) % BUFFER_COUNT;

    for (unsigned int i = 0; i < buffer_size * 2; i++) {
        ptr[i] = src_buff[i] >> 16;
    }

    (*queueItf)->Enqueue(queueItf, ptr, 4 * buffer_size);
}

void AndroidAudioDriver::_buffer_callbacks(
    SLAndroidSimpleBufferQueueItf queueItf,
    void* pContext
) {
    AndroidAudioDriver* ad = (AndroidAudioDriver*)pContext;

    ad->_buffer_callback(queueItf);
}

const char* AndroidAudioDriver::get_name() const {
    return "Android";
}

Error AndroidAudioDriver::init() {
    SLresult res;
    SLEngineOption EngineOption[] = {
        {(SLuint32)SL_ENGINEOPTION_THREADSAFE, (SLuint32)SL_BOOLEAN_TRUE}
    };
    res = slCreateEngine(&sl, 1, EngineOption, 0, NULL, NULL);
    ERR_FAIL_COND_V_MSG(
        res != SL_RESULT_SUCCESS,
        ERR_INVALID_PARAMETER,
        "Could not initialize OpenSL."
    );

    res = (*sl)->Realize(sl, SL_BOOLEAN_FALSE);
    ERR_FAIL_COND_V_MSG(
        res != SL_RESULT_SUCCESS,
        ERR_INVALID_PARAMETER,
        "Could not realize OpenSL."
    );

    return OK;
}

void AndroidAudioDriver::start() {
    active = false;

    SLresult res;

    buffer_size = 1024;

    for (int i = 0; i < BUFFER_COUNT; i++) {
        buffers[i] = memnew_arr(int16_t, buffer_size * 2);
        memset(buffers[i], 0, buffer_size * 4);
    }

    mixdown_buffer = memnew_arr(int32_t, buffer_size * 2);

    /* Callback context for the buffer queue callback function */

    /* Get the SL Engine Interface which is implicit */
    res = (*sl)->GetInterface(sl, SL_IID_ENGINE, (void*)&EngineItf);

    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);

    {
        const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
        const SLboolean req[1]     = {SL_BOOLEAN_FALSE};
        res = (*EngineItf)->CreateOutputMix(EngineItf, &OutputMix, 0, ids, req);
    }

    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);
    // Realizing the Output Mix object in synchronous mode.
    res = (*OutputMix)->Realize(OutputMix, SL_BOOLEAN_FALSE);
    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);

    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
        SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
        BUFFER_COUNT
    };
    // bufferQueue.locatorType = SL_DATALOCATOR_BUFFERQUEUE;
    // bufferQueue.numBuffers = BUFFER_COUNT; /* Four buffers in our buffer
    // queue */
    /* Setup the format of the content in the buffer queue */
    pcm.formatType    = SL_DATAFORMAT_PCM;
    pcm.numChannels   = 2;
    pcm.samplesPerSec = SL_SAMPLINGRATE_44_1;
    pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    pcm.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
    pcm.channelMask   = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
#ifdef BIG_ENDIAN_ENABLED
    pcm.endianness = SL_BYTEORDER_BIGENDIAN;
#else
    pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
#endif
    audioSource.pFormat  = (void*)&pcm;
    audioSource.pLocator = (void*)&loc_bufq;

    /* Setup the data sink structure */
    locator_outputmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    locator_outputmix.outputMix   = OutputMix;
    audioSink.pLocator            = (void*)&locator_outputmix;
    audioSink.pFormat             = NULL;
    /* Initialize the context for Buffer queue callbacks */
    // cntxt.pDataBase = (void*)&pcmData;
    // cntxt.pData = cntxt.pDataBase;
    // cntxt.size = sizeof(pcmData);

    /* Create the music player */

    {
        const SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND};
        const SLboolean req[2]     = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

        res = (*EngineItf)
                  ->CreateAudioPlayer(
                      EngineItf,
                      &player,
                      &audioSource,
                      &audioSink,
                      1,
                      ids,
                      req
                  );
        ERR_FAIL_COND(res != SL_RESULT_SUCCESS);
    }
    /* Realizing the player in synchronous mode. */
    res = (*player)->Realize(player, SL_BOOLEAN_FALSE);
    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);
    /* Get seek and play interfaces */
    res = (*player)->GetInterface(player, SL_IID_PLAY, (void*)&playItf);
    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);
    res = (*player)->GetInterface(
        player,
        SL_IID_BUFFERQUEUE,
        (void*)&bufferQueueItf
    );
    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);
    /* Setup to receive buffer queue event callbacks */
    res = (*bufferQueueItf)
              ->RegisterCallback(bufferQueueItf, _buffer_callbacks, this);
    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);

    last_free = 0;

    // fill up buffers
    for (int i = 0; i < BUFFER_COUNT; i++) {
        /* Enqueue a few buffers to get the ball rolling */
        res = (*bufferQueueItf)
                  ->Enqueue(
                      bufferQueueItf,
                      buffers[i],
                      4 * buffer_size
                  ); /* Size given in */
    }

    res = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);
    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);

    active = true;
}

void AndroidAudioDriver::_record_buffer_callback(
    SLAndroidSimpleBufferQueueItf queueItf
) {
    for (int i = 0; i < rec_buffer.size(); i++) {
        int32_t sample = rec_buffer[i] << 16;
        input_buffer_write(sample);
        input_buffer_write(sample); // call twice to convert to Stereo
    }

    SLresult res = (*recordBufferQueueItf)
                       ->Enqueue(
                           recordBufferQueueItf,
                           rec_buffer.ptrw(),
                           rec_buffer.size() * sizeof(int16_t)
                       );
    ERR_FAIL_COND(res != SL_RESULT_SUCCESS);
}

void AndroidAudioDriver::_record_buffer_callbacks(
    SLAndroidSimpleBufferQueueItf queueItf,
    void* pContext
) {
    AndroidAudioDriver* ad = (AndroidAudioDriver*)pContext;

    ad->_record_buffer_callback(queueItf);
}

Error AndroidAudioDriver::capture_init_device() {
    SLDataLocator_IODevice loc_dev = {
        SL_DATALOCATOR_IODEVICE,
        SL_IODEVICE_AUDIOINPUT,
        SL_DEFAULTDEVICEID_AUDIOINPUT,
        NULL
    };
    SLDataSource recSource = {&loc_dev, NULL};

    SLDataLocator_AndroidSimpleBufferQueue loc_bq = {
        SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
        2
    };
    SLDataFormat_PCM format_pcm = {
        SL_DATAFORMAT_PCM,
        1,
        SL_SAMPLINGRATE_44_1,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER,
        SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSink recSnk = {&loc_bq, &format_pcm};

    const SLInterfaceID ids[2] = {
        SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
        SL_IID_ANDROIDCONFIGURATION
    };
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    SLresult res = (*EngineItf)
                       ->CreateAudioRecorder(
                           EngineItf,
                           &recorder,
                           &recSource,
                           &recSnk,
                           2,
                           ids,
                           req
                       );
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    res = (*recorder)->Realize(recorder, SL_BOOLEAN_FALSE);
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    res = (*recorder)->GetInterface(recorder, SL_IID_RECORD, (void*)&recordItf);
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    res = (*recorder)->GetInterface(
        recorder,
        SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
        (void*)&recordBufferQueueItf
    );
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    res = (*recordBufferQueueItf)
              ->RegisterCallback(
                  recordBufferQueueItf,
                  _record_buffer_callbacks,
                  this
              );
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    SLuint32 state;
    res = (*recordItf)->GetRecordState(recordItf, &state);
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    if (state != SL_RECORDSTATE_STOPPED) {
        res = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);
        ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

        res = (*recordBufferQueueItf)->Clear(recordBufferQueueItf);
        ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);
    }

    const int rec_buffer_frames = 2048;
    rec_buffer.resize(rec_buffer_frames);
    input_buffer_init(rec_buffer_frames);

    res = (*recordBufferQueueItf)
              ->Enqueue(
                  recordBufferQueueItf,
                  rec_buffer.ptrw(),
                  rec_buffer.size() * sizeof(int16_t)
              );
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    res = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_RECORDING);
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    return OK;
}

Error AndroidAudioDriver::capture_start() {
    if (OS::get_singleton()->request_permission("RECORD_AUDIO")) {
        return capture_init_device();
    }

    return OK;
}

Error AndroidAudioDriver::capture_stop() {
    SLuint32 state;
    SLresult res = (*recordItf)->GetRecordState(recordItf, &state);
    ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

    if (state != SL_RECORDSTATE_STOPPED) {
        res = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);
        ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);

        res = (*recordBufferQueueItf)->Clear(recordBufferQueueItf);
        ERR_FAIL_COND_V(res != SL_RESULT_SUCCESS, ERR_CANT_OPEN);
    }

    return OK;
}

int AndroidAudioDriver::get_mix_rate() const {
    return 44100; // hardcoded for Android, as selected by SL_SAMPLINGRATE_44_1
}

AudioDriver::SpeakerMode AndroidAudioDriver::get_speaker_mode() const {
    return SPEAKER_MODE_STEREO;
}

void AndroidAudioDriver::lock() {
    if (active) {
        mutex.lock();
    }
}

void AndroidAudioDriver::unlock() {
    if (active) {
        mutex.unlock();
    }
}

void AndroidAudioDriver::finish() {
    (*sl)->Destroy(sl);
}

void AndroidAudioDriver::set_pause(bool p_pause) {
    pause = p_pause;

    if (active) {
        if (pause) {
            (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PAUSED);
        } else {
            (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);
        }
    }
}

AndroidAudioDriver::AndroidAudioDriver() {
    pause  = false;
    active = false;
}
