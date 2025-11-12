// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "audio_driver_pulseaudio.h"

#ifdef PULSEAUDIO_ENABLED

#include "core/os/os.h"
#include "core/project_settings.h"

#ifdef ALSAMIDI_ENABLED
#include "drivers/alsa/asound-so_wrap.h"
#endif // ALSAMIDI_ENABLED

Error AudioDriverPulseAudio::init() {
#ifdef DEBUG_ENABLED
    constexpr int dylibloader_verbose = 1;
#else  // ! DEBUG_ENABLED
    constexpr int dylibloader_verbose = 0;
#endif // DEBUG_ENABLED

#ifdef ALSAMIDI_ENABLED
    // If using PulseAudio with ALSA MIDI, we need to initialize ALSA.
    initialize_asound(dylibloader_verbose);
#endif // ALSAMIDI_ENABLED

    if (initialize_pulse(dylibloader_verbose)) {
        return ERR_CANT_OPEN;
    }

    active        = false;
    thread_exited = false;
    exit_thread   = false;
    mix_rate      = GLOBAL_GET("audio/mix_rate");
    mainloop      = pa_mainloop_new();
    ERR_FAIL_NULL_V_MSG(
        mainloop,
        ERR_CANT_OPEN,
        "PulseAudio: Couldn't create mainloop!"
    );
    context = pa_context_new(pa_mainloop_get_api(mainloop), "Rebel");
    ERR_FAIL_NULL_V_MSG(
        context,
        ERR_CANT_OPEN,
        "PulseAudio: Couldn't create context!"
    );

    pa_context_set_state_callback(context, context_state_callback, this);
    int error_code =
        pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    if (error_code < 0) {
        if (context) {
            pa_context_unref(context);
            context = nullptr;
        }
        if (mainloop) {
            pa_mainloop_free(mainloop);
            mainloop = nullptr;
        }
        ERR_PRINT("PulseAudio: Couldn't connect context!");
        return ERR_CANT_OPEN;
    }

    pa_ready = 0;
    while (pa_ready == 0) {
        error_code = pa_mainloop_iterate(mainloop, 1, nullptr);
        if (error_code < 0) {
            ERR_PRINT("PulseAudio: Couldn't iterate mainloop!");
        }
    }
    if (pa_ready < 0) {
        if (context) {
            pa_context_disconnect(context);
            pa_context_unref(context);
            context = nullptr;
        }
        if (mainloop) {
            pa_mainloop_free(mainloop);
            mainloop = nullptr;
        }
        return ERR_CANT_OPEN;
    }

    Error error = initialize_output_device();
    if (error != OK) {
        ERR_PRINT(
            "PulseAudio: Failed to initialize the output "
            "device!"
        );
        return error;
    }

    thread.start(thread_function, this);
    return OK;
}

void AudioDriverPulseAudio::start() {
    active = true;
}

const char* AudioDriverPulseAudio::get_name() const {
    return "PulseAudio";
}

String AudioDriverPulseAudio::get_device() {
    lock();
    String device_name = output_device_name;
    unlock();
    return device_name;
}

void AudioDriverPulseAudio::set_device(const String device_name) {
    lock();
    new_output_device_name = device_name;
    unlock();
}

Array AudioDriverPulseAudio::get_device_list() {
    output_devices.clear();
    output_devices.push_back("Default");
    if (context == nullptr) {
        ERR_PRINT("PulseAudio: Not initialised!");
        return output_devices;
    }

    lock();
    pa_status               = 0;
    pa_operation* operation = pa_context_get_sink_info_list(
        context,
        get_sink_info_list_callback,
        this
    );
    if (operation) {
        while (pa_status == 0) {
            const int error_code = pa_mainloop_iterate(mainloop, 1, nullptr);
            if (error_code < 0) {
                ERR_PRINT("PulseAudio: Couldn't iterate mainloop!");
            }
        }
        pa_operation_unref(operation);
    } else {
        ERR_PRINT("PulseAudio: Failed to get sink info list operation!");
    }
    unlock();

    return output_devices;
}

String AudioDriverPulseAudio::capture_get_device() {
    lock();
    String device_name = input_device_name;
    unlock();
    return device_name;
}

void AudioDriverPulseAudio::capture_set_device(const String& device_name) {
    lock();
    new_input_device_name = device_name;
    unlock();
}

Array AudioDriverPulseAudio::capture_get_device_list() {
    input_devices.clear();
    input_devices.push_back("Default");
    if (context == nullptr) {
        return input_devices;
    }

    lock();
    pa_status               = 0;
    pa_operation* operation = pa_context_get_source_info_list(
        context,
        get_source_info_list_callback,
        this
    );
    if (operation) {
        while (pa_status == 0) {
            const int error_code = pa_mainloop_iterate(mainloop, 1, nullptr);
            if (error_code < 0) {
                ERR_PRINT("PulseAudio: Couldn't iterate mainloop!");
            }
        }
        pa_operation_unref(operation);
    } else {
        ERR_PRINT("PulseAudio: Failed to get source info list operation!");
    }
    unlock();

    return input_devices;
}

float AudioDriverPulseAudio::get_latency() {
    if (latency != 0) {
        return latency;
    }

    lock();
    pa_usec_t pa_latency = 0;
    if (pa_stream_get_state(output_stream) == PA_STREAM_READY) {
        int negative = 0;
        if (pa_stream_get_latency(output_stream, &pa_latency, &negative) >= 0) {
            if (negative) {
                pa_latency = 0;
            }
        }
    }
    if (pa_latency > 0) {
        latency = static_cast<float>(pa_latency) / 1000000.f;
    }
    unlock();

    return latency;
}

int AudioDriverPulseAudio::get_mix_rate() const {
    return mix_rate;
}

AudioDriver::SpeakerMode AudioDriverPulseAudio::get_speaker_mode() const {
    return get_speaker_mode_by_total_channels(channels);
}

Error AudioDriverPulseAudio::capture_start() {
    lock();
    const Error error = initialize_input_device();
    unlock();
    return error;
}

Error AudioDriverPulseAudio::capture_stop() {
    lock();
    disconnect_input_device();
    unlock();
    return OK;
}

void AudioDriverPulseAudio::lock() {
    mutex.lock();
}

void AudioDriverPulseAudio::unlock() {
    mutex.unlock();
}

void AudioDriverPulseAudio::finish() {
    if (!thread.is_started()) {
        return;
    }
    exit_thread = true;
    thread.wait_to_finish();
    disconnect_output_device();
    if (context) {
        pa_context_disconnect(context);
        pa_context_unref(context);
        context = nullptr;
    }
    if (mainloop) {
        pa_mainloop_free(mainloop);
        mainloop = nullptr;
    }
}

Error AudioDriverPulseAudio::initialize_output_device() {
    if (output_device_name != "Default") {
        // Check if the specified output device exists.
        const Array list = get_device_list();
        if (list.find(output_device_name) == -1) {
            // Use the default, because we can't find the specified device.
            output_device_name     = "Default";
            new_output_device_name = "Default";
        }
    }

    const Error error = detect_output_channels();
    if (error != OK) {
        ERR_PRINT("PulseAudio: Failed to detect the output channels!");
        return error;
    }
    print_verbose(
        "PulseAudio: Detected " + itos(output_channel_map.channels)
        + " output channels."
    );

    // PulseAudio's number of channels is stored in output_channel_map.channels.
    // Rebel Engine uses an even number of channels.
    // If the number of channels is odd, Rebel Engine will report one more.
    switch (output_channel_map.channels) {
        case 1: // Mono
        case 3: // Surround 2.1
        case 5: // Surround 5.0
        case 7: // Surround 7.0
            channels = output_channel_map.channels + 1;
            break;

        case 2: // Stereo
        case 4: // Surround 4.0
        case 6: // Surround 5.1
        case 8: // Surround 7.1
            channels = output_channel_map.channels;
            break;

        default:
            WARN_PRINT(
                "PulseAudio: Unsupported number of output channels: "
                + itos(output_channel_map.channels)
            );
            pa_channel_map_init_stereo(&output_channel_map);
            channels = 2;
            break;
    }

    const int output_latency = GLOBAL_GET("audio/output_latency");
    const int buffer_frames =
        static_cast<int>(closest_power_of_2(output_latency * mix_rate / 1000));
    const int buffer_size = buffer_frames * output_channel_map.channels;
    print_verbose(
        "PulseAudio: Detected " + itos(output_channel_map.channels)
        + " output channels."
    );
    print_verbose(
        "PulseAudio: Using " + itos(buffer_frames) + " output buffer frames."
    );
    print_verbose(
        "PulseAudio: Calculated output latency: "
        + itos(buffer_frames * 1000 / mix_rate) + " ms."
    );

    pa_sample_spec sample_spec;
    sample_spec.format        = PA_SAMPLE_S16LE;
    sample_spec.channels      = output_channel_map.channels;
    sample_spec.rate          = mix_rate;
    output_channel_map.map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
    output_channel_map.map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
    output_channel_map.map[2] = PA_CHANNEL_POSITION_FRONT_CENTER;
    output_channel_map.map[3] = PA_CHANNEL_POSITION_LFE;
    output_channel_map.map[4] = PA_CHANNEL_POSITION_REAR_LEFT;
    output_channel_map.map[5] = PA_CHANNEL_POSITION_REAR_RIGHT;
    output_channel_map.map[6] = PA_CHANNEL_POSITION_SIDE_LEFT;
    output_channel_map.map[7] = PA_CHANNEL_POSITION_SIDE_RIGHT;

    output_stream = pa_stream_new(
        context,
        "Rebel Audio",
        &sample_spec,
        &output_channel_map
    );
    ERR_FAIL_NULL_V_MSG(
        output_stream,
        ERR_CANT_OPEN,
        "PulseAudio: Failed to create an output stream: "
            + String(pa_strerror(pa_context_errno(context)))
    );

    pa_buffer_attr buffer_attr;
    // PulseAudio appears to have no function to get the number of fragments;
    // so fragment size is hard-coded to the default of 4.
    constexpr int fragment_size = 4;
    // Note: Actual latency is target length (tlength) / fragment size.
    buffer_attr.tlength         = buffer_size * sizeof(int16_t) * fragment_size;
    // Allow PulseAudio to set the values.
    buffer_attr.prebuf          = static_cast<uint32_t>(-1);
    buffer_attr.maxlength       = static_cast<uint32_t>(-1);
    buffer_attr.minreq          = static_cast<uint32_t>(-1);

    const char* device_name = output_device_name == "Default"
                                ? nullptr
                                : output_device_name.utf8().get_data();
    constexpr auto flags    = static_cast<pa_stream_flags>(
        PA_STREAM_INTERPOLATE_TIMING | PA_STREAM_ADJUST_LATENCY
        | PA_STREAM_AUTO_TIMING_UPDATE
    );
    const int error_code = pa_stream_connect_playback(
        output_stream,
        device_name,
        &buffer_attr,
        flags,
        nullptr,
        nullptr
    );
    ERR_FAIL_COND_V_MSG(
        error_code < 0,
        ERR_CANT_OPEN,
        "PulseAudio: Failed to connect the playback stream!"
    );

    internal_buffer.resize(buffer_frames * channels);
    output_buffer.resize(buffer_size);

    // Reset audio input to keep synchronisation.
    input_position = 0;
    input_size     = 0;

    return OK;
}

Error AudioDriverPulseAudio::initialize_input_device() {
    // Check if the specified input device exists.
    if (input_device_name != "Default") {
        const Array list = capture_get_device_list();
        if (list.find(input_device_name) == -1) {
            // Use the default, because we can't find the specified device.
            input_device_name     = "Default";
            new_input_device_name = "Default";
        }
    }

    const Error error = detect_input_channels();
    if (error != OK) {
        ERR_PRINT("PulseAudio: Failed to detect the input channels!");
        return error;
    }
    print_verbose(
        "PulseAudio: Detected " + itos(input_channel_map.channels)
        + " input channels."
    );

    switch (input_channel_map.channels) {
        case 1: // Mono
        case 2: // Stereo
            break;
        default:
            WARN_PRINT(
                "PulseAudio: Unsupported number of input channels: "
                + itos(input_channel_map.channels)
            );
            pa_channel_map_init_stereo(&input_channel_map);
            break;
    }

    constexpr int input_latency = 30;
    const int input_buffer_frames =
        static_cast<int>(closest_power_of_2(input_latency * mix_rate / 1000));
    const int input_buffer_size =
        input_buffer_frames * input_channel_map.channels;
    print_verbose(
        "PulseAudio: Detected " + itos(input_channel_map.channels)
        + " input channels."
    );
    print_verbose(
        "PulseAudio: Using " + itos(input_buffer_frames)
        + "input buffer frames."
    );
    print_verbose(
        "PulseAudio: Calculated input latency: "
        + itos(input_buffer_frames * 1000 / mix_rate) + "ms."
    );

    pa_sample_spec sample_spec;
    sample_spec.format   = PA_SAMPLE_S16LE;
    sample_spec.channels = input_channel_map.channels;
    sample_spec.rate     = mix_rate;

    input_stream = pa_stream_new(
        context,
        "Rebel Record",
        &sample_spec,
        &input_channel_map
    );
    ERR_FAIL_NULL_V_MSG(
        input_stream,
        ERR_CANT_OPEN,
        "PulseAudio: Failed to create an input stream: "
            + String(pa_strerror(pa_context_errno(context)))
    );

    pa_buffer_attr buffer_attr;
    buffer_attr.fragsize = input_buffer_size * sizeof(int16_t);

    const char* device_name = input_device_name == "Default"
                                ? nullptr
                                : input_device_name.utf8().get_data();
    constexpr auto flags    = static_cast<pa_stream_flags>(
        PA_STREAM_INTERPOLATE_TIMING | PA_STREAM_ADJUST_LATENCY
        | PA_STREAM_AUTO_TIMING_UPDATE
    );
    const int error_code = pa_stream_connect_record(
        input_stream,
        device_name,
        &buffer_attr,
        flags
    );
    ERR_FAIL_COND_V_MSG(
        error_code < 0,
        ERR_CANT_OPEN,
        "PulseAudio: Failed to connect the record stream!"
    );

    input_buffer_init(input_buffer_frames);

    return OK;
}

Error AudioDriverPulseAudio::detect_output_channels() {
    pa_channel_map_init_stereo(&output_channel_map);
    String device_name = output_device_name;
    if (device_name == "Default") {
        const Error error = get_default_device_names();
        if (error != OK) {
            return error;
        }
        device_name = default_output_device;
    }
    print_verbose(
        "PulseAudio: Detecting output channels for device: " + device_name
    );
    pa_status               = 0;
    pa_operation* operation = pa_context_get_sink_info_by_name(
        context,
        device_name.utf8().get_data(),
        get_sink_info_by_name_callback,
        this
    );
    if (operation) {
        while (pa_status == 0) {
            const int error_code = pa_mainloop_iterate(mainloop, 1, nullptr);
            if (error_code < 0) {
                ERR_PRINT("PulseAudio: Couldn't iterate mainloop!");
            }
        }
        pa_operation_unref(operation);
        if (pa_status == -1) {
            return FAILED;
        }
    } else {
        ERR_PRINT("PulseAudio: Failed to get sink info by name operation!");
    }
    return OK;
}

Error AudioDriverPulseAudio::detect_input_channels() {
    pa_channel_map_init_stereo(&input_channel_map);
    String device_name = input_device_name;
    if (device_name == "Default") {
        const Error error = get_default_device_names();
        if (error != OK) {
            return error;
        }
        device_name = default_output_device;
    }
    print_verbose(
        "PulseAudio: Detecting input channels for device: " + device_name
    );
    pa_status               = 0;
    pa_operation* operation = pa_context_get_source_info_by_name(
        context,
        device_name.utf8().get_data(),
        get_source_info_by_name_callback,
        this
    );
    if (operation) {
        while (pa_status == 0) {
            const int error_code = pa_mainloop_iterate(mainloop, 1, nullptr);
            if (error_code < 0) {
                ERR_PRINT("PulseAudio: Couldn't iterate mainloop!");
            }
        }
        pa_operation_unref(operation);
        if (pa_status == -1) {
            return FAILED;
        }
    } else {
        ERR_PRINT("PulseAudio: Failed to get source info by name operation!");
    }
    return OK;
}

void AudioDriverPulseAudio::disconnect_output_device() {
    if (output_stream) {
        const int error_code = pa_stream_disconnect(output_stream);
        if (error_code != 0) {
            ERR_PRINT(
                "PulseAudio: Failed to disconnect from the output stream: "
                + String(pa_strerror(error_code))
            );
        }
        pa_stream_unref(output_stream);
        output_stream = nullptr;
    }
}

void AudioDriverPulseAudio::disconnect_input_device() {
    if (input_stream) {
        const int error_code = pa_stream_disconnect(input_stream);
        if (error_code != 0) {
            ERR_PRINT(
                "PulseAudio: Failed to disconnect from the input stream: "
                + String(pa_strerror(error_code))
            );
        }
        pa_stream_unref(input_stream);
        input_stream = nullptr;
    }
}

Error AudioDriverPulseAudio::get_default_device_names() {
    pa_status = 0;
    pa_operation* operation =
        pa_context_get_server_info(context, get_server_info_callback, this);
    if (operation) {
        while (pa_status == 0) {
            const int error_code = pa_mainloop_iterate(mainloop, 1, nullptr);
            if (error_code < 0) {
                ERR_PRINT("PulseAudio: Couldn't iterate mainloop!");
            }
        }
        pa_operation_unref(operation);
    } else {
        ERR_PRINT(
            "pa_context_get_server_info error: "
            + String(pa_strerror(pa_context_errno(context)))
        );
        return FAILED;
    }
    return OK;
}

size_t AudioDriverPulseAudio::playback_sound() {
    if (available_bytes == 0) {
        get_output_bytes();
    }
    apply_pulse_audio_updates();
    return stream_output_bytes();
}

void AudioDriverPulseAudio::get_output_bytes() {
    lock();
    start_counting_ticks();
    if (!active) {
        clear_output_buffer();
    } else {
        fill_internal_buffer();
        copy_internal_buffer_to_output_buffer();
    }
    available_bytes = output_buffer.size() * sizeof(int16_t);
    write_offset    = 0;
    stop_counting_ticks();
    unlock();
}

void AudioDriverPulseAudio::clear_output_buffer() {
    const auto output_buffer_write = output_buffer.ptrw();
    for (int i = 0; i < output_buffer.size(); i++) {
        output_buffer_write[i] = 0;
    }
}

void AudioDriverPulseAudio::fill_internal_buffer() {
    const int buffer_frames = internal_buffer.size() / channels;
    audio_server_process(buffer_frames, internal_buffer.ptrw());
}

void AudioDriverPulseAudio::copy_internal_buffer_to_output_buffer() {
    const auto output_buffer_write = output_buffer.ptrw();
    if (channels == output_channel_map.channels) {
        for (int i = 0; i < output_buffer.size(); i++) {
            output_buffer_write[i] =
                static_cast<int16_t>(internal_buffer[i] >> 16);
        }
    } else {
        // Uneven number of channels.
        int internal_index      = 0;
        int output_index        = 0;
        const int buffer_frames = internal_buffer.size() / channels;
        for (int i = 0; i < buffer_frames; i++) {
            for (int j = 0; j < output_channel_map.channels - 1; j++) {
                output_buffer_write[output_index++] = static_cast<int16_t>(
                    internal_buffer[internal_index++] >> 16
                );
            }
            const uint32_t left  = internal_buffer[internal_index++] >> 16;
            const uint32_t right = internal_buffer[internal_index++] >> 16;
            output_buffer_write[output_index++] =
                static_cast<int16_t>((left + right) / 2);
        }
    }
}

void AudioDriverPulseAudio::apply_pulse_audio_updates() const {
    int updates;
    do {
        updates = pa_mainloop_iterate(mainloop, 0, nullptr);
    } while (updates > 0);
}

size_t AudioDriverPulseAudio::stream_output_bytes() {
    size_t bytes = 0;
    lock();
    start_counting_ticks();
    if (available_bytes > 0
        && pa_stream_get_state(output_stream) == PA_STREAM_READY) {
        const size_t writable_bytes = pa_stream_writable_size(output_stream);
        if (writable_bytes > 0) {
            bytes = MIN(writable_bytes, available_bytes);
            const auto data =
                reinterpret_cast<const char*>(output_buffer.ptr());
            const int error_code = pa_stream_write(
                output_stream,
                data + write_offset,
                bytes,
                nullptr,
                0LL,
                PA_SEEK_RELATIVE
            );
            if (error_code != 0) {
                ERR_PRINT(
                    "PulseAudio: Failed to write data to output stream: "
                    + String(pa_strerror(error_code))
                );
            } else {
                available_bytes -= bytes;
                write_offset    += bytes;
            }
        }
    }
    stop_counting_ticks();
    unlock();
    return bytes;
}

Error AudioDriverPulseAudio::change_output_device() {
    disconnect_output_device();
    output_device_name = new_output_device_name;
    Error error        = initialize_output_device();
    if (error != OK) {
        ERR_PRINT("PulseAudio: Failed to initialize the output device!");
        output_device_name     = "Default";
        new_output_device_name = "Default";
        error                  = initialize_output_device();
        if (error != OK) {
            active      = false;
            exit_thread = true;
        }
    }
    available_bytes = 0;
    write_offset    = 0;
    return error;
}

Error AudioDriverPulseAudio::change_input_device() {
    disconnect_input_device();
    input_device_name = new_input_device_name;
    Error error       = initialize_input_device();
    if (error != OK) {
        ERR_PRINT("PulseAudio: Failed to initialize the input device!");
        input_device_name     = "Default";
        new_input_device_name = "Default";
        error                 = initialize_input_device();
        if (error != OK) {
            active      = false;
            exit_thread = true;
        }
    }
    return error;
}

Error AudioDriverPulseAudio::check_default_device() {
    Error error               = OK;
    String old_default_device = default_output_device;
    get_default_device_names();
    if (old_default_device != default_output_device) {
        disconnect_output_device();
        error = initialize_output_device();
        if (error != OK) {
            ERR_PRINT(
                "PulseAudio: Failed to initialize the output "
                "device!"
            );
            active      = false;
            exit_thread = true;
        }
        available_bytes = 0;
        write_offset    = 0;
    }
    return error;
}

size_t AudioDriverPulseAudio::record_sound() {
    size_t read_bytes     = 0;
    size_t readable_bytes = pa_stream_readable_size(input_stream);
    if (readable_bytes > 0) {
        const void* data       = nullptr;
        const size_t max_bytes = output_buffer.size() * sizeof(int16_t);
        readable_bytes         = MIN(readable_bytes, max_bytes);
        int error_code = pa_stream_peek(input_stream, &data, &readable_bytes);
        if (error_code != 0) {
            ERR_PRINT("PulseAudio: Failed to read the input stream!");
        } else {
            auto source = static_cast<const int16_t*>(data);
            for (size_t i = readable_bytes >> 1; i > 0; i--) {
                const int32_t sample = *source++;
                input_buffer_write(sample << 16);
                if (input_channel_map.channels == 1) {
                    // In case input device is single channel convert it
                    // to Stereo
                    input_buffer_write(sample);
                }
            }
            read_bytes = readable_bytes;
            error_code = pa_stream_drop(input_stream);
            if (error_code != 0) {
                ERR_PRINT("PulseAudio: Failed to drop the input stream!");
            }
        }
    }
    return read_bytes;
}

void AudioDriverPulseAudio::thread_function(void* user_data) {
    const auto audio_driver    = static_cast<AudioDriverPulseAudio*>(user_data);
    uint64_t last_checked_time = OS::get_singleton()->get_ticks_msec();

    while (!audio_driver->exit_thread) {
        size_t read_bytes    = 0;
        size_t written_bytes = 0;

        written_bytes += audio_driver->playback_sound();

        audio_driver->lock();
        audio_driver->start_counting_ticks();

        // Check if output device has changed.
        if (audio_driver->new_output_device_name
            != audio_driver->output_device_name) {
            if (audio_driver->change_output_device() != OK) {
                break;
            }
        }

        // Check if default device has changed.
        if (audio_driver->output_device_name == "Default") {
            const uint64_t current_time = OS::get_singleton()->get_ticks_msec();
            if (current_time > last_checked_time + 1000) {
                last_checked_time = current_time;
                if (audio_driver->check_default_device() != OK) {
                    break;
                }
            }
        }

        if (audio_driver->input_stream
            && pa_stream_get_state(audio_driver->input_stream)
                   == PA_STREAM_READY) {
            read_bytes += audio_driver->record_sound();

            // Check if input device has changed.
            if (audio_driver->new_input_device_name
                != audio_driver->input_device_name) {
                if (audio_driver->change_input_device() != OK) {
                    break;
                }
            }
        }

        audio_driver->stop_counting_ticks();
        audio_driver->unlock();

        // If nothing was read or written, pause.
        if (written_bytes == 0 && read_bytes == 0) {
            OS::get_singleton()->delay_usec(1000);
        }
    }
    audio_driver->thread_exited = true;
}

void AudioDriverPulseAudio::context_state_callback(
    pa_context* context,
    void* user_data
) {
    const auto audio_driver = static_cast<AudioDriverPulseAudio*>(user_data);
    switch (pa_context_get_state(context)) {
        case PA_CONTEXT_READY:
            print_verbose("PulseAudio: Context ready.");
            audio_driver->pa_ready = 1;
            break;
        case PA_CONTEXT_TERMINATED:
            print_verbose("PulseAudio: Context terminated.");
            audio_driver->pa_ready = -1;
            break;
        case PA_CONTEXT_FAILED:
            print_verbose("PulseAudio: Context failed.");
            audio_driver->pa_ready = -1;
            break;
        default:
            print_verbose("PulseAudio: Context state changed.");
            // TODO: Check if we want to handle some of the other
            // PA context states like PA_CONTEXT_UNCONNECTED.
            break;
    }
}

void AudioDriverPulseAudio::get_server_info_callback(
    pa_context* context,
    const pa_server_info* server_info,
    void* user_data
) {
    ERR_FAIL_NULL_MSG(server_info, "PulseAudio server info is null.");
    const auto audio_driver = static_cast<AudioDriverPulseAudio*>(user_data);
    audio_driver->default_input_device  = server_info->default_source_name;
    audio_driver->default_output_device = server_info->default_sink_name;
    audio_driver->pa_status++;
}

void AudioDriverPulseAudio::get_sink_info_list_callback(
    pa_context* context,
    const pa_sink_info* sink_info,
    int eol,
    void* user_data
) {
    // If eol is positive, we've reached the end of the list.
    if (eol > 0) {
        return;
    }
    const auto audio_driver = static_cast<AudioDriverPulseAudio*>(user_data);
    audio_driver->output_devices.push_back(sink_info->name);
    audio_driver->pa_status++;
}

void AudioDriverPulseAudio::get_source_info_list_callback(
    pa_context* context,
    const pa_source_info* source_info,
    int eol,
    void* user_data
) {
    // If eol is positive, we've reached the end of the list.
    if (eol > 0) {
        return;
    }
    const auto audio_driver = static_cast<AudioDriverPulseAudio*>(user_data);
    if (source_info->monitor_of_sink == PA_INVALID_INDEX) {
        audio_driver->input_devices.push_back(source_info->name);
    }
    audio_driver->pa_status++;
}

void AudioDriverPulseAudio::get_sink_info_by_name_callback(
    pa_context* context,
    const pa_sink_info* sink_info,
    int eol,
    void* user_data
) {
    // If eol is positive, we've reached the end of the list.
    if (eol > 0) {
        return;
    }
    const auto audio_driver = static_cast<AudioDriverPulseAudio*>(user_data);
    // If eol is negative, there was an error.
    if (eol < 0) {
        ERR_PRINT(
            "PulseAudio: Failed to get sink info by name: "
            + String(pa_strerror(pa_context_errno(context)))
        );
        audio_driver->pa_status--;
        return;
    }
    audio_driver->output_channel_map = sink_info->channel_map;
    audio_driver->pa_status++;
}

void AudioDriverPulseAudio::get_source_info_by_name_callback(
    pa_context* context,
    const pa_source_info* source_info,
    int eol,
    void* user_data
) {
    // If eol is positive, we've reached the end of the list.
    if (eol > 0) {
        return;
    }
    const auto audio_driver = static_cast<AudioDriverPulseAudio*>(user_data);
    // If eol is negative, there was an error.
    if (eol < 0) {
        ERR_PRINT(
            "PulseAudio: Failed to get source info by name: "
            + String(pa_strerror(pa_context_errno(context)))
        );
        audio_driver->pa_status--;
        return;
    }
    audio_driver->input_channel_map = source_info->channel_map;
    audio_driver->pa_status++;
}

#endif // PULSEAUDIO_ENABLED
