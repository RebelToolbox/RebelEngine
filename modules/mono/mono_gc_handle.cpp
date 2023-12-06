// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "mono_gc_handle.h"

#include "mono_gd/gd_mono.h"

uint32_t MonoGCHandle::new_strong_handle(MonoObject* p_object) {
    return mono_gchandle_new(p_object, /* pinned: */ false);
}

uint32_t MonoGCHandle::new_strong_handle_pinned(MonoObject* p_object) {
    return mono_gchandle_new(p_object, /* pinned: */ true);
}

uint32_t MonoGCHandle::new_weak_handle(MonoObject* p_object) {
    return mono_gchandle_new_weakref(p_object, /* track_resurrection: */ false);
}

void MonoGCHandle::free_handle(uint32_t p_gchandle) {
    mono_gchandle_free(p_gchandle);
}

Ref<MonoGCHandle> MonoGCHandle::create_strong(MonoObject* p_object) {
    return memnew(MonoGCHandle(new_strong_handle(p_object), STRONG_HANDLE));
}

Ref<MonoGCHandle> MonoGCHandle::create_weak(MonoObject* p_object) {
    return memnew(MonoGCHandle(new_weak_handle(p_object), WEAK_HANDLE));
}

void MonoGCHandle::release() {
#ifdef DEBUG_ENABLED
    CRASH_COND(!released && GDMono::get_singleton() == NULL);
#endif

    if (!released && GDMono::get_singleton()->is_runtime_initialized()) {
        free_handle(handle);
        released = true;
    }
}

MonoGCHandle::MonoGCHandle(uint32_t p_handle, HandleType p_handle_type) {
    released = false;
    weak     = p_handle_type == WEAK_HANDLE;
    handle   = p_handle;
}

MonoGCHandle::~MonoGCHandle() {
    release();
}
