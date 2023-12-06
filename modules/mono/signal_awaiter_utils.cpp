// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "signal_awaiter_utils.h"

#include "csharp_script.h"
#include "mono_gd/gd_mono_cache.h"
#include "mono_gd/gd_mono_class.h"
#include "mono_gd/gd_mono_marshal.h"
#include "mono_gd/gd_mono_utils.h"

namespace SignalAwaiterUtils
{

Error connect_signal_awaiter(
    Object* p_source,
    const String& p_signal,
    Object* p_target,
    MonoObject* p_awaiter
) {
    ERR_FAIL_NULL_V(p_source, ERR_INVALID_DATA);
    ERR_FAIL_NULL_V(p_target, ERR_INVALID_DATA);

    Ref<SignalAwaiterHandle> sa_con = memnew(SignalAwaiterHandle(p_awaiter));
#ifdef DEBUG_ENABLED
    sa_con->set_connection_target(p_target);
#endif

    Vector<Variant> binds;
    binds.push_back(sa_con);

    Error err = p_source->connect(
        p_signal,
        sa_con.ptr(),
        CSharpLanguage::get_singleton()->get_string_names()._signal_callback,
        binds,
        Object::CONNECT_ONESHOT
    );

    if (err != OK) {
        // Set it as completed to prevent it from calling the failure callback
        // when released. The awaiter will be aware of the failure by checking
        // the returned error.
        sa_con->set_completed(true);
    }

    return err;
}
} // namespace SignalAwaiterUtils

Variant SignalAwaiterHandle::_signal_callback(
    const Variant** p_args,
    int p_argcount,
    Variant::CallError& r_error
) {
#ifdef DEBUG_ENABLED
    ERR_FAIL_COND_V_MSG(
        conn_target_id && !ObjectDB::get_instance(conn_target_id),
        Variant(),
        "Resumed after await, but class instance is gone."
    );
#endif

    if (p_argcount < 1) {
        r_error.error    = Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
        r_error.argument = 1;
        return Variant();
    }

    Ref<SignalAwaiterHandle> self = *p_args[p_argcount - 1];

    if (self.is_null()) {
        r_error.error    = Variant::CallError::CALL_ERROR_INVALID_ARGUMENT;
        r_error.argument = p_argcount - 1;
        r_error.expected = Variant::OBJECT;
        return Variant();
    }

    set_completed(true);

    int signal_argc        = p_argcount - 1;
    MonoArray* signal_args = mono_array_new(
        mono_domain_get(),
        CACHED_CLASS_RAW(MonoObject),
        signal_argc
    );

    for (int i = 0; i < signal_argc; i++) {
        MonoObject* boxed = GDMonoMarshal::variant_to_mono_object(*p_args[i]);
        mono_array_setref(signal_args, i, boxed);
    }

    MonoException* exc = NULL;
    GD_MONO_BEGIN_RUNTIME_INVOKE;
    CACHED_METHOD_THUNK(SignalAwaiter, SignalCallback)
        .invoke(get_target(), signal_args, &exc);
    GD_MONO_END_RUNTIME_INVOKE;

    if (exc) {
        GDMonoUtils::set_pending_exception(exc);
        ERR_FAIL_V(Variant());
    }

    return Variant();
}

void SignalAwaiterHandle::_bind_methods() {
    ClassDB::bind_vararg_method(
        METHOD_FLAGS_DEFAULT,
        "_signal_callback",
        &SignalAwaiterHandle::_signal_callback,
        MethodInfo("_signal_callback")
    );
}

SignalAwaiterHandle::SignalAwaiterHandle(MonoObject* p_managed) :
    MonoGCHandle(MonoGCHandle::new_strong_handle(p_managed), STRONG_HANDLE) {
#ifdef DEBUG_ENABLED
    conn_target_id = 0;
#endif
}

SignalAwaiterHandle::~SignalAwaiterHandle() {
    if (!completed) {
        MonoObject* awaiter = get_target();

        if (awaiter) {
            MonoException* exc = NULL;
            GD_MONO_BEGIN_RUNTIME_INVOKE;
            CACHED_METHOD_THUNK(SignalAwaiter, FailureCallback)
                .invoke(awaiter, &exc);
            GD_MONO_END_RUNTIME_INVOKE;

            if (exc) {
                GDMonoUtils::set_pending_exception(exc);
                ERR_FAIL();
            }
        }
    }
}
