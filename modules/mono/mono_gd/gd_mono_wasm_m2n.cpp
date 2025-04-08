// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gd_mono_wasm_m2n.h"

#ifdef WEB_ENABLED

#include "core/oa_hash_map.h"

typedef mono_bool (*RebelMonoM2nIcallTrampolineDispatch)(
    const char* cookie,
    void* target_func,
    Mono_InterpMethodArguments* margs
);

// This extern function is implemented in our patched version of Mono
MONO_API void rebel_mono_register_m2n_icall_trampoline_dispatch_hook(
    RebelMonoM2nIcallTrampolineDispatch hook
);

namespace GDMonoWasmM2n {

struct HashMapCookieComparator {
    static bool compare(const char* p_lhs, const char* p_rhs) {
        return strcmp(p_lhs, p_rhs) == 0;
    }
};

// The default hasher supports 'const char *' C Strings, but we need a custom
// comparator
OAHashMap<
    const char*,
    TrampolineFunc,
    HashMapHasherDefault,
    HashMapCookieComparator>
    trampolines;

void set_trampoline(
    const char* cookies,
    GDMonoWasmM2n::TrampolineFunc trampoline_func
) {
    trampolines.set(cookies, trampoline_func);
}

mono_bool trampoline_dispatch_hook(
    const char* cookie,
    void* target_func,
    Mono_InterpMethodArguments* margs
) {
    TrampolineFunc trampoline_func;

    if (!trampolines.lookup(cookie, trampoline_func)) {
        return false;
    }

    (*trampoline_func)(target_func, margs);
    return true;
}

bool initialized = false;

void lazy_initialize() {
    // Doesn't need to be thread safe
    if (!initialized) {
        initialized = true;
        rebel_mono_register_m2n_icall_trampoline_dispatch_hook(
            &trampoline_dispatch_hook
        );
    }
}
} // namespace GDMonoWasmM2n

#endif
