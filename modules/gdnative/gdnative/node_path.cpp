// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/node_path.h"

#include "core/node_path.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_node_path) == sizeof(NodePath),
    "NodePath size mismatch"
);

void GDAPI
rebel_node_path_new(rebel_node_path* r_dest, const rebel_string* p_from) {
    NodePath* dest     = (NodePath*)r_dest;
    const String* from = (const String*)p_from;
    memnew_placement(dest, NodePath(*from));
}

void GDAPI rebel_node_path_new_copy(
    rebel_node_path* r_dest,
    const rebel_node_path* p_src
) {
    NodePath* dest      = (NodePath*)r_dest;
    const NodePath* src = (const NodePath*)p_src;
    memnew_placement(dest, NodePath(*src));
}

void GDAPI rebel_node_path_destroy(rebel_node_path* p_self) {
    NodePath* self = (NodePath*)p_self;
    self->~NodePath();
}

rebel_string GDAPI rebel_node_path_as_string(const rebel_node_path* p_self) {
    rebel_string ret;
    const NodePath* self = (const NodePath*)p_self;
    memnew_placement(&ret, String(*self));
    return ret;
}

rebel_bool GDAPI rebel_node_path_is_absolute(const rebel_node_path* p_self) {
    const NodePath* self = (const NodePath*)p_self;
    return self->is_absolute();
}

rebel_int GDAPI rebel_node_path_get_name_count(const rebel_node_path* p_self) {
    const NodePath* self = (const NodePath*)p_self;
    return self->get_name_count();
}

rebel_string GDAPI
rebel_node_path_get_name(const rebel_node_path* p_self, const rebel_int p_idx) {
    rebel_string dest;
    const NodePath* self = (const NodePath*)p_self;

    memnew_placement(&dest, String(self->get_name(p_idx)));
    return dest;
}

rebel_int GDAPI rebel_node_path_get_subname_count(const rebel_node_path* p_self
) {
    const NodePath* self = (const NodePath*)p_self;
    return self->get_subname_count();
}

rebel_string GDAPI rebel_node_path_get_subname(
    const rebel_node_path* p_self,
    const rebel_int p_idx
) {
    rebel_string dest;
    const NodePath* self = (const NodePath*)p_self;

    memnew_placement(&dest, String(self->get_subname(p_idx)));
    return dest;
}

rebel_string GDAPI
rebel_node_path_get_concatenated_subnames(const rebel_node_path* p_self) {
    rebel_string dest;
    const NodePath* self = (const NodePath*)p_self;
    memnew_placement(&dest, String(self->get_concatenated_subnames()));
    return dest;
}

rebel_bool GDAPI rebel_node_path_is_empty(const rebel_node_path* p_self) {
    const NodePath* self = (const NodePath*)p_self;
    return self->is_empty();
}

rebel_bool GDAPI rebel_node_path_operator_equal(
    const rebel_node_path* p_self,
    const rebel_node_path* p_b
) {
    const NodePath* self = (const NodePath*)p_self;
    const NodePath* b    = (const NodePath*)p_b;
    return *self == *b;
}

rebel_node_path rebel_node_path_get_as_property_path(
    const rebel_node_path* p_self
) {
    const NodePath* self = (const NodePath*)p_self;
    rebel_node_path res;
    NodePath* val = (NodePath*)&res;
    memnew_placement(val, NodePath);
    *val = self->get_as_property_path();
    return res;
}

#ifdef __cplusplus
}
#endif
