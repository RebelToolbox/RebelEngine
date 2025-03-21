// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_POOL_ARRAYS_H
#define GDNATIVE_POOL_ARRAYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/////// Read Access

#define REBEL_POOL_ARRAY_READ_ACCESS_SIZE 1

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_ARRAY_READ_ACCESS_SIZE];
} rebel_pool_array_read_access;

typedef rebel_pool_array_read_access rebel_pool_byte_array_read_access;
typedef rebel_pool_array_read_access rebel_pool_int_array_read_access;
typedef rebel_pool_array_read_access rebel_pool_real_array_read_access;
typedef rebel_pool_array_read_access rebel_pool_string_array_read_access;
typedef rebel_pool_array_read_access rebel_pool_vector2_array_read_access;
typedef rebel_pool_array_read_access rebel_pool_vector3_array_read_access;
typedef rebel_pool_array_read_access rebel_pool_color_array_read_access;

/////// Write Access

#define REBEL_POOL_ARRAY_WRITE_ACCESS_SIZE 1

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_ARRAY_WRITE_ACCESS_SIZE];
} rebel_pool_array_write_access;

typedef rebel_pool_array_write_access rebel_pool_byte_array_write_access;
typedef rebel_pool_array_write_access rebel_pool_int_array_write_access;
typedef rebel_pool_array_write_access rebel_pool_real_array_write_access;
typedef rebel_pool_array_write_access rebel_pool_string_array_write_access;
typedef rebel_pool_array_write_access rebel_pool_vector2_array_write_access;
typedef rebel_pool_array_write_access rebel_pool_vector3_array_write_access;
typedef rebel_pool_array_write_access rebel_pool_color_array_write_access;

/////// PoolByteArray

#define REBEL_POOL_BYTE_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_POOL_BYTE_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_POOL_BYTE_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_BYTE_ARRAY_SIZE];
} rebel_pool_byte_array;
#endif

/////// PoolIntArray

#define REBEL_POOL_INT_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_POOL_INT_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_POOL_INT_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_INT_ARRAY_SIZE];
} rebel_pool_int_array;
#endif

/////// PoolRealArray

#define REBEL_POOL_REAL_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_POOL_REAL_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_POOL_REAL_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_REAL_ARRAY_SIZE];
} rebel_pool_real_array;
#endif

/////// PoolStringArray

#define REBEL_POOL_STRING_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_POOL_STRING_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_POOL_STRING_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_STRING_ARRAY_SIZE];
} rebel_pool_string_array;
#endif

/////// PoolVector2Array

#define REBEL_POOL_VECTOR2_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_POOL_VECTOR2_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_POOL_VECTOR2_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_VECTOR2_ARRAY_SIZE];
} rebel_pool_vector2_array;
#endif

/////// PoolVector3Array

#define REBEL_POOL_VECTOR3_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_POOL_VECTOR3_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_POOL_VECTOR3_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_VECTOR3_ARRAY_SIZE];
} rebel_pool_vector3_array;
#endif

/////// PoolColorArray

#define REBEL_POOL_COLOR_ARRAY_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_POOL_COLOR_ARRAY_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_POOL_COLOR_ARRAY_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_POOL_COLOR_ARRAY_SIZE];
} rebel_pool_color_array;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/array.h>
#include <gdnative/color.h>
#include <gdnative/gdnative.h>
#include <gdnative/vector2.h>
#include <gdnative/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

// byte

void GDAPI rebel_pool_byte_array_new(rebel_pool_byte_array* r_dest);
void GDAPI rebel_pool_byte_array_new_copy(
    rebel_pool_byte_array* r_dest,
    const rebel_pool_byte_array* p_src
);
void GDAPI rebel_pool_byte_array_new_with_array(
    rebel_pool_byte_array* r_dest,
    const rebel_array* p_a
);

void GDAPI rebel_pool_byte_array_append(
    rebel_pool_byte_array* p_self,
    const uint8_t p_data
);

void GDAPI rebel_pool_byte_array_append_array(
    rebel_pool_byte_array* p_self,
    const rebel_pool_byte_array* p_array
);

rebel_error GDAPI rebel_pool_byte_array_insert(
    rebel_pool_byte_array* p_self,
    const rebel_int p_idx,
    const uint8_t p_data
);

void GDAPI rebel_pool_byte_array_invert(rebel_pool_byte_array* p_self);

void GDAPI rebel_pool_byte_array_push_back(
    rebel_pool_byte_array* p_self,
    const uint8_t p_data
);

void GDAPI rebel_pool_byte_array_remove(
    rebel_pool_byte_array* p_self,
    const rebel_int p_idx
);

void GDAPI rebel_pool_byte_array_resize(
    rebel_pool_byte_array* p_self,
    const rebel_int p_size
);

rebel_pool_byte_array_read_access GDAPI* rebel_pool_byte_array_read(
    const rebel_pool_byte_array* p_self
);

rebel_pool_byte_array_write_access GDAPI* rebel_pool_byte_array_write(
    rebel_pool_byte_array* p_self
);

void GDAPI rebel_pool_byte_array_set(
    rebel_pool_byte_array* p_self,
    const rebel_int p_idx,
    const uint8_t p_data
);
uint8_t GDAPI rebel_pool_byte_array_get(
    const rebel_pool_byte_array* p_self,
    const rebel_int p_idx
);

rebel_int GDAPI rebel_pool_byte_array_size(const rebel_pool_byte_array* p_self);

rebel_bool GDAPI rebel_pool_byte_array_empty(const rebel_pool_byte_array* p_self
);

void GDAPI rebel_pool_byte_array_destroy(rebel_pool_byte_array* p_self);

// int

void GDAPI rebel_pool_int_array_new(rebel_pool_int_array* r_dest);
void GDAPI rebel_pool_int_array_new_copy(
    rebel_pool_int_array* r_dest,
    const rebel_pool_int_array* p_src
);
void GDAPI rebel_pool_int_array_new_with_array(
    rebel_pool_int_array* r_dest,
    const rebel_array* p_a
);

void GDAPI rebel_pool_int_array_append(
    rebel_pool_int_array* p_self,
    const rebel_int p_data
);

void GDAPI rebel_pool_int_array_append_array(
    rebel_pool_int_array* p_self,
    const rebel_pool_int_array* p_array
);

rebel_error GDAPI rebel_pool_int_array_insert(
    rebel_pool_int_array* p_self,
    const rebel_int p_idx,
    const rebel_int p_data
);

void GDAPI rebel_pool_int_array_invert(rebel_pool_int_array* p_self);

void GDAPI rebel_pool_int_array_push_back(
    rebel_pool_int_array* p_self,
    const rebel_int p_data
);

void GDAPI rebel_pool_int_array_remove(
    rebel_pool_int_array* p_self,
    const rebel_int p_idx
);

void GDAPI rebel_pool_int_array_resize(
    rebel_pool_int_array* p_self,
    const rebel_int p_size
);

rebel_pool_int_array_read_access GDAPI* rebel_pool_int_array_read(
    const rebel_pool_int_array* p_self
);

rebel_pool_int_array_write_access GDAPI* rebel_pool_int_array_write(
    rebel_pool_int_array* p_self
);

void GDAPI rebel_pool_int_array_set(
    rebel_pool_int_array* p_self,
    const rebel_int p_idx,
    const rebel_int p_data
);
rebel_int GDAPI rebel_pool_int_array_get(
    const rebel_pool_int_array* p_self,
    const rebel_int p_idx
);

rebel_int GDAPI rebel_pool_int_array_size(const rebel_pool_int_array* p_self);

rebel_bool GDAPI rebel_pool_int_array_empty(const rebel_pool_int_array* p_self);

void GDAPI rebel_pool_int_array_destroy(rebel_pool_int_array* p_self);

// real

void GDAPI rebel_pool_real_array_new(rebel_pool_real_array* r_dest);
void GDAPI rebel_pool_real_array_new_copy(
    rebel_pool_real_array* r_dest,
    const rebel_pool_real_array* p_src
);
void GDAPI rebel_pool_real_array_new_with_array(
    rebel_pool_real_array* r_dest,
    const rebel_array* p_a
);

void GDAPI rebel_pool_real_array_append(
    rebel_pool_real_array* p_self,
    const rebel_real p_data
);

void GDAPI rebel_pool_real_array_append_array(
    rebel_pool_real_array* p_self,
    const rebel_pool_real_array* p_array
);

rebel_error GDAPI rebel_pool_real_array_insert(
    rebel_pool_real_array* p_self,
    const rebel_int p_idx,
    const rebel_real p_data
);

void GDAPI rebel_pool_real_array_invert(rebel_pool_real_array* p_self);

void GDAPI rebel_pool_real_array_push_back(
    rebel_pool_real_array* p_self,
    const rebel_real p_data
);

void GDAPI rebel_pool_real_array_remove(
    rebel_pool_real_array* p_self,
    const rebel_int p_idx
);

void GDAPI rebel_pool_real_array_resize(
    rebel_pool_real_array* p_self,
    const rebel_int p_size
);

rebel_pool_real_array_read_access GDAPI* rebel_pool_real_array_read(
    const rebel_pool_real_array* p_self
);

rebel_pool_real_array_write_access GDAPI* rebel_pool_real_array_write(
    rebel_pool_real_array* p_self
);

void GDAPI rebel_pool_real_array_set(
    rebel_pool_real_array* p_self,
    const rebel_int p_idx,
    const rebel_real p_data
);
rebel_real GDAPI rebel_pool_real_array_get(
    const rebel_pool_real_array* p_self,
    const rebel_int p_idx
);

rebel_int GDAPI rebel_pool_real_array_size(const rebel_pool_real_array* p_self);

rebel_bool GDAPI rebel_pool_real_array_empty(const rebel_pool_real_array* p_self
);

void GDAPI rebel_pool_real_array_destroy(rebel_pool_real_array* p_self);

// string

void GDAPI rebel_pool_string_array_new(rebel_pool_string_array* r_dest);
void GDAPI rebel_pool_string_array_new_copy(
    rebel_pool_string_array* r_dest,
    const rebel_pool_string_array* p_src
);
void GDAPI rebel_pool_string_array_new_with_array(
    rebel_pool_string_array* r_dest,
    const rebel_array* p_a
);

void GDAPI rebel_pool_string_array_append(
    rebel_pool_string_array* p_self,
    const rebel_string* p_data
);

void GDAPI rebel_pool_string_array_append_array(
    rebel_pool_string_array* p_self,
    const rebel_pool_string_array* p_array
);

rebel_error GDAPI rebel_pool_string_array_insert(
    rebel_pool_string_array* p_self,
    const rebel_int p_idx,
    const rebel_string* p_data
);

void GDAPI rebel_pool_string_array_invert(rebel_pool_string_array* p_self);

void GDAPI rebel_pool_string_array_push_back(
    rebel_pool_string_array* p_self,
    const rebel_string* p_data
);

void GDAPI rebel_pool_string_array_remove(
    rebel_pool_string_array* p_self,
    const rebel_int p_idx
);

void GDAPI rebel_pool_string_array_resize(
    rebel_pool_string_array* p_self,
    const rebel_int p_size
);

rebel_pool_string_array_read_access GDAPI* rebel_pool_string_array_read(
    const rebel_pool_string_array* p_self
);

rebel_pool_string_array_write_access GDAPI* rebel_pool_string_array_write(
    rebel_pool_string_array* p_self
);

void GDAPI rebel_pool_string_array_set(
    rebel_pool_string_array* p_self,
    const rebel_int p_idx,
    const rebel_string* p_data
);
rebel_string GDAPI rebel_pool_string_array_get(
    const rebel_pool_string_array* p_self,
    const rebel_int p_idx
);

rebel_int GDAPI
rebel_pool_string_array_size(const rebel_pool_string_array* p_self);

rebel_bool GDAPI
rebel_pool_string_array_empty(const rebel_pool_string_array* p_self);

void GDAPI rebel_pool_string_array_destroy(rebel_pool_string_array* p_self);

// vector2

void GDAPI rebel_pool_vector2_array_new(rebel_pool_vector2_array* r_dest);
void GDAPI rebel_pool_vector2_array_new_copy(
    rebel_pool_vector2_array* r_dest,
    const rebel_pool_vector2_array* p_src
);
void GDAPI rebel_pool_vector2_array_new_with_array(
    rebel_pool_vector2_array* r_dest,
    const rebel_array* p_a
);

void GDAPI rebel_pool_vector2_array_append(
    rebel_pool_vector2_array* p_self,
    const rebel_vector2* p_data
);

void GDAPI rebel_pool_vector2_array_append_array(
    rebel_pool_vector2_array* p_self,
    const rebel_pool_vector2_array* p_array
);

rebel_error GDAPI rebel_pool_vector2_array_insert(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_idx,
    const rebel_vector2* p_data
);

void GDAPI rebel_pool_vector2_array_invert(rebel_pool_vector2_array* p_self);

void GDAPI rebel_pool_vector2_array_push_back(
    rebel_pool_vector2_array* p_self,
    const rebel_vector2* p_data
);

void GDAPI rebel_pool_vector2_array_remove(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_idx
);

void GDAPI rebel_pool_vector2_array_resize(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_size
);

rebel_pool_vector2_array_read_access GDAPI* rebel_pool_vector2_array_read(
    const rebel_pool_vector2_array* p_self
);

rebel_pool_vector2_array_write_access GDAPI* rebel_pool_vector2_array_write(
    rebel_pool_vector2_array* p_self
);

void GDAPI rebel_pool_vector2_array_set(
    rebel_pool_vector2_array* p_self,
    const rebel_int p_idx,
    const rebel_vector2* p_data
);
rebel_vector2 GDAPI rebel_pool_vector2_array_get(
    const rebel_pool_vector2_array* p_self,
    const rebel_int p_idx
);

rebel_int GDAPI
rebel_pool_vector2_array_size(const rebel_pool_vector2_array* p_self);

rebel_bool GDAPI
rebel_pool_vector2_array_empty(const rebel_pool_vector2_array* p_self);

void GDAPI rebel_pool_vector2_array_destroy(rebel_pool_vector2_array* p_self);

// vector3

void GDAPI rebel_pool_vector3_array_new(rebel_pool_vector3_array* r_dest);
void GDAPI rebel_pool_vector3_array_new_copy(
    rebel_pool_vector3_array* r_dest,
    const rebel_pool_vector3_array* p_src
);
void GDAPI rebel_pool_vector3_array_new_with_array(
    rebel_pool_vector3_array* r_dest,
    const rebel_array* p_a
);

void GDAPI rebel_pool_vector3_array_append(
    rebel_pool_vector3_array* p_self,
    const rebel_vector3* p_data
);

void GDAPI rebel_pool_vector3_array_append_array(
    rebel_pool_vector3_array* p_self,
    const rebel_pool_vector3_array* p_array
);

rebel_error GDAPI rebel_pool_vector3_array_insert(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_idx,
    const rebel_vector3* p_data
);

void GDAPI rebel_pool_vector3_array_invert(rebel_pool_vector3_array* p_self);

void GDAPI rebel_pool_vector3_array_push_back(
    rebel_pool_vector3_array* p_self,
    const rebel_vector3* p_data
);

void GDAPI rebel_pool_vector3_array_remove(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_idx
);

void GDAPI rebel_pool_vector3_array_resize(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_size
);

rebel_pool_vector3_array_read_access GDAPI* rebel_pool_vector3_array_read(
    const rebel_pool_vector3_array* p_self
);

rebel_pool_vector3_array_write_access GDAPI* rebel_pool_vector3_array_write(
    rebel_pool_vector3_array* p_self
);

void GDAPI rebel_pool_vector3_array_set(
    rebel_pool_vector3_array* p_self,
    const rebel_int p_idx,
    const rebel_vector3* p_data
);
rebel_vector3 GDAPI rebel_pool_vector3_array_get(
    const rebel_pool_vector3_array* p_self,
    const rebel_int p_idx
);

rebel_int GDAPI
rebel_pool_vector3_array_size(const rebel_pool_vector3_array* p_self);

rebel_bool GDAPI
rebel_pool_vector3_array_empty(const rebel_pool_vector3_array* p_self);

void GDAPI rebel_pool_vector3_array_destroy(rebel_pool_vector3_array* p_self);

// color

void GDAPI rebel_pool_color_array_new(rebel_pool_color_array* r_dest);
void GDAPI rebel_pool_color_array_new_copy(
    rebel_pool_color_array* r_dest,
    const rebel_pool_color_array* p_src
);
void GDAPI rebel_pool_color_array_new_with_array(
    rebel_pool_color_array* r_dest,
    const rebel_array* p_a
);

void GDAPI rebel_pool_color_array_append(
    rebel_pool_color_array* p_self,
    const rebel_color* p_data
);

void GDAPI rebel_pool_color_array_append_array(
    rebel_pool_color_array* p_self,
    const rebel_pool_color_array* p_array
);

rebel_error GDAPI rebel_pool_color_array_insert(
    rebel_pool_color_array* p_self,
    const rebel_int p_idx,
    const rebel_color* p_data
);

void GDAPI rebel_pool_color_array_invert(rebel_pool_color_array* p_self);

void GDAPI rebel_pool_color_array_push_back(
    rebel_pool_color_array* p_self,
    const rebel_color* p_data
);

void GDAPI rebel_pool_color_array_remove(
    rebel_pool_color_array* p_self,
    const rebel_int p_idx
);

void GDAPI rebel_pool_color_array_resize(
    rebel_pool_color_array* p_self,
    const rebel_int p_size
);

rebel_pool_color_array_read_access GDAPI* rebel_pool_color_array_read(
    const rebel_pool_color_array* p_self
);

rebel_pool_color_array_write_access GDAPI* rebel_pool_color_array_write(
    rebel_pool_color_array* p_self
);

void GDAPI rebel_pool_color_array_set(
    rebel_pool_color_array* p_self,
    const rebel_int p_idx,
    const rebel_color* p_data
);
rebel_color GDAPI rebel_pool_color_array_get(
    const rebel_pool_color_array* p_self,
    const rebel_int p_idx
);

rebel_int GDAPI rebel_pool_color_array_size(const rebel_pool_color_array* p_self
);

rebel_bool GDAPI
rebel_pool_color_array_empty(const rebel_pool_color_array* p_self);

void GDAPI rebel_pool_color_array_destroy(rebel_pool_color_array* p_self);

//
// read accessor functions
//

rebel_pool_byte_array_read_access GDAPI* rebel_pool_byte_array_read_access_copy(
    const rebel_pool_byte_array_read_access* p_other
);
const uint8_t GDAPI* rebel_pool_byte_array_read_access_ptr(
    const rebel_pool_byte_array_read_access* p_read
);
void GDAPI rebel_pool_byte_array_read_access_operator_assign(
    rebel_pool_byte_array_read_access* p_read,
    rebel_pool_byte_array_read_access* p_other
);
void GDAPI rebel_pool_byte_array_read_access_destroy(
    rebel_pool_byte_array_read_access* p_read
);

rebel_pool_int_array_read_access GDAPI* rebel_pool_int_array_read_access_copy(
    const rebel_pool_int_array_read_access* p_other
);
const rebel_int GDAPI* rebel_pool_int_array_read_access_ptr(
    const rebel_pool_int_array_read_access* p_read
);
void GDAPI rebel_pool_int_array_read_access_operator_assign(
    rebel_pool_int_array_read_access* p_read,
    rebel_pool_int_array_read_access* p_other
);
void GDAPI rebel_pool_int_array_read_access_destroy(
    rebel_pool_int_array_read_access* p_read
);

rebel_pool_real_array_read_access GDAPI* rebel_pool_real_array_read_access_copy(
    const rebel_pool_real_array_read_access* p_other
);
const rebel_real GDAPI* rebel_pool_real_array_read_access_ptr(
    const rebel_pool_real_array_read_access* p_read
);
void GDAPI rebel_pool_real_array_read_access_operator_assign(
    rebel_pool_real_array_read_access* p_read,
    rebel_pool_real_array_read_access* p_other
);
void GDAPI rebel_pool_real_array_read_access_destroy(
    rebel_pool_real_array_read_access* p_read
);

rebel_pool_string_array_read_access GDAPI*
rebel_pool_string_array_read_access_copy(
    const rebel_pool_string_array_read_access* p_other
);
const rebel_string GDAPI* rebel_pool_string_array_read_access_ptr(
    const rebel_pool_string_array_read_access* p_read
);
void GDAPI rebel_pool_string_array_read_access_operator_assign(
    rebel_pool_string_array_read_access* p_read,
    rebel_pool_string_array_read_access* p_other
);
void GDAPI rebel_pool_string_array_read_access_destroy(
    rebel_pool_string_array_read_access* p_read
);

rebel_pool_vector2_array_read_access GDAPI*
rebel_pool_vector2_array_read_access_copy(
    const rebel_pool_vector2_array_read_access* p_other
);
const rebel_vector2 GDAPI* rebel_pool_vector2_array_read_access_ptr(
    const rebel_pool_vector2_array_read_access* p_read
);
void GDAPI rebel_pool_vector2_array_read_access_operator_assign(
    rebel_pool_vector2_array_read_access* p_read,
    rebel_pool_vector2_array_read_access* p_other
);
void GDAPI rebel_pool_vector2_array_read_access_destroy(
    rebel_pool_vector2_array_read_access* p_read
);

rebel_pool_vector3_array_read_access GDAPI*
rebel_pool_vector3_array_read_access_copy(
    const rebel_pool_vector3_array_read_access* p_other
);
const rebel_vector3 GDAPI* rebel_pool_vector3_array_read_access_ptr(
    const rebel_pool_vector3_array_read_access* p_read
);
void GDAPI rebel_pool_vector3_array_read_access_operator_assign(
    rebel_pool_vector3_array_read_access* p_read,
    rebel_pool_vector3_array_read_access* p_other
);
void GDAPI rebel_pool_vector3_array_read_access_destroy(
    rebel_pool_vector3_array_read_access* p_read
);

rebel_pool_color_array_read_access GDAPI*
rebel_pool_color_array_read_access_copy(
    const rebel_pool_color_array_read_access* p_other
);
const rebel_color GDAPI* rebel_pool_color_array_read_access_ptr(
    const rebel_pool_color_array_read_access* p_read
);
void GDAPI rebel_pool_color_array_read_access_operator_assign(
    rebel_pool_color_array_read_access* p_read,
    rebel_pool_color_array_read_access* p_other
);
void GDAPI rebel_pool_color_array_read_access_destroy(
    rebel_pool_color_array_read_access* p_read
);

//
// write accessor functions
//

rebel_pool_byte_array_write_access GDAPI*
rebel_pool_byte_array_write_access_copy(
    const rebel_pool_byte_array_write_access* p_other
);
uint8_t GDAPI* rebel_pool_byte_array_write_access_ptr(
    const rebel_pool_byte_array_write_access* p_write
);
void GDAPI rebel_pool_byte_array_write_access_operator_assign(
    rebel_pool_byte_array_write_access* p_write,
    rebel_pool_byte_array_write_access* p_other
);
void GDAPI rebel_pool_byte_array_write_access_destroy(
    rebel_pool_byte_array_write_access* p_write
);

rebel_pool_int_array_write_access GDAPI* rebel_pool_int_array_write_access_copy(
    const rebel_pool_int_array_write_access* p_other
);
rebel_int GDAPI* rebel_pool_int_array_write_access_ptr(
    const rebel_pool_int_array_write_access* p_write
);
void GDAPI rebel_pool_int_array_write_access_operator_assign(
    rebel_pool_int_array_write_access* p_write,
    rebel_pool_int_array_write_access* p_other
);
void GDAPI rebel_pool_int_array_write_access_destroy(
    rebel_pool_int_array_write_access* p_write
);

rebel_pool_real_array_write_access GDAPI*
rebel_pool_real_array_write_access_copy(
    const rebel_pool_real_array_write_access* p_other
);
rebel_real GDAPI* rebel_pool_real_array_write_access_ptr(
    const rebel_pool_real_array_write_access* p_write
);
void GDAPI rebel_pool_real_array_write_access_operator_assign(
    rebel_pool_real_array_write_access* p_write,
    rebel_pool_real_array_write_access* p_other
);
void GDAPI rebel_pool_real_array_write_access_destroy(
    rebel_pool_real_array_write_access* p_write
);

rebel_pool_string_array_write_access GDAPI*
rebel_pool_string_array_write_access_copy(
    const rebel_pool_string_array_write_access* p_other
);
rebel_string GDAPI* rebel_pool_string_array_write_access_ptr(
    const rebel_pool_string_array_write_access* p_write
);
void GDAPI rebel_pool_string_array_write_access_operator_assign(
    rebel_pool_string_array_write_access* p_write,
    rebel_pool_string_array_write_access* p_other
);
void GDAPI rebel_pool_string_array_write_access_destroy(
    rebel_pool_string_array_write_access* p_write
);

rebel_pool_vector2_array_write_access GDAPI*
rebel_pool_vector2_array_write_access_copy(
    const rebel_pool_vector2_array_write_access* p_other
);
rebel_vector2 GDAPI* rebel_pool_vector2_array_write_access_ptr(
    const rebel_pool_vector2_array_write_access* p_write
);
void GDAPI rebel_pool_vector2_array_write_access_operator_assign(
    rebel_pool_vector2_array_write_access* p_write,
    rebel_pool_vector2_array_write_access* p_other
);
void GDAPI rebel_pool_vector2_array_write_access_destroy(
    rebel_pool_vector2_array_write_access* p_write
);

rebel_pool_vector3_array_write_access GDAPI*
rebel_pool_vector3_array_write_access_copy(
    const rebel_pool_vector3_array_write_access* p_other
);
rebel_vector3 GDAPI* rebel_pool_vector3_array_write_access_ptr(
    const rebel_pool_vector3_array_write_access* p_write
);
void GDAPI rebel_pool_vector3_array_write_access_operator_assign(
    rebel_pool_vector3_array_write_access* p_write,
    rebel_pool_vector3_array_write_access* p_other
);
void GDAPI rebel_pool_vector3_array_write_access_destroy(
    rebel_pool_vector3_array_write_access* p_write
);

rebel_pool_color_array_write_access GDAPI*
rebel_pool_color_array_write_access_copy(
    const rebel_pool_color_array_write_access* p_other
);
rebel_color GDAPI* rebel_pool_color_array_write_access_ptr(
    const rebel_pool_color_array_write_access* p_write
);
void GDAPI rebel_pool_color_array_write_access_operator_assign(
    rebel_pool_color_array_write_access* p_write,
    rebel_pool_color_array_write_access* p_other
);
void GDAPI rebel_pool_color_array_write_access_destroy(
    rebel_pool_color_array_write_access* p_write
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_POOL_ARRAYS_H
