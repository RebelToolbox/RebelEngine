// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_NATIVESCRIPT_H
#define GDNATIVE_NATIVESCRIPT_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    REBEL_METHOD_RPC_MODE_DISABLED,
    REBEL_METHOD_RPC_MODE_REMOTE,
    REBEL_METHOD_RPC_MODE_MASTER,
    REBEL_METHOD_RPC_MODE_PUPPET,
    REBEL_METHOD_RPC_MODE_SLAVE = REBEL_METHOD_RPC_MODE_PUPPET,
    REBEL_METHOD_RPC_MODE_REMOTESYNC,
    REBEL_METHOD_RPC_MODE_SYNC = REBEL_METHOD_RPC_MODE_REMOTESYNC,
    REBEL_METHOD_RPC_MODE_MASTERSYNC,
    REBEL_METHOD_RPC_MODE_PUPPETSYNC,
} rebel_method_rpc_mode;

typedef enum {
    REBEL_PROPERTY_HINT_NONE,  ///< no hint provided.
    REBEL_PROPERTY_HINT_RANGE, ///< hint_text = "min,max,step,slider; //slider
                               ///< is optional"
    REBEL_PROPERTY_HINT_EXP_RANGE,  ///< hint_text = "min,max,step", exponential
                                    ///< edit
    REBEL_PROPERTY_HINT_ENUM,       ///< hint_text= "val1,val2,val3,etc"
    REBEL_PROPERTY_HINT_EXP_EASING, /// exponential easing function (Math::ease)
    REBEL_PROPERTY_HINT_LENGTH,     ///< hint_text= "length" (as integer)
    REBEL_PROPERTY_HINT_SPRITE_FRAME, // FIXME: Obsolete: drop whenever we can
                                      // break compat
    REBEL_PROPERTY_HINT_KEY_ACCEL,    ///< hint_text= "length" (as integer)
    REBEL_PROPERTY_HINT_FLAGS, ///< hint_text= "flag1,flag2,etc" (as bit flags)
    REBEL_PROPERTY_HINT_LAYERS_2D_RENDER,
    REBEL_PROPERTY_HINT_LAYERS_2D_PHYSICS,
    REBEL_PROPERTY_HINT_LAYERS_3D_RENDER,
    REBEL_PROPERTY_HINT_LAYERS_3D_PHYSICS,
    REBEL_PROPERTY_HINT_FILE, ///< a file path must be passed, hint_text
                              ///< (optionally) is a filter "*.png,*.wav,*.doc,"
    REBEL_PROPERTY_HINT_DIR,  ///< a directory path must be passed
    REBEL_PROPERTY_HINT_GLOBAL_FILE, ///< a file path must be passed, hint_text
                                     ///< (optionally) is a filter
                                     ///< "*.png,*.wav,*.doc,"
    REBEL_PROPERTY_HINT_GLOBAL_DIR,  ///< a directory path must be passed
    REBEL_PROPERTY_HINT_RESOURCE_TYPE,  ///< a resource object type
    REBEL_PROPERTY_HINT_MULTILINE_TEXT, ///< used for string properties that can
                                        ///< contain multiple lines
    REBEL_PROPERTY_HINT_PLACEHOLDER_TEXT, ///< used to set a placeholder text
                                          ///< for string properties
    REBEL_PROPERTY_HINT_COLOR_NO_ALPHA,   ///< used for ignoring alpha component
                                          ///< when editing a color
    REBEL_PROPERTY_HINT_IMAGE_COMPRESS_LOSSY,
    REBEL_PROPERTY_HINT_IMAGE_COMPRESS_LOSSLESS,
    REBEL_PROPERTY_HINT_OBJECT_ID,
    REBEL_PROPERTY_HINT_TYPE_STRING, ///< a type string, the hint is the base
                                     ///< type to choose
    REBEL_PROPERTY_HINT_NODE_PATH_TO_EDITED_NODE, ///< so something else can
                                                  ///< provide this (used in
                                                  ///< scripts)
    REBEL_PROPERTY_HINT_METHOD_OF_VARIANT_TYPE,   ///< a method of a type
    REBEL_PROPERTY_HINT_METHOD_OF_BASE_TYPE,      ///< a method of a base type
    REBEL_PROPERTY_HINT_METHOD_OF_INSTANCE,       ///< a method of an instance
    REBEL_PROPERTY_HINT_METHOD_OF_SCRIPT, ///< a method of a script & base
    REBEL_PROPERTY_HINT_PROPERTY_OF_VARIANT_TYPE, ///< a property of a type
    REBEL_PROPERTY_HINT_PROPERTY_OF_BASE_TYPE,    ///< a property of a base type
    REBEL_PROPERTY_HINT_PROPERTY_OF_INSTANCE,     ///< a property of an instance
    REBEL_PROPERTY_HINT_PROPERTY_OF_SCRIPT, ///< a property of a script & base
    REBEL_PROPERTY_HINT_MAX,
} rebel_property_hint;

typedef enum {
    REBEL_PROPERTY_USAGE_STORAGE       = 1,
    REBEL_PROPERTY_USAGE_EDITOR        = 2,
    REBEL_PROPERTY_USAGE_NETWORK       = 4,
    REBEL_PROPERTY_USAGE_EDITOR_HELPER = 8,
    REBEL_PROPERTY_USAGE_CHECKABLE = 16, // used for editing global variables
    REBEL_PROPERTY_USAGE_CHECKED   = 32, // used for editing global variables
    REBEL_PROPERTY_USAGE_INTERNATIONALIZED =
        64,                           // hint for internationalized strings
    REBEL_PROPERTY_USAGE_GROUP = 128, // used for grouping props in the editor
    REBEL_PROPERTY_USAGE_CATEGORY = 256,
    REBEL_PROPERTY_USAGE_STORE_IF_NONZERO =
        512, // FIXME: Obsolete: drop whenever we can break compat
    REBEL_PROPERTY_USAGE_STORE_IF_NONONE =
        1024, // FIXME: Obsolete: drop whenever we can break compat
    REBEL_PROPERTY_USAGE_NO_INSTANCE_STATE      = 2048,
    REBEL_PROPERTY_USAGE_RESTART_IF_CHANGED     = 4096,
    REBEL_PROPERTY_USAGE_SCRIPT_VARIABLE        = 8192,
    REBEL_PROPERTY_USAGE_STORE_IF_NULL          = 16384,
    REBEL_PROPERTY_USAGE_ANIMATE_AS_TRIGGER     = 32768,
    REBEL_PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED = 65536,

    REBEL_PROPERTY_USAGE_DEFAULT = REBEL_PROPERTY_USAGE_STORAGE
                                 | REBEL_PROPERTY_USAGE_EDITOR
                                 | REBEL_PROPERTY_USAGE_NETWORK,
    REBEL_PROPERTY_USAGE_DEFAULT_INTL =
        REBEL_PROPERTY_USAGE_STORAGE | REBEL_PROPERTY_USAGE_EDITOR
        | REBEL_PROPERTY_USAGE_NETWORK | REBEL_PROPERTY_USAGE_INTERNATIONALIZED,
    REBEL_PROPERTY_USAGE_NOEDITOR =
        REBEL_PROPERTY_USAGE_STORAGE | REBEL_PROPERTY_USAGE_NETWORK,
} rebel_property_usage_flags;

typedef struct {
    rebel_method_rpc_mode rset_type;

    rebel_int type;
    rebel_property_hint hint;
    rebel_string hint_string;
    rebel_property_usage_flags usage;
    rebel_variant default_value;
} rebel_property_attributes;

typedef struct {
    // instance pointer, method_data - return user data
    GDCALLINGCONV void* (*create_func)(rebel_object*, void*);
    void* method_data;
    GDCALLINGCONV void (*free_func)(void*);
} rebel_instance_create_func;

typedef struct {
    // instance pointer, method data, user data
    GDCALLINGCONV void (*destroy_func)(rebel_object*, void*, void*);
    void* method_data;
    GDCALLINGCONV void (*free_func)(void*);
} rebel_instance_destroy_func;

void GDAPI rebel_nativescript_register_class(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_base,
    rebel_instance_create_func p_create_func,
    rebel_instance_destroy_func p_destroy_func
);

void GDAPI rebel_nativescript_register_tool_class(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_base,
    rebel_instance_create_func p_create_func,
    rebel_instance_destroy_func p_destroy_func
);

typedef struct {
    rebel_method_rpc_mode rpc_type;
} rebel_method_attributes;

typedef struct {
    // instance pointer, method data, user data, num args, args - return result
    // as variant
    GDCALLINGCONV
    rebel_variant (*method)(rebel_object*, void*, void*, int, rebel_variant**);
    void* method_data;
    GDCALLINGCONV void (*free_func)(void*);
} rebel_instance_method;

void GDAPI rebel_nativescript_register_method(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_function_name,
    rebel_method_attributes p_attr,
    rebel_instance_method p_method
);

typedef struct {
    // instance pointer, method data, user data, value
    GDCALLINGCONV void (*set_func)(rebel_object*, void*, void*, rebel_variant*);
    void* method_data;
    GDCALLINGCONV void (*free_func)(void*);
} rebel_property_set_func;

typedef struct {
    // instance pointer, method data, user data, value
    GDCALLINGCONV rebel_variant (*get_func)(rebel_object*, void*, void*);
    void* method_data;
    GDCALLINGCONV void (*free_func)(void*);
} rebel_property_get_func;

void GDAPI rebel_nativescript_register_property(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_path,
    rebel_property_attributes* p_attr,
    rebel_property_set_func p_set_func,
    rebel_property_get_func p_get_func
);

typedef struct {
    rebel_string name;
    rebel_int type;
    rebel_property_hint hint;
    rebel_string hint_string;
    rebel_property_usage_flags usage;
    rebel_variant default_value;
} rebel_signal_argument;

typedef struct {
    rebel_string name;
    int num_args;
    rebel_signal_argument* args;
    int num_default_args;
    rebel_variant* default_args;
} rebel_signal;

void GDAPI rebel_nativescript_register_signal(
    void* p_gdnative_handle,
    const char* p_name,
    const rebel_signal* p_signal
);

void GDAPI* rebel_nativescript_get_userdata(rebel_object* p_instance);

/*
 *
 *
 * NativeScript 1.1
 *
 *
 */

// method registering with argument names

typedef struct {
    rebel_string name;

    rebel_variant_type type;
    rebel_property_hint hint;
    rebel_string hint_string;
} rebel_method_arg;

void GDAPI rebel_nativescript_set_method_argument_information(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_function_name,
    int p_num_args,
    const rebel_method_arg* p_args
);

// documentation

void GDAPI rebel_nativescript_set_class_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    rebel_string p_documentation
);
void GDAPI rebel_nativescript_set_method_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_function_name,
    rebel_string p_documentation
);
void GDAPI rebel_nativescript_set_property_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_path,
    rebel_string p_documentation
);
void GDAPI rebel_nativescript_set_signal_documentation(
    void* p_gdnative_handle,
    const char* p_name,
    const char* p_signal_name,
    rebel_string p_documentation
);

// type tag API

void GDAPI rebel_nativescript_set_global_type_tag(
    int p_idx,
    const char* p_name,
    const void* p_type_tag
);
const void GDAPI* rebel_nativescript_get_global_type_tag(
    int p_idx,
    const char* p_name
);

void GDAPI rebel_nativescript_set_type_tag(
    void* p_gdnative_handle,
    const char* p_name,
    const void* p_type_tag
);
const void GDAPI* rebel_nativescript_get_type_tag(const rebel_object* p_object);

// instance binding API

typedef struct {
    GDCALLINGCONV void* (*alloc_instance_binding_data)(void*, const void*, rebel_object*);
    GDCALLINGCONV void (*free_instance_binding_data)(void*, void*);
    GDCALLINGCONV void (*refcount_incremented_instance_binding)(void*, rebel_object*);
    GDCALLINGCONV bool (*refcount_decremented_instance_binding)(void*, rebel_object*);
    void* data;
    GDCALLINGCONV void (*free_func)(void*);
} rebel_instance_binding_functions;

int GDAPI rebel_nativescript_register_instance_binding_data_functions(
    rebel_instance_binding_functions p_binding_functions
);
void GDAPI
rebel_nativescript_unregister_instance_binding_data_functions(int p_idx);

void GDAPI* rebel_nativescript_get_instance_binding_data(
    int p_idx,
    rebel_object* p_object
);

void GDAPI
rebel_nativescript_profiling_add_data(const char* p_signature, uint64_t p_time);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_NATIVESCRIPT_H
