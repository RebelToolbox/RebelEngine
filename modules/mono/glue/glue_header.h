// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef MONO_GLUE_ENABLED

#include "base_object_glue.h"
#include "collections_glue.h"
#include "gd_glue.h"
#include "nodepath_glue.h"
#include "rid_glue.h"
#include "string_glue.h"

/**
 * Registers internal calls that were not generated. This function is called
 * from the generated RebelSharpBindings::register_generated_icalls() function.
 */
void rebel_register_glue_header_icalls() {
    rebel_register_collections_icalls();
    rebel_register_gd_icalls();
    rebel_register_nodepath_icalls();
    rebel_register_object_icalls();
    rebel_register_rid_icalls();
    rebel_register_string_icalls();
}

// Used by the generated glue

#include "../mono_gd/gd_mono_class.h"
#include "../mono_gd/gd_mono_internals.h"
#include "../mono_gd/gd_mono_utils.h"
#include "core/array.h"
#include "core/class_db.h"
#include "core/dictionary.h"
#include "core/engine.h"
#include "core/method_bind.h"
#include "core/node_path.h"
#include "core/object.h"
#include "core/reference.h"
#include "core/typedefs.h"
#include "core/ustring.h"

#define REBELSHARP_INSTANCE_OBJECT(m_instance, m_type)                         \
    static ClassDB::ClassInfo* ci = NULL;                                      \
    if (!ci) {                                                                 \
        ci = ClassDB::classes.getptr(m_type);                                  \
    }                                                                          \
    Object* m_instance = ci->creation_func();

#include "arguments_vector.h"

#endif // MONO_GLUE_ENABLED
