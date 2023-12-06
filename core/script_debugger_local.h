// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SCRIPT_DEBUGGER_LOCAL_H
#define SCRIPT_DEBUGGER_LOCAL_H

#include "core/list.h"
#include "core/script_language.h"

class ScriptDebuggerLocal : public ScriptDebugger {
    bool profiling;
    float frame_time, idle_time, physics_time, physics_frame_time;
    uint64_t idle_accum;
    String target_function;
    Map<String, String> options;

    Vector<ScriptLanguage::ProfilingInfo> pinfo;

    Pair<String, int> to_breakpoint(const String& p_line);
    void print_variables(
        const List<String>& names,
        const List<Variant>& values,
        const String& variable_prefix
    );

public:
    void debug(
        ScriptLanguage* p_script,
        bool p_can_continue,
        bool p_is_error_breakpoint
    );
    virtual void send_message(const String& p_message, const Array& p_args);
    virtual void send_error(
        const String& p_func,
        const String& p_file,
        int p_line,
        const String& p_err,
        const String& p_descr,
        ErrorHandlerType p_type,
        const Vector<ScriptLanguage::StackInfo>& p_stack_info
    );

    virtual bool is_profiling() const {
        return profiling;
    }

    virtual void add_profiling_frame_data(
        const StringName& p_name,
        const Array& p_data
    ) {}

    virtual void idle_poll();

    virtual void profiling_start();
    virtual void profiling_end();
    virtual void profiling_set_frame_times(
        float p_frame_time,
        float p_idle_time,
        float p_physics_time,
        float p_physics_frame_time
    );

    ScriptDebuggerLocal();
};

#endif // SCRIPT_DEBUGGER_LOCAL_H
