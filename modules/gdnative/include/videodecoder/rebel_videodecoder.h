// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_VIDEODECODER_H
#define GDNATIVE_VIDEODECODER_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REBEL_AV_API_MAJOR 0
#define REBEL_AV_API_MINOR 1

typedef struct {
    rebel_gdnative_api_version version;
    void* next;
    void* (*constructor)(rebel_object*);
    void (*destructor)(void*);
    const char* (*get_plugin_name)();
    const char** (*get_supported_extensions)(int* count);
    rebel_bool (*open_file)(void*, void*); // data struct, and a FileAccess
                                           // pointer
    rebel_real (*get_length)(const void*);
    rebel_real (*get_playback_position)(const void*);
    void (*seek)(void*, rebel_real);
    void (*set_audio_track)(void*, rebel_int);
    void (*update)(void*, rebel_real);
    rebel_pool_byte_array* (*get_videoframe)(void*);
    rebel_int (*get_audioframe)(void*, float*, int);
    rebel_int (*get_channels)(const void*);
    rebel_int (*get_mix_rate)(const void*);
    rebel_vector2 (*get_texture_size)(const void*);
} rebel_videodecoder_interface_gdnative;

typedef int (*GDNativeAudioMixCallback)(void*, const float*, int);

// FileAccess wrappers for custom FFmpeg IO
rebel_int GDAPI
rebel_videodecoder_file_read(void* file_ptr, uint8_t* buf, int buf_size);
int64_t GDAPI
rebel_videodecoder_file_seek(void* file_ptr, int64_t pos, int whence);
void GDAPI rebel_videodecoder_register_decoder(
    const rebel_videodecoder_interface_gdnative* p_interface
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VIDEODECODER_H
