// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "audio_driver_alsa.h"

#ifdef ALSA_ENABLED

#include "core/os/os.h"
#include "core/project_settings.h"

#include <cerrno>

#ifdef PULSEAUDIO_ENABLED
extern "C" {
extern int initialize_pulse(int verbose);
}
#endif

Error AudioDriverALSA::init() {
#ifdef DEBUG_ENABLED
    int dylibloader_verbose = 1;
#else
    int dylibloader_verbose = 0;
#endif
#ifdef PULSEAUDIO_ENABLED
    // On pulse enabled systems Alsa will silently use pulse.
    // If pulse is not installed, this will fail silently.
    initialize_pulse(dylibloader_verbose);
#endif

    if (initialize_asound(dylibloader_verbose)) {
        return ERR_CANT_OPEN;
    }

    active         = false;
    thread_running = false;

    const Error error = initialize_device();
    if (error != OK) {
        return error;
    }

    thread.start(thread_function, this);
    return OK;
}

void AudioDriverALSA::start() {
    active = true;
}

const char* AudioDriverALSA::get_name() const {
    return "ALSA";
}

String AudioDriverALSA::get_device() {
    lock();
    String name = device_name;
    unlock();
    return name;
}

void AudioDriverALSA::set_device(const String new_name) {
    lock();
    new_device_name = new_name;
    unlock();
}

Array AudioDriverALSA::get_device_list() {
    Array list;
    list.push_back("Default");
    void** hints;
    const int error_code = snd_device_name_hint(-1, "pcm", &hints);
    if (error_code < 0) {
        ERR_PRINT(
            "ALSA: Couldn't retrieve device name hints: "
            + String(snd_strerror(error_code))
        );
        return list;
    }
    for (void** hint = hints; *hint != nullptr; hint++) {
        char* name = snd_device_name_get_hint(*hint, "NAME");
        char* desc = snd_device_name_get_hint(*hint, "DESC");
        if (name) {
            if (strncmp(name, "plughw", 6) != 0) {
                continue;
            }
            if (desc) {
                list.push_back(String::utf8(name) + ";" + String::utf8(desc));
            } else {
                list.push_back(String::utf8(name));
            }
        }
        if (desc) {
            free(desc);
        }
        if (name) {
            free(name);
        }
    }
    snd_device_name_free_hint(hints);
    return list;
}

int AudioDriverALSA::get_mix_rate() const {
    return mix_rate;
}

AudioDriver::SpeakerMode AudioDriverALSA::get_speaker_mode() const {
    return SPEAKER_MODE_STEREO;
}

void AudioDriverALSA::lock() {
    mutex.lock();
}

void AudioDriverALSA::unlock() {
    mutex.unlock();
}

void AudioDriverALSA::finish() {
    thread_running = true;
    thread.wait_to_finish();
    disconnect_device();
}

Error AudioDriverALSA::initialize_device() {
    channels              = 2;
    // TODO: Add:
    // 4 channels: "plug:surround40"
    // 6 channels: "plug:surround51"
    mix_rate              = GLOBAL_GET("audio/mix_rate");
    // In ALSA the period size appears to determine the actual latency.
    // Ref: https://www.alsa-project.org/main/index.php/FramesPeriods
    constexpr int periods = 2;
    const int latency     = GLOBAL_GET("audio/output_latency");
    buffer_frames =
        static_cast<int>(closest_power_of_2(latency * mix_rate / 1000));
    buffer_size = buffer_frames * periods;
    period_size = buffer_frames;

    if (device_name != "Default") {
        // Check if the specified device exists.
        const Array list = get_device_list();
        if (list.find(device_name) == -1) {
            device_name     = "Default";
            new_device_name = "Default";
        }
    }

    auto name = "default";
    if (device_name != "Default") {
        const int position = device_name.find(";");
        name               = device_name.substr(0, position).utf8().get_data();
    }
    int status = snd_pcm_open(
        &pcm_handle,
        name,
        SND_PCM_STREAM_PLAYBACK,
        SND_PCM_NONBLOCK
    );
    ERR_FAIL_COND_V_MSG(
        status < 0,
        ERR_CANT_OPEN,
        "ALSA: Couldn't open a PCM: " + String(snd_strerror(status))
    );

    // Set hardware parameters.
    // Reference:
    // https://www.alsa-project.org/alsa-doc/alsa-lib/group___p_c_m___h_w___params.html

    snd_pcm_hw_params_t* hw_params;
    snd_pcm_hw_params_alloca(&hw_params);
    status = snd_pcm_hw_params_any(pcm_handle, hw_params);
    if (status < 0) {
        return initialize_error(
            status,
            "Couldn't retrieve a configuration space for a PCM"
        );
    }

    status = snd_pcm_hw_params_set_access(
        pcm_handle,
        hw_params,
        SND_PCM_ACCESS_RW_INTERLEAVED
    );
    if (status < 0) {
        return initialize_error(status, "Couldn't set access type");
    }

    status = snd_pcm_hw_params_set_format(
        pcm_handle,
        hw_params,
        SND_PCM_FORMAT_S16_LE
    );
    if (status < 0) {
        return initialize_error(status, "Couldn't set format");
    }

    status = snd_pcm_hw_params_set_channels(pcm_handle, hw_params, 2);
    if (status < 0) {
        return initialize_error(status, "Couldn't set channel count");
    }

    unsigned int rate_value = mix_rate;
    status                  = snd_pcm_hw_params_set_rate_near(
        pcm_handle,
        hw_params,
        &rate_value,
        nullptr
    );
    if (status < 0) {
        return initialize_error(status, "Couldn't set rate");
    }
    mix_rate = static_cast<int>(rate_value);
    print_verbose("ALSA: Mix rate set to " + itos(mix_rate));

    snd_pcm_uframes_t buffer_size_value = buffer_size;
    status = snd_pcm_hw_params_set_buffer_size_near(
        pcm_handle,
        hw_params,
        &buffer_size_value
    );
    if (status < 0) {
        return initialize_error(status, "Couldn't set buffer size");
    }
    buffer_size = static_cast<int>(buffer_size_value);
    print_verbose("ALSA: Buffer size set to " + itos(buffer_size));

    snd_pcm_uframes_t period_size_value = period_size;
    status = snd_pcm_hw_params_set_period_size_near(
        pcm_handle,
        hw_params,
        &period_size_value,
        nullptr
    );
    if (status < 0) {
        return initialize_error(status, "Couldn't set period size");
    }
    period_size = static_cast<int>(period_size_value);
    print_verbose("ALSA: Period size set to " + itos(period_size));

    unsigned int periods_value = periods;
    status                     = snd_pcm_hw_params_set_periods_near(
        pcm_handle,
        hw_params,
        &periods_value,
        nullptr
    );
    if (status < 0) {
        return initialize_error(status, "Couldn't set period count");
    }
    print_verbose("ALSA: Period count set to : " + itos(periods));

    status = snd_pcm_hw_params(pcm_handle, hw_params);
    if (status < 0) {
        return initialize_error(
            status,
            "Couldn't install hardware configuration"
        );
    }

    // Set software parameters.
    // Reference:
    // https://www.alsa-project.org/alsa-doc/alsa-lib/group___p_c_m___s_w___params.html

    snd_pcm_sw_params_t* sw_params;
    snd_pcm_sw_params_alloca(&sw_params);

    status = snd_pcm_sw_params_current(pcm_handle, sw_params);
    if (status < 0) {
        return initialize_error(
            status,
            "Couldn't retrieve the current software configuration"
        );
    }

    status =
        snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, period_size);
    if (status < 0) {
        return initialize_error(
            status,
            "Couldn't set minimum available frames for PCM to be considered "
            "ready"
        );
    }

    status = snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, 1);
    if (status < 0) {
        return initialize_error(
            status,
            "Couldn't set threshold frames to start playback"
        );
    }

    status = snd_pcm_sw_params(pcm_handle, sw_params);
    if (status < 0) {
        return initialize_error(
            status,
            "Couldn't install software configuration"
        );
    }

    internal_buffer.resize(period_size * channels);
    output_buffer.resize(period_size * channels);

    return OK;
}

Error AudioDriverALSA::initialize_error(
    const int status,
    const String& message
) {
    ERR_PRINT("ALSA: " + message + ": " + snd_strerror(status));
    snd_pcm_close(pcm_handle);
    pcm_handle = nullptr;
    return ERR_CANT_OPEN;
}

void AudioDriverALSA::disconnect_device() {
    if (pcm_handle) {
        snd_pcm_close(pcm_handle);
        pcm_handle = nullptr;
    }
}

void AudioDriverALSA::playback_sound() {
    get_output_bytes();
    write_output_bytes();
}

void AudioDriverALSA::get_output_bytes() {
    if (!active) {
        clear_output_buffer();
    } else {
        fill_internal_buffer();
        copy_internal_buffer_to_output_buffer();
    }
}

void AudioDriverALSA::clear_output_buffer() {
    for (int i = 0; i < output_buffer.size(); i++) {
        output_buffer.write[i] = 0;
    }
}

void AudioDriverALSA::fill_internal_buffer() {
    audio_server_process(period_size, internal_buffer.ptrw());
}

void AudioDriverALSA::copy_internal_buffer_to_output_buffer() {
    for (int i = 0; i < output_buffer.size(); i++) {
        output_buffer.write[i] = static_cast<int16_t>(internal_buffer[i] >> 16);
    }
}

void AudioDriverALSA::write_output_bytes() {
    int available_bytes = period_size;
    int bytes_written   = 0;

    while (available_bytes && !thread_running) {
        const auto data                = output_buffer.ptr();
        const int actual_bytes_written = snd_pcm_writei(
            pcm_handle,
            data + bytes_written * channels,
            available_bytes
        );
        if (actual_bytes_written < 0) {
            // Write failed.
            int error_number = -actual_bytes_written;
            if (error_number == EAGAIN) {
                // Device's buffer is full. Try again later.
                stop_counting_ticks();
                unlock();
                OS::get_singleton()->delay_usec(1000);
                lock();
                start_counting_ticks();
            } else {
                // Try to recover.
                error_number = snd_pcm_recover(pcm_handle, -error_number, 0);
                if (error_number < 0) {
                    ERR_PRINT(
                        "ALSA: Failed to write to device and cannot recover: "
                        + String(snd_strerror(error_number))
                    );
                    active         = false;
                    thread_running = true;
                }
            }
        } else {
            available_bytes -= actual_bytes_written;
            bytes_written   += actual_bytes_written;
        }
    }
}

Error AudioDriverALSA::change_output_device() {
    disconnect_device();
    device_name = new_device_name;

    Error error = initialize_device();
    if (error != OK) {
        ERR_PRINT("ALSA: Failed to initialize the output device!");
        device_name     = "Default";
        new_device_name = "Default";
        error           = initialize_device();
        if (error != OK) {
            active         = false;
            thread_running = true;
        }
    }
    return error;
}

void AudioDriverALSA::thread_function(void* user_data) {
    auto* audio_driver = static_cast<AudioDriverALSA*>(user_data);
    while (!audio_driver->thread_running) {
        audio_driver->lock();
        audio_driver->start_counting_ticks();
        audio_driver->playback_sound();
        // Check if output device has changed.
        if (audio_driver->device_name != audio_driver->new_device_name) {
            if (audio_driver->change_output_device() != OK) {
                break;
            }
        }
        audio_driver->stop_counting_ticks();
        audio_driver->unlock();
    }
}

#endif // ALSA_ENABLED
