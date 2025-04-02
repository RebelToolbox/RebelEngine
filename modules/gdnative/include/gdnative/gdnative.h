// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_GDNATIVE_H
#define GDNATIVE_GDNATIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__ANDROID__)
#define GDCALLINGCONV
#define GDAPI GDCALLINGCONV
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#define GDCALLINGCONV __attribute__((visibility("default")))
#define GDAPI         GDCALLINGCONV
#elif TARGET_OS_MAC
#define GDCALLINGCONV __attribute__((sysv_abi))
#define GDAPI         GDCALLINGCONV
#endif
#else // !_WIN32 && !__APPLE__
#define GDCALLINGCONV __attribute__((sysv_abi))
#define GDAPI         GDCALLINGCONV
#endif

// This is for libraries *using* the header, not for exposing Rebel Engine API.
#ifdef __GNUC__
#define GDN_EXPORT __attribute__((visibility("default")))
#elif defined(_WIN32)
#define GDN_EXPORT __declspec(dllexport)
#else
#define GDN_EXPORT
#endif

#include <stdbool.h>
#include <stdint.h>

#define REBEL_API_VERSION 1

////// Error

typedef enum {
    REBEL_OK,               // (0)
    REBEL_FAILED,           ///< Generic fail error
    REBEL_ERR_UNAVAILABLE,  ///< What is requested is unsupported/unavailable
    REBEL_ERR_UNCONFIGURED, ///< The object being used hasn't been properly set
                            ///< up yet
    REBEL_ERR_UNAUTHORIZED, ///< Missing credentials for requested resource
    REBEL_ERR_PARAMETER_RANGE_ERROR, ///< Parameter given out of range (5)
    REBEL_ERR_OUT_OF_MEMORY,         ///< Out of memory
    REBEL_ERR_FILE_NOT_FOUND,
    REBEL_ERR_FILE_BAD_DRIVE,
    REBEL_ERR_FILE_BAD_PATH,
    REBEL_ERR_FILE_NO_PERMISSION, // (10)
    REBEL_ERR_FILE_ALREADY_IN_USE,
    REBEL_ERR_FILE_CANT_OPEN,
    REBEL_ERR_FILE_CANT_WRITE,
    REBEL_ERR_FILE_CANT_READ,
    REBEL_ERR_FILE_UNRECOGNIZED, // (15)
    REBEL_ERR_FILE_CORRUPT,
    REBEL_ERR_FILE_MISSING_DEPENDENCIES,
    REBEL_ERR_FILE_EOF,
    REBEL_ERR_CANT_OPEN,   ///< Can't open a resource/socket/file
    REBEL_ERR_CANT_CREATE, // (20)
    REBEL_ERR_QUERY_FAILED,
    REBEL_ERR_ALREADY_IN_USE,
    REBEL_ERR_LOCKED, ///< resource is locked
    REBEL_ERR_TIMEOUT,
    REBEL_ERR_CANT_CONNECT, // (25)
    REBEL_ERR_CANT_RESOLVE,
    REBEL_ERR_CONNECTION_ERROR,
    REBEL_ERR_CANT_ACQUIRE_RESOURCE,
    REBEL_ERR_CANT_FORK,
    REBEL_ERR_INVALID_DATA,       ///< Data passed is invalid (30)
    REBEL_ERR_INVALID_PARAMETER,  ///< Parameter passed is invalid
    REBEL_ERR_ALREADY_EXISTS,     ///< When adding, item already exists
    REBEL_ERR_DOES_NOT_EXIST,     ///< When retrieving/erasing, it item does not
                                  ///< exist
    REBEL_ERR_DATABASE_CANT_READ, ///< database is full
    REBEL_ERR_DATABASE_CANT_WRITE, ///< database is full (35)
    REBEL_ERR_COMPILATION_FAILED,
    REBEL_ERR_METHOD_NOT_FOUND,
    REBEL_ERR_LINK_FAILED,
    REBEL_ERR_SCRIPT_FAILED,
    REBEL_ERR_CYCLIC_LINK, // (40)
    REBEL_ERR_INVALID_DECLARATION,
    REBEL_ERR_DUPLICATE_SYMBOL,
    REBEL_ERR_PARSE_ERROR,
    REBEL_ERR_BUSY,
    REBEL_ERR_SKIP, // (45)
    REBEL_ERR_HELP, ///< user requested help!!
    REBEL_ERR_BUG,  ///< a bug in the software certainly happened, due to a
                    ///< double check failing or unexpected behavior.
    REBEL_ERR_PRINTER_ON_FIRE, /// the parallel port printer is engulfed in
                               /// flames
} rebel_error;

////// bool

typedef bool rebel_bool;

#define REBEL_TRUE  1
#define REBEL_FALSE 0

/////// int

typedef int rebel_int;

/////// real

typedef float rebel_real;

/////// Object (forward declared)
typedef void rebel_object;

/////// String

#include <gdnative/string.h>

/////// String name

#include <gdnative/string_name.h>

////// Vector2

#include <gdnative/vector2.h>

////// Rect2

#include <gdnative/rect2.h>

////// Vector3

#include <gdnative/vector3.h>

////// Transform2D

#include <gdnative/transform2d.h>

/////// Plane

#include <gdnative/plane.h>

/////// Quat

#include <gdnative/quat.h>

/////// AABB

#include <gdnative/aabb.h>

/////// Basis

#include <gdnative/basis.h>

/////// Transform

#include <gdnative/transform.h>

/////// Color

#include <gdnative/color.h>

/////// NodePath

#include <gdnative/node_path.h>

/////// RID

#include <gdnative/rid.h>

/////// Dictionary

#include <gdnative/dictionary.h>

/////// Array

#include <gdnative/array.h>

// single API file for Pool*Array
#include <gdnative/pool_arrays.h>

void GDAPI rebel_object_destroy(rebel_object* p_o);

////// Variant

#include <gdnative/variant.h>

////// Singleton API

rebel_object GDAPI* rebel_global_get_singleton(char* p_name
); // result shouldn't be freed

////// MethodBind API

typedef struct {
    uint8_t _dont_touch_that[1]; // TODO
} rebel_method_bind;

rebel_method_bind GDAPI* rebel_method_bind_get_method(
    const char* p_classname,
    const char* p_methodname
);
void GDAPI rebel_method_bind_ptrcall(
    rebel_method_bind* p_method_bind,
    rebel_object* p_instance,
    const void** p_args,
    void* p_ret
);
rebel_variant GDAPI rebel_method_bind_call(
    rebel_method_bind* p_method_bind,
    rebel_object* p_instance,
    const rebel_variant** p_args,
    const int p_arg_count,
    rebel_variant_call_error* p_call_error
);

////// Script API

typedef struct rebel_gdnative_api_version {
    unsigned int major;
    unsigned int minor;
} rebel_gdnative_api_version;

typedef struct rebel_gdnative_api_struct rebel_gdnative_api_struct;

struct rebel_gdnative_api_struct {
    unsigned int type;
    rebel_gdnative_api_version version;
    const rebel_gdnative_api_struct* next;
};

#define GDNATIVE_VERSION_COMPATIBLE(want, have)                                \
    (want.major == have.major && want.minor <= have.minor)

typedef struct {
    rebel_bool in_editor;
    uint64_t core_api_hash;
    uint64_t editor_api_hash;
    uint64_t no_api_hash;
    void (*report_version_mismatch)(
        const rebel_object* p_library,
        const char* p_what,
        rebel_gdnative_api_version p_want,
        rebel_gdnative_api_version p_have
    );
    void (*report_loading_error)(
        const rebel_object* p_library,
        const char* p_what
    );
    rebel_object* gd_native_library; // pointer to GDNativeLibrary that is being
                                     // initialized
    const struct rebel_gdnative_core_api_struct* api_struct;
    const rebel_string* active_library_path;
} rebel_gdnative_init_options;

typedef struct {
    rebel_bool in_editor;
} rebel_gdnative_terminate_options;

// Calling convention?
typedef rebel_object* (*rebel_class_constructor)();

rebel_class_constructor GDAPI
rebel_get_class_constructor(const char* p_classname);

rebel_dictionary GDAPI rebel_get_global_constants();

////// GDNative procedure types
typedef void (*rebel_gdnative_init_fn)(rebel_gdnative_init_options*);
typedef void (*rebel_gdnative_terminate_fn)(rebel_gdnative_terminate_options*);
typedef rebel_variant (*rebel_gdnative_procedure_fn)(rebel_array*);

////// System Functions

typedef rebel_variant (*native_call_cb)(void*, rebel_array*);
void GDAPI rebel_register_native_call_type(
    const char* p_call_type,
    native_call_cb p_callback
);

// Using these will help Rebel track how much memory is in use in debug mode.
void GDAPI* rebel_alloc(int p_bytes);
void GDAPI* rebel_realloc(void* p_ptr, int p_bytes);
void GDAPI rebel_free(void* p_ptr);

// Print using Rebel's error handler list
void GDAPI rebel_print_error(
    const char* p_description,
    const char* p_function,
    const char* p_file,
    int p_line
);
void GDAPI rebel_print_warning(
    const char* p_description,
    const char* p_function,
    const char* p_file,
    int p_line
);
void GDAPI rebel_print(const rebel_string* p_message);

// GDNATIVE CORE 1.0.1

bool GDAPI rebel_is_instance_valid(const rebel_object* p_object);

// tags used for safe dynamic casting
void GDAPI* rebel_get_class_tag(const rebel_string_name* p_class);
rebel_object GDAPI* rebel_object_cast_to(
    const rebel_object* p_object,
    void* p_class_tag
);

// equivalent of GDScript's instance_from_id
rebel_object GDAPI* rebel_instance_from_id(rebel_int p_instance_id);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_GDNATIVE_H
