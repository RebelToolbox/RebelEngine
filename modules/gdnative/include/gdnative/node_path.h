// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDNATIVE_NODE_PATH_H
#define GDNATIVE_NODE_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define REBEL_NODE_PATH_SIZE sizeof(void*)

#ifndef REBEL_CORE_API_REBEL_NODE_PATH_TYPE_DEFINED
#define REBEL_CORE_API_REBEL_NODE_PATH_TYPE_DEFINED

typedef struct {
    uint8_t _dont_touch_that[REBEL_NODE_PATH_SIZE];
} rebel_node_path;
#endif

// reduce extern "C" nesting for VS2013
#ifdef __cplusplus
}
#endif

#include <gdnative/gdnative.h>
#include <gdnative/string.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI
rebel_node_path_new(rebel_node_path* r_dest, const rebel_string* p_from);
void GDAPI
rebel_node_path_new_copy(rebel_node_path* r_dest, const rebel_node_path* p_src);
void GDAPI rebel_node_path_destroy(rebel_node_path* p_self);

rebel_string GDAPI rebel_node_path_as_string(const rebel_node_path* p_self);

rebel_bool GDAPI rebel_node_path_is_absolute(const rebel_node_path* p_self);

rebel_int GDAPI rebel_node_path_get_name_count(const rebel_node_path* p_self);

rebel_string GDAPI
rebel_node_path_get_name(const rebel_node_path* p_self, const rebel_int p_idx);

rebel_int GDAPI rebel_node_path_get_subname_count(const rebel_node_path* p_self
);

rebel_string GDAPI rebel_node_path_get_subname(
    const rebel_node_path* p_self,
    const rebel_int p_idx
);

rebel_string GDAPI
rebel_node_path_get_concatenated_subnames(const rebel_node_path* p_self);

rebel_bool GDAPI rebel_node_path_is_empty(const rebel_node_path* p_self);

rebel_bool GDAPI rebel_node_path_operator_equal(
    const rebel_node_path* p_self,
    const rebel_node_path* p_b
);

rebel_node_path rebel_node_path_get_as_property_path(
    const rebel_node_path* p_self
);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_NODE_PATH_H
