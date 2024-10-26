// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SIGNAL_AWAITER_UTILS_H
#define SIGNAL_AWAITER_UTILS_H

#include "core/reference.h"
#include "mono_gc_handle.h"

namespace SignalAwaiterUtils {

Error connect_signal_awaiter(
    Object* p_source,
    const String& p_signal,
    Object* p_target,
    MonoObject* p_awaiter
);
} // namespace SignalAwaiterUtils

class SignalAwaiterHandle : public MonoGCHandle {
    GDCLASS(SignalAwaiterHandle, MonoGCHandle);

    bool completed;

#ifdef DEBUG_ENABLED
    ObjectID conn_target_id;
#endif

    Variant _signal_callback(
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );

protected:
    static void _bind_methods();

public:
    _FORCE_INLINE_ bool is_completed() {
        return completed;
    }

    _FORCE_INLINE_ void set_completed(bool p_completed) {
        completed = p_completed;
    }

#ifdef DEBUG_ENABLED
    _FORCE_INLINE_ void set_connection_target(Object* p_target) {
        conn_target_id = p_target->get_instance_id();
    }
#endif

    SignalAwaiterHandle(MonoObject* p_managed);
    ~SignalAwaiterHandle();
};

#endif // SIGNAL_AWAITER_UTILS_H
