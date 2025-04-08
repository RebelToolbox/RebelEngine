// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_PLUGINSCRIPT_H
#define GDNATIVE_PLUGINSCRIPT_H

#include <gdnative/gdnative.h>
#include <nativescript/rebel_nativescript.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void rebel_pluginscript_instance_data;
typedef void rebel_pluginscript_script_data;
typedef void rebel_pluginscript_language_data;

// --- Instance ---

// TODO: use rebel_string_name for faster lookup ?
typedef struct {
    rebel_pluginscript_instance_data* (*init)(
        rebel_pluginscript_script_data* p_data,
        rebel_object* p_owner
    );
    void (*finish)(rebel_pluginscript_instance_data* p_data);

    rebel_bool (*set_prop)(
        rebel_pluginscript_instance_data* p_data,
        const rebel_string* p_name,
        const rebel_variant* p_value
    );
    rebel_bool (*get_prop)(
        rebel_pluginscript_instance_data* p_data,
        const rebel_string* p_name,
        rebel_variant* r_ret
    );

    rebel_variant (*call_method)(
        rebel_pluginscript_instance_data* p_data,
        const rebel_string_name* p_method,
        const rebel_variant** p_args,
        int p_argcount,
        rebel_variant_call_error* r_error
    );

    void (*notification)(
        rebel_pluginscript_instance_data* p_data,
        int p_notification
    );
    // TODO: could this rpc mode stuff be moved to the
    // rebel_pluginscript_script_manifest ?
    rebel_method_rpc_mode (*get_rpc_mode)(
        rebel_pluginscript_instance_data* p_data,
        const rebel_string* p_method
    );
    rebel_method_rpc_mode (*get_rset_mode)(
        rebel_pluginscript_instance_data* p_data,
        const rebel_string* p_variable
    );

    // this is used by script languages that keep a reference counter of their
    // own you can make make Ref<> not die when it reaches zero, so deleting the
    // reference depends entirely from the script.
    //  Note: You can set those function pointer to NULL if not needed.
    void (*refcount_incremented)(rebel_pluginscript_instance_data* p_data);
    bool (*refcount_decremented)(rebel_pluginscript_instance_data* p_data
    ); // return true if it can die
} rebel_pluginscript_instance_desc;

// --- Script ---

typedef struct {
    rebel_pluginscript_script_data* data;
    rebel_string_name name;
    rebel_bool is_tool;
    rebel_string_name base;

    // Member lines format: {<string>: <int>}
    rebel_dictionary member_lines;
    // Method info dictionary format
    // {
    //  name: <string>
    //  args: [<dict:property>]
    //  default_args: [<variant>]
    //  return: <dict:property>
    //  flags: <int>
    //  rpc_mode: <int:rebel_method_rpc_mode>
    // }
    rebel_array methods;
    // Same format than for methods
    rebel_array signals;
    // Property info dictionary format
    // {
    //  name: <string>
    //  type: <int:rebel_variant_type>
    //  hint: <int:rebel_property_hint>
    //  hint_string: <string>
    //  usage: <int:rebel_property_usage_flags>
    //  default_value: <variant>
    //  rset_mode: <int:rebel_method_rpc_mode>
    // }
    rebel_array properties;
} rebel_pluginscript_script_manifest;

typedef struct {
    rebel_pluginscript_script_manifest (*init)(
        rebel_pluginscript_language_data* p_data,
        const rebel_string* p_path,
        const rebel_string* p_source,
        rebel_error* r_error
    );
    void (*finish)(rebel_pluginscript_script_data* p_data);
    rebel_pluginscript_instance_desc instance_desc;
} rebel_pluginscript_script_desc;

// --- Language ---

typedef struct {
    rebel_string_name signature;
    rebel_int call_count;
    rebel_int total_time; // In microseconds
    rebel_int self_time;  // In microseconds
} rebel_pluginscript_profiling_data;

typedef struct {
    const char* name;
    const char* type;
    const char* extension;
    const char** recognized_extensions; // NULL terminated array
    rebel_pluginscript_language_data* (*init)();
    void (*finish)(rebel_pluginscript_language_data* p_data);
    const char** reserved_words;     // NULL terminated array
    const char** comment_delimiters; // NULL terminated array
    const char** string_delimiters;  // NULL terminated array
    rebel_bool has_named_classes;
    rebel_bool supports_builtin_mode;

    rebel_string (*get_template_source_code)(
        rebel_pluginscript_language_data* p_data,
        const rebel_string* p_class_name,
        const rebel_string* p_base_class_name
    );
    rebel_bool (*validate)(
        rebel_pluginscript_language_data* p_data,
        const rebel_string* p_script,
        int* r_line_error,
        int* r_col_error,
        rebel_string* r_test_error,
        const rebel_string* p_path,
        rebel_pool_string_array* r_functions
    );
    int (*find_function)(
        rebel_pluginscript_language_data* p_data,
        const rebel_string* p_function,
        const rebel_string* p_code
    ); // Can be NULL
    rebel_string (*make_function)(
        rebel_pluginscript_language_data* p_data,
        const rebel_string* p_class,
        const rebel_string* p_name,
        const rebel_pool_string_array* p_args
    );
    rebel_error (*complete_code)(
        rebel_pluginscript_language_data* p_data,
        const rebel_string* p_code,
        const rebel_string* p_path,
        rebel_object* p_owner,
        rebel_array* r_options,
        rebel_bool* r_force,
        rebel_string* r_call_hint
    );
    void (*auto_indent_code)(
        rebel_pluginscript_language_data* p_data,
        rebel_string* p_code,
        int p_from_line,
        int p_to_line
    );

    void (*add_global_constant)(
        rebel_pluginscript_language_data* p_data,
        const rebel_string* p_variable,
        const rebel_variant* p_value
    );
    rebel_string (*debug_get_error)(rebel_pluginscript_language_data* p_data);
    int (*debug_get_stack_level_count)(rebel_pluginscript_language_data* p_data
    );
    int (*debug_get_stack_level_line)(
        rebel_pluginscript_language_data* p_data,
        int p_level
    );
    rebel_string (*debug_get_stack_level_function)(
        rebel_pluginscript_language_data* p_data,
        int p_level
    );
    rebel_string (*debug_get_stack_level_source)(
        rebel_pluginscript_language_data* p_data,
        int p_level
    );
    void (*debug_get_stack_level_locals)(
        rebel_pluginscript_language_data* p_data,
        int p_level,
        rebel_pool_string_array* p_locals,
        rebel_array* p_values,
        int p_max_subitems,
        int p_max_depth
    );
    void (*debug_get_stack_level_members)(
        rebel_pluginscript_language_data* p_data,
        int p_level,
        rebel_pool_string_array* p_members,
        rebel_array* p_values,
        int p_max_subitems,
        int p_max_depth
    );
    void (*debug_get_globals)(
        rebel_pluginscript_language_data* p_data,
        rebel_pool_string_array* p_locals,
        rebel_array* p_values,
        int p_max_subitems,
        int p_max_depth
    );
    rebel_string (*debug_parse_stack_level_expression)(
        rebel_pluginscript_language_data* p_data,
        int p_level,
        const rebel_string* p_expression,
        int p_max_subitems,
        int p_max_depth
    );

    // TODO: could this stuff be moved to the rebel_pluginscript_language_desc ?
    void (*get_public_functions)(
        rebel_pluginscript_language_data* p_data,
        rebel_array* r_functions
    );
    void (*get_public_constants)(
        rebel_pluginscript_language_data* p_data,
        rebel_dictionary* r_constants
    );

    void (*profiling_start)(rebel_pluginscript_language_data* p_data);
    void (*profiling_stop)(rebel_pluginscript_language_data* p_data);
    int (*profiling_get_accumulated_data)(
        rebel_pluginscript_language_data* p_data,
        rebel_pluginscript_profiling_data* r_info,
        int p_info_max
    );
    int (*profiling_get_frame_data)(
        rebel_pluginscript_language_data* p_data,
        rebel_pluginscript_profiling_data* r_info,
        int p_info_max
    );
    void (*profiling_frame)(rebel_pluginscript_language_data* p_data);

    rebel_pluginscript_script_desc script_desc;
} rebel_pluginscript_language_desc;

void GDAPI rebel_pluginscript_register_language(
    const rebel_pluginscript_language_desc* language_desc
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_PLUGINSCRIPT_H
