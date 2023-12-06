// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CSHARP_GC_HANDLE_H
#define CSHARP_GC_HANDLE_H

#include "core/reference.h"

#include <mono/jit/jit.h>

class MonoGCHandle : public Reference {
    GDCLASS(MonoGCHandle, Reference);

    bool released;
    bool weak;
    uint32_t handle;

public:
    enum HandleType {
        STRONG_HANDLE,
        WEAK_HANDLE
    };

    static uint32_t new_strong_handle(MonoObject* p_object);
    static uint32_t new_strong_handle_pinned(MonoObject* p_object);
    static uint32_t new_weak_handle(MonoObject* p_object);
    static void free_handle(uint32_t p_gchandle);

    static Ref<MonoGCHandle> create_strong(MonoObject* p_object);
    static Ref<MonoGCHandle> create_weak(MonoObject* p_object);

    _FORCE_INLINE_ bool is_released() {
        return released;
    }

    _FORCE_INLINE_ bool is_weak() {
        return weak;
    }

    _FORCE_INLINE_ MonoObject* get_target() const {
        return released ? NULL : mono_gchandle_get_target(handle);
    }

    _FORCE_INLINE_ void set_handle(
        uint32_t p_handle,
        HandleType p_handle_type
    ) {
        released = false;
        weak     = p_handle_type == WEAK_HANDLE;
        handle   = p_handle;
    }

    void release();

    MonoGCHandle(uint32_t p_handle, HandleType p_handle_type);
    ~MonoGCHandle();
};

#endif // CSHARP_GC_HANDLE_H
