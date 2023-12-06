// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "rid_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "core/resource.h"

RID* godot_icall_RID_Ctor(Object* p_from) {
    Resource* res_from = Object::cast_to<Resource>(p_from);

    if (res_from) {
        return memnew(RID(res_from->get_rid()));
    }

    return memnew(RID);
}

void godot_icall_RID_Dtor(RID* p_ptr) {
    ERR_FAIL_NULL(p_ptr);
    memdelete(p_ptr);
}

uint32_t godot_icall_RID_get_id(RID* p_ptr) {
    return p_ptr->get_id();
}

void godot_register_rid_icalls() {
    GDMonoUtils::add_internal_call(
        "Godot.RID::godot_icall_RID_Ctor",
        godot_icall_RID_Ctor
    );
    GDMonoUtils::add_internal_call(
        "Godot.RID::godot_icall_RID_Dtor",
        godot_icall_RID_Dtor
    );
    GDMonoUtils::add_internal_call(
        "Godot.RID::godot_icall_RID_get_id",
        godot_icall_RID_get_id
    );
}

#endif // MONO_GLUE_ENABLED
