// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_VARIANT_H
#define GDNATIVE_VARIANT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_VARIANT_SIZE (16 + sizeof(int64_t))

#ifndef REBEL_CORE_API_REBEL_VARIANT_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_VARIANT_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_VARIANT_SIZE];
} rebel_variant;
#endif

typedef enum rebel_variant_type {
    REBEL_VARIANT_TYPE_NIL,

    // atomic types
    REBEL_VARIANT_TYPE_BOOL,
    REBEL_VARIANT_TYPE_INT,
    REBEL_VARIANT_TYPE_REAL,
    REBEL_VARIANT_TYPE_STRING,

    // math types

    REBEL_VARIANT_TYPE_VECTOR2, // 5
    REBEL_VARIANT_TYPE_RECT2,
    REBEL_VARIANT_TYPE_VECTOR3,
    REBEL_VARIANT_TYPE_TRANSFORM2D,
    REBEL_VARIANT_TYPE_PLANE,
    REBEL_VARIANT_TYPE_QUAT, // 10
    REBEL_VARIANT_TYPE_AABB,
    REBEL_VARIANT_TYPE_BASIS,
    REBEL_VARIANT_TYPE_TRANSFORM,

    // misc types
    REBEL_VARIANT_TYPE_COLOR,
    REBEL_VARIANT_TYPE_NODE_PATH, // 15
    REBEL_VARIANT_TYPE_RID,
    REBEL_VARIANT_TYPE_OBJECT,
    REBEL_VARIANT_TYPE_DICTIONARY,
    REBEL_VARIANT_TYPE_ARRAY, // 20

    // arrays
    REBEL_VARIANT_TYPE_POOL_BYTE_ARRAY,
    REBEL_VARIANT_TYPE_POOL_INT_ARRAY,
    REBEL_VARIANT_TYPE_POOL_REAL_ARRAY,
    REBEL_VARIANT_TYPE_POOL_STRING_ARRAY,
    REBEL_VARIANT_TYPE_POOL_VECTOR2_ARRAY, // 25
    REBEL_VARIANT_TYPE_POOL_VECTOR3_ARRAY,
    REBEL_VARIANT_TYPE_POOL_COLOR_ARRAY,
} rebel_variant_type;

typedef enum rebel_variant_call_error_error {
    REBEL_CALL_ERROR_CALL_OK,
    REBEL_CALL_ERROR_CALL_ERROR_INVALID_METHOD,
    REBEL_CALL_ERROR_CALL_ERROR_INVALID_ARGUMENT,
    REBEL_CALL_ERROR_CALL_ERROR_TOO_MANY_ARGUMENTS,
    REBEL_CALL_ERROR_CALL_ERROR_TOO_FEW_ARGUMENTS,
    REBEL_CALL_ERROR_CALL_ERROR_INSTANCE_IS_NULL,
} rebel_variant_call_error_error;

typedef struct rebel_variant_call_error {
    rebel_variant_call_error_error error;
    int argument;
    rebel_variant_type expected;
} rebel_variant_call_error;

typedef enum rebel_variant_operator {
    // comparison
    REBEL_VARIANT_OP_EQUAL,
    REBEL_VARIANT_OP_NOT_EQUAL,
    REBEL_VARIANT_OP_LESS,
    REBEL_VARIANT_OP_LESS_EQUAL,
    REBEL_VARIANT_OP_GREATER,
    REBEL_VARIANT_OP_GREATER_EQUAL,

    // mathematic
    REBEL_VARIANT_OP_ADD,
    REBEL_VARIANT_OP_SUBTRACT,
    REBEL_VARIANT_OP_MULTIPLY,
    REBEL_VARIANT_OP_DIVIDE,
    REBEL_VARIANT_OP_NEGATE,
    REBEL_VARIANT_OP_POSITIVE,
    REBEL_VARIANT_OP_MODULE,
    REBEL_VARIANT_OP_STRING_CONCAT,

    // bitwise
    REBEL_VARIANT_OP_SHIFT_LEFT,
    REBEL_VARIANT_OP_SHIFT_RIGHT,
    REBEL_VARIANT_OP_BIT_AND,
    REBEL_VARIANT_OP_BIT_OR,
    REBEL_VARIANT_OP_BIT_XOR,
    REBEL_VARIANT_OP_BIT_NEGATE,

    // logic
    REBEL_VARIANT_OP_AND,
    REBEL_VARIANT_OP_OR,
    REBEL_VARIANT_OP_XOR,
    REBEL_VARIANT_OP_NOT,

    // containment
    REBEL_VARIANT_OP_IN,

    REBEL_VARIANT_OP_MAX,
} rebel_variant_operator;

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/aabb.h>
#include <gdnative/array.h>
#include <gdnative/basis.h>
#include <gdnative/color.h>
#include <gdnative/dictionary.h>
#include <gdnative/gdnative.h>
#include <gdnative/node_path.h>
#include <gdnative/plane.h>
#include <gdnative/pool_arrays.h>
#include <gdnative/quat.h>
#include <gdnative/rect2.h>
#include <gdnative/rid.h>
#include <gdnative/string.h>
#include <gdnative/transform.h>
#include <gdnative/transform2d.h>
#include <gdnative/variant.h>
#include <gdnative/vector2.h>
#include <gdnative/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

rebel_variant_type GDAPI rebel_variant_get_type(const rebel_variant* p_v);

void GDAPI
rebel_variant_new_copy(rebel_variant* r_dest, const rebel_variant* p_src);

void GDAPI rebel_variant_new_nil(rebel_variant* r_dest);

void GDAPI rebel_variant_new_bool(rebel_variant* r_dest, const rebel_bool p_b);
void GDAPI rebel_variant_new_uint(rebel_variant* r_dest, const uint64_t p_i);
void GDAPI rebel_variant_new_int(rebel_variant* r_dest, const int64_t p_i);
void GDAPI rebel_variant_new_real(rebel_variant* r_dest, const double p_r);
void GDAPI
rebel_variant_new_string(rebel_variant* r_dest, const rebel_string* p_s);
void GDAPI
rebel_variant_new_vector2(rebel_variant* r_dest, const rebel_vector2* p_v2);
void GDAPI
rebel_variant_new_rect2(rebel_variant* r_dest, const rebel_rect2* p_rect2);
void GDAPI
rebel_variant_new_vector3(rebel_variant* r_dest, const rebel_vector3* p_v3);
void GDAPI rebel_variant_new_transform2d(
    rebel_variant* r_dest,
    const rebel_transform2d* p_t2d
);
void GDAPI
rebel_variant_new_plane(rebel_variant* r_dest, const rebel_plane* p_plane);
void GDAPI
rebel_variant_new_quat(rebel_variant* r_dest, const rebel_quat* p_quat);
void GDAPI
rebel_variant_new_aabb(rebel_variant* r_dest, const rebel_aabb* p_aabb);
void GDAPI
rebel_variant_new_basis(rebel_variant* r_dest, const rebel_basis* p_basis);
void GDAPI rebel_variant_new_transform(
    rebel_variant* r_dest,
    const rebel_transform* p_trans
);
void GDAPI
rebel_variant_new_color(rebel_variant* r_dest, const rebel_color* p_color);
void GDAPI
rebel_variant_new_node_path(rebel_variant* r_dest, const rebel_node_path* p_np);
void GDAPI rebel_variant_new_rid(rebel_variant* r_dest, const rebel_rid* p_rid);
void GDAPI
rebel_variant_new_object(rebel_variant* r_dest, const rebel_object* p_obj);
void GDAPI rebel_variant_new_dictionary(
    rebel_variant* r_dest,
    const rebel_dictionary* p_dict
);
void GDAPI
rebel_variant_new_array(rebel_variant* r_dest, const rebel_array* p_arr);
void GDAPI rebel_variant_new_pool_byte_array(
    rebel_variant* r_dest,
    const rebel_pool_byte_array* p_pba
);
void GDAPI rebel_variant_new_pool_int_array(
    rebel_variant* r_dest,
    const rebel_pool_int_array* p_pia
);
void GDAPI rebel_variant_new_pool_real_array(
    rebel_variant* r_dest,
    const rebel_pool_real_array* p_pra
);
void GDAPI rebel_variant_new_pool_string_array(
    rebel_variant* r_dest,
    const rebel_pool_string_array* p_psa
);
void GDAPI rebel_variant_new_pool_vector2_array(
    rebel_variant* r_dest,
    const rebel_pool_vector2_array* p_pv2a
);
void GDAPI rebel_variant_new_pool_vector3_array(
    rebel_variant* r_dest,
    const rebel_pool_vector3_array* p_pv3a
);
void GDAPI rebel_variant_new_pool_color_array(
    rebel_variant* r_dest,
    const rebel_pool_color_array* p_pca
);

rebel_bool GDAPI rebel_variant_as_bool(const rebel_variant* p_self);
uint64_t GDAPI rebel_variant_as_uint(const rebel_variant* p_self);
int64_t GDAPI rebel_variant_as_int(const rebel_variant* p_self);
double GDAPI rebel_variant_as_real(const rebel_variant* p_self);
rebel_string GDAPI rebel_variant_as_string(const rebel_variant* p_self);
rebel_vector2 GDAPI rebel_variant_as_vector2(const rebel_variant* p_self);
rebel_rect2 GDAPI rebel_variant_as_rect2(const rebel_variant* p_self);
rebel_vector3 GDAPI rebel_variant_as_vector3(const rebel_variant* p_self);
rebel_transform2d GDAPI rebel_variant_as_transform2d(const rebel_variant* p_self
);
rebel_plane GDAPI rebel_variant_as_plane(const rebel_variant* p_self);
rebel_quat GDAPI rebel_variant_as_quat(const rebel_variant* p_self);
rebel_aabb GDAPI rebel_variant_as_aabb(const rebel_variant* p_self);
rebel_basis GDAPI rebel_variant_as_basis(const rebel_variant* p_self);
rebel_transform GDAPI rebel_variant_as_transform(const rebel_variant* p_self);
rebel_color GDAPI rebel_variant_as_color(const rebel_variant* p_self);
rebel_node_path GDAPI rebel_variant_as_node_path(const rebel_variant* p_self);
rebel_rid GDAPI rebel_variant_as_rid(const rebel_variant* p_self);
rebel_object GDAPI* rebel_variant_as_object(const rebel_variant* p_self);
rebel_dictionary GDAPI rebel_variant_as_dictionary(const rebel_variant* p_self);
rebel_array GDAPI rebel_variant_as_array(const rebel_variant* p_self);
rebel_pool_byte_array GDAPI
rebel_variant_as_pool_byte_array(const rebel_variant* p_self);
rebel_pool_int_array GDAPI
rebel_variant_as_pool_int_array(const rebel_variant* p_self);
rebel_pool_real_array GDAPI
rebel_variant_as_pool_real_array(const rebel_variant* p_self);
rebel_pool_string_array GDAPI
rebel_variant_as_pool_string_array(const rebel_variant* p_self);
rebel_pool_vector2_array GDAPI
rebel_variant_as_pool_vector2_array(const rebel_variant* p_self);
rebel_pool_vector3_array GDAPI
rebel_variant_as_pool_vector3_array(const rebel_variant* p_self);
rebel_pool_color_array GDAPI
rebel_variant_as_pool_color_array(const rebel_variant* p_self);

rebel_variant GDAPI rebel_variant_call(
    rebel_variant* p_self,
    const rebel_string* p_method,
    const rebel_variant** p_args,
    const rebel_int p_argcount,
    rebel_variant_call_error* r_error
);

rebel_bool GDAPI rebel_variant_has_method(
    const rebel_variant* p_self,
    const rebel_string* p_method
);

rebel_bool GDAPI rebel_variant_operator_equal(
    const rebel_variant* p_self,
    const rebel_variant* p_other
);
rebel_bool GDAPI rebel_variant_operator_less(
    const rebel_variant* p_self,
    const rebel_variant* p_other
);

rebel_bool GDAPI rebel_variant_hash_compare(
    const rebel_variant* p_self,
    const rebel_variant* p_other
);

rebel_bool GDAPI rebel_variant_booleanize(const rebel_variant* p_self);

void GDAPI rebel_variant_destroy(rebel_variant* p_self);

// GDNative core 1.1

rebel_string GDAPI rebel_variant_get_operator_name(rebel_variant_operator p_op);
void GDAPI rebel_variant_evaluate(
    rebel_variant_operator p_op,
    const rebel_variant* p_a,
    const rebel_variant* p_b,
    rebel_variant* r_ret,
    rebel_bool* r_valid
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_VARIANT_H
