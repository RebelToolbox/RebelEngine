// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gltf_mesh.h"

void GLTFMesh::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_mesh"), &GLTFMesh::get_mesh);
    ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &GLTFMesh::set_mesh);
    ClassDB::bind_method(
        D_METHOD("get_blend_weights"),
        &GLTFMesh::get_blend_weights
    );
    ClassDB::bind_method(
        D_METHOD("set_blend_weights", "blend_weights"),
        &GLTFMesh::set_blend_weights
    );
    ClassDB::bind_method(
        D_METHOD("get_instance_materials"),
        &GLTFMesh::get_instance_materials
    );
    ClassDB::bind_method(
        D_METHOD("set_instance_materials", "instance_materials"),
        &GLTFMesh::set_instance_materials
    );

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh"), "set_mesh", "get_mesh");
    ADD_PROPERTY(
        PropertyInfo(Variant::POOL_REAL_ARRAY, "blend_weights"),
        "set_blend_weights",
        "get_blend_weights"
    ); // Vector<float>
    ADD_PROPERTY(
        PropertyInfo(Variant::ARRAY, "instance_materials"),
        "set_instance_materials",
        "get_instance_materials"
    );
}

Ref<ArrayMesh> GLTFMesh::get_mesh() {
    return mesh;
}

void GLTFMesh::set_mesh(Ref<ArrayMesh> p_mesh) {
    mesh = p_mesh;
}

Array GLTFMesh::get_instance_materials() {
    return instance_materials;
}

void GLTFMesh::set_instance_materials(Array p_instance_materials) {
    instance_materials = p_instance_materials;
}

Vector<float> GLTFMesh::get_blend_weights() {
    return blend_weights;
}

void GLTFMesh::set_blend_weights(Vector<float> p_blend_weights) {
    blend_weights = p_blend_weights;
}
