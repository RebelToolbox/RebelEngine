// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "rid_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "core/resource.h"

RID* rebel_icall_RID_Ctor(Object* p_from) {
    Resource* res_from = Object::cast_to<Resource>(p_from);

    if (res_from) {
        return memnew(RID(res_from->get_rid()));
    }

    return memnew(RID);
}

void rebel_icall_RID_Dtor(RID* p_ptr) {
    ERR_FAIL_NULL(p_ptr);
    memdelete(p_ptr);
}

uint32_t rebel_icall_RID_get_id(RID* p_ptr) {
    return p_ptr->get_id();
}

void rebel_register_rid_icalls() {
    GDMonoUtils::add_internal_call(
        "Rebel.RID::rebel_icall_RID_Ctor",
        rebel_icall_RID_Ctor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.RID::rebel_icall_RID_Dtor",
        rebel_icall_RID_Dtor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.RID::rebel_icall_RID_get_id",
        rebel_icall_RID_get_id
    );
}

#endif // MONO_GLUE_ENABLED
