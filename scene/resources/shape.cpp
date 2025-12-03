// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "shape.h"

#include "core/os/os.h"
#include "scene/main/scene_tree.h"
#include "scene/resources/mesh.h"
#include "servers/physics_server.h"

void Shape::add_vertices_to_array(
    PoolVector<Vector3>& array,
    const Transform& p_xform
) {
    Vector<Vector3> toadd = get_debug_mesh_lines();

    if (toadd.size()) {
        int base = array.size();
        array.resize(base + toadd.size());
        PoolVector<Vector3>::Write w = array.write();
        for (int i = 0; i < toadd.size(); i++) {
            w[i + base] = p_xform.xform(toadd[i]);
        }
    }
}

real_t Shape::get_margin() const {
    return margin;
}

void Shape::set_margin(real_t p_margin) {
    margin = p_margin;
    PhysicsServer::get_singleton()->shape_set_margin(shape, margin);
}

Ref<ArrayMesh> Shape::get_debug_mesh() {
    if (debug_mesh_cache.is_valid()) {
        return debug_mesh_cache;
    }

    Vector<Vector3> lines = get_debug_mesh_lines();

    debug_mesh_cache = Ref<ArrayMesh>(memnew(ArrayMesh));

    if (!lines.empty()) {
        // make mesh
        PoolVector<Vector3> array;
        array.resize(lines.size());
        {
            PoolVector<Vector3>::Write w = array.write();
            for (int i = 0; i < lines.size(); i++) {
                w[i] = lines[i];
            }
        }

        Array arr;
        arr.resize(Mesh::ARRAY_MAX);
        arr[Mesh::ARRAY_VERTEX] = array;

        SceneTree* st =
            Object::cast_to<SceneTree>(OS::get_singleton()->get_main_loop());

        debug_mesh_cache->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, arr);

        if (st) {
            debug_mesh_cache->surface_set_material(
                0,
                st->get_debug_collision_material()
            );
        }
    }

    return debug_mesh_cache;
}

void Shape::_update_shape() {
    emit_changed();
    debug_mesh_cache.unref();
}

void Shape::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_margin", "margin"), &Shape::set_margin);
    ClassDB::bind_method(D_METHOD("get_margin"), &Shape::get_margin);

    ClassDB::bind_method(D_METHOD("get_debug_mesh"), &Shape::get_debug_mesh);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::REAL,
            "margin",
            PROPERTY_HINT_RANGE,
            "0.001,10,0.001"
        ),
        "set_margin",
        "get_margin"
    );
}

Shape::Shape() : margin(0.04f) {
    ERR_PRINT("Constructor must not be called!");
}

Shape::Shape(RID p_shape) : margin(0.04f) {
    shape = p_shape;
}

Shape::~Shape() {
    PhysicsServer::get_singleton()->free(shape);
}
