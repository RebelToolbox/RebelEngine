// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "nodepath_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "core/ustring.h"

NodePath* rebel_icall_NodePath_Ctor(MonoString* p_path) {
    return memnew(NodePath(GDMonoMarshal::mono_string_to_rebel(p_path)));
}

void rebel_icall_NodePath_Dtor(NodePath* p_ptr) {
    ERR_FAIL_NULL(p_ptr);
    memdelete(p_ptr);
}

MonoString* rebel_icall_NodePath_operator_String(NodePath* p_np) {
    return GDMonoMarshal::mono_string_from_rebel(p_np->operator String());
}

MonoBoolean rebel_icall_NodePath_is_absolute(NodePath* p_ptr) {
    return (MonoBoolean)p_ptr->is_absolute();
}

uint32_t rebel_icall_NodePath_get_name_count(NodePath* p_ptr) {
    return p_ptr->get_name_count();
}

MonoString* rebel_icall_NodePath_get_name(NodePath* p_ptr, uint32_t p_idx) {
    return GDMonoMarshal::mono_string_from_rebel(p_ptr->get_name(p_idx));
}

uint32_t rebel_icall_NodePath_get_subname_count(NodePath* p_ptr) {
    return p_ptr->get_subname_count();
}

MonoString* rebel_icall_NodePath_get_subname(NodePath* p_ptr, uint32_t p_idx) {
    return GDMonoMarshal::mono_string_from_rebel(p_ptr->get_subname(p_idx));
}

MonoString* rebel_icall_NodePath_get_concatenated_subnames(NodePath* p_ptr) {
    return GDMonoMarshal::mono_string_from_rebel(
        p_ptr->get_concatenated_subnames()
    );
}

NodePath* rebel_icall_NodePath_get_as_property_path(NodePath* p_ptr) {
    return memnew(NodePath(p_ptr->get_as_property_path()));
}

MonoBoolean rebel_icall_NodePath_is_empty(NodePath* p_ptr) {
    return (MonoBoolean)p_ptr->is_empty();
}

void rebel_register_nodepath_icalls() {
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_Ctor",
        rebel_icall_NodePath_Ctor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_Dtor",
        rebel_icall_NodePath_Dtor
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_operator_String",
        rebel_icall_NodePath_operator_String
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_get_as_property_path",
        rebel_icall_NodePath_get_as_property_path
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_get_concatenated_subnames",
        rebel_icall_NodePath_get_concatenated_subnames
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_get_name",
        rebel_icall_NodePath_get_name
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_get_name_count",
        rebel_icall_NodePath_get_name_count
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_get_subname",
        rebel_icall_NodePath_get_subname
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_get_subname_count",
        rebel_icall_NodePath_get_subname_count
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_is_absolute",
        rebel_icall_NodePath_is_absolute
    );
    GDMonoUtils::add_internal_call(
        "Rebel.NodePath::rebel_icall_NodePath_is_empty",
        rebel_icall_NodePath_is_empty
    );
}

#endif // MONO_GLUE_ENABLED
