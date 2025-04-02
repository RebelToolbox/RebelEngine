// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RID_GLUE_H
#define RID_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "../mono_gd/gd_mono_marshal.h"
#include "core/object.h"
#include "core/rid.h"

RID* rebel_icall_RID_Ctor(Object* p_from);

void rebel_icall_RID_Dtor(RID* p_ptr);

uint32_t rebel_icall_RID_get_id(RID* p_ptr);

// Register internal calls

void rebel_register_rid_icalls();

#endif // MONO_GLUE_ENABLED

#endif // RID_GLUE_H
