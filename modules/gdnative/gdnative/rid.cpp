// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/rid.h"

#include "core/resource.h"
#include "core/rid.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(godot_rid) == sizeof(RID), "RID size mismatch");

void GDAPI godot_rid_new(godot_rid* r_dest) {
    RID* dest = (RID*)r_dest;
    memnew_placement(dest, RID);
}

godot_int GDAPI godot_rid_get_id(const godot_rid* p_self) {
    const RID* self = (const RID*)p_self;
    return self->get_id();
}

void GDAPI
godot_rid_new_with_resource(godot_rid* r_dest, const godot_object* p_from) {
    const Resource* res_from = Object::cast_to<Resource>((Object*)p_from);
    godot_rid_new(r_dest);
    if (res_from) {
        RID* dest = (RID*)r_dest;
        *dest     = RID(res_from->get_rid());
    }
}

godot_bool GDAPI
godot_rid_operator_equal(const godot_rid* p_self, const godot_rid* p_b) {
    const RID* self = (const RID*)p_self;
    const RID* b    = (const RID*)p_b;
    return *self == *b;
}

godot_bool GDAPI
godot_rid_operator_less(const godot_rid* p_self, const godot_rid* p_b) {
    const RID* self = (const RID*)p_self;
    const RID* b    = (const RID*)p_b;
    return *self < *b;
}

#ifdef __cplusplus
}
#endif
