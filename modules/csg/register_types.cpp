// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "csg_gizmos.h"
#include "csg_shape.h"

void register_csg_types() {
#ifndef _3D_DISABLED

    ClassDB::register_virtual_class<CSGShape>();
    ClassDB::register_virtual_class<CSGPrimitive>();
    ClassDB::register_class<CSGMesh>();
    ClassDB::register_class<CSGSphere>();
    ClassDB::register_class<CSGBox>();
    ClassDB::register_class<CSGCylinder>();
    ClassDB::register_class<CSGTorus>();
    ClassDB::register_class<CSGPolygon>();
    ClassDB::register_class<CSGCombiner>();

#ifdef TOOLS_ENABLED
    EditorPlugins::add_by_type<EditorPluginCSG>();
#endif
#endif
}

void unregister_csg_types() {}
