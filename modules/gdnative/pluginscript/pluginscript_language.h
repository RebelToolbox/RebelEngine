// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PLUGINSCRIPT_LANGUAGE_H
#define PLUGINSCRIPT_LANGUAGE_H

// Rebel imports
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/map.h"
#include "core/script_language.h"
#include "core/self_list.h"
// PluginScript imports
#include "pluginscript_loader.h"

#include <pluginscript/rebel_pluginscript.h>

class PluginScript;
class PluginScriptInstance;

class PluginScriptLanguage : public ScriptLanguage {
    friend class PluginScript;
    friend class PluginScriptInstance;

    Ref<ResourceFormatLoaderPluginScript> _resource_loader;
    Ref<ResourceFormatSaverPluginScript> _resource_saver;
    const rebel_pluginscript_language_desc _desc;
    rebel_pluginscript_language_data* _data;

    Mutex _lock;
    SelfList<PluginScript>::List _script_list;

public:
    virtual String get_name() const;

    _FORCE_INLINE_ Ref<ResourceFormatLoaderPluginScript> get_resource_loader() {
        return _resource_loader;
    }

    _FORCE_INLINE_ Ref<ResourceFormatSaverPluginScript> get_resource_saver() {
        return _resource_saver;
    }

    /* LANGUAGE FUNCTIONS */
    virtual void init();
    virtual String get_type() const;
    virtual String get_extension() const;
    virtual Error execute_file(const String& p_path);
    virtual void finish();

    /* EDITOR FUNCTIONS */
    virtual void get_reserved_words(List<String>* p_words) const;
    virtual bool is_control_flow_keyword(String p_keyword) const;
    virtual void get_comment_delimiters(List<String>* p_delimiters) const;
    virtual void get_string_delimiters(List<String>* p_delimiters) const;
    virtual Ref<Script> get_template(
        const String& p_class_name,
        const String& p_base_class_name
    ) const;
    virtual bool validate(
        const String& p_script,
        int& r_line_error,
        int& r_col_error,
        String& r_test_error,
        const String& p_path                      = "",
        List<String>* r_functions                 = nullptr,
        List<ScriptLanguage::Warning>* r_warnings = nullptr,
        Set<int>* r_safe_lines                    = nullptr
    ) const;
    virtual Script* create_script() const;
    virtual bool has_named_classes() const;
    virtual bool supports_builtin_mode() const;

    virtual bool can_inherit_from_file() {
        return true;
    }

    virtual int find_function(const String& p_function, const String& p_code)
        const;
    virtual String make_function(
        const String& p_class,
        const String& p_name,
        const PoolStringArray& p_args
    ) const;
    virtual Error complete_code(
        const String& p_code,
        const String& p_path,
        Object* p_owner,
        List<ScriptCodeCompletionOption>* r_options,
        bool& r_force,
        String& r_call_hint
    );
    virtual void auto_indent_code(
        String& p_code,
        int p_from_line,
        int p_to_line
    ) const;
    virtual void add_global_constant(
        const StringName& p_variable,
        const Variant& p_value
    );

    /* MULTITHREAD FUNCTIONS */

    // some VMs need to be notified of thread creation/exiting to allocate a
    // stack
    //  void thread_enter() {}
    //  void thread_exit() {}

    /* DEBUGGER FUNCTIONS */

    virtual String debug_get_error() const;
    virtual int debug_get_stack_level_count() const;
    virtual int debug_get_stack_level_line(int p_level) const;
    virtual String debug_get_stack_level_function(int p_level) const;
    virtual String debug_get_stack_level_source(int p_level) const;
    virtual void debug_get_stack_level_locals(
        int p_level,
        List<String>* p_locals,
        List<Variant>* p_values,
        int p_max_subitems = -1,
        int p_max_depth    = -1
    );
    virtual void debug_get_stack_level_members(
        int p_level,
        List<String>* p_members,
        List<Variant>* p_values,
        int p_max_subitems = -1,
        int p_max_depth    = -1
    );
    virtual void debug_get_globals(
        List<String>* p_locals,
        List<Variant>* p_values,
        int p_max_subitems = -1,
        int p_max_depth    = -1
    );
    virtual String debug_parse_stack_level_expression(
        int p_level,
        const String& p_expression,
        int p_max_subitems = -1,
        int p_max_depth    = -1
    );

    // virtual Vector<StackInfo> debug_get_current_stack_info() { return
    // Vector<StackInfo>(); }

    virtual void reload_all_scripts();
    virtual void reload_tool_script(
        const Ref<Script>& p_script,
        bool p_soft_reload
    );

    /* LOADER FUNCTIONS */

    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual void get_public_functions(List<MethodInfo>* p_functions) const;
    virtual void get_public_constants(List<Pair<String, Variant>>* p_constants
    ) const;

    virtual void profiling_start();
    virtual void profiling_stop();

    virtual int profiling_get_accumulated_data(
        ProfilingInfo* p_info_arr,
        int p_info_max
    );
    virtual int profiling_get_frame_data(
        ProfilingInfo* p_info_arr,
        int p_info_max
    );

    virtual void frame();

    void lock();
    void unlock();

    PluginScriptLanguage(const rebel_pluginscript_language_desc* desc);

    virtual ~PluginScriptLanguage() {}
};

#endif // PLUGINSCRIPT_LANGUAGE_H
