// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "thread_local.h"

#ifdef WINDOWS_ENABLED
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "core/os/memory.h"
#include "core/print_string.h"

struct ThreadLocalStorage::Impl {
#ifdef WINDOWS_ENABLED
    DWORD dwFlsIndex;
#else
    pthread_key_t key;
#endif

    void* get_value() const {
#ifdef WINDOWS_ENABLED
        return FlsGetValue(dwFlsIndex);
#else
        return pthread_getspecific(key);
#endif
    }

    void set_value(void* p_value) const {
#ifdef WINDOWS_ENABLED
        FlsSetValue(dwFlsIndex, p_value);
#else
        pthread_setspecific(key, p_value);
#endif
    }

#ifdef WINDOWS_ENABLED
#define _CALLBACK_FUNC_ __stdcall
#else
#define _CALLBACK_FUNC_
#endif

    Impl(void(_CALLBACK_FUNC_* p_destr_callback_func)(void*)) {
#ifdef WINDOWS_ENABLED
        dwFlsIndex = FlsAlloc(p_destr_callback_func);
        ERR_FAIL_COND(dwFlsIndex == FLS_OUT_OF_INDEXES);
#else
        pthread_key_create(&key, p_destr_callback_func);
#endif
    }

    ~Impl() {
#ifdef WINDOWS_ENABLED
        FlsFree(dwFlsIndex);
#else
        pthread_key_delete(key);
#endif
    }
};

void* ThreadLocalStorage::get_value() const {
    return pimpl->get_value();
}

void ThreadLocalStorage::set_value(void* p_value) const {
    pimpl->set_value(p_value);
}

void ThreadLocalStorage::alloc(void(_CALLBACK_FUNC_* p_destr_callback)(void*)) {
    pimpl = memnew(ThreadLocalStorage::Impl(p_destr_callback));
}

#undef _CALLBACK_FUNC_

void ThreadLocalStorage::free() {
    memdelete(pimpl);
    pimpl = NULL;
}
