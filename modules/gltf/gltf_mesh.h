// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_MESH_H
#define GLTF_MESH_H

#include "core/resource.h"
#include "editor/import/resource_importer_scene.h"
#include "scene/resources/mesh.h"

class GLTFMesh : public Resource {
    GDCLASS(GLTFMesh, Resource);

private:
    Ref<ArrayMesh> mesh;
    Vector<float> blend_weights;
    Array instance_materials;

protected:
    static void _bind_methods();

public:
    Ref<ArrayMesh> get_mesh();
    void set_mesh(Ref<ArrayMesh> p_mesh);
    Vector<float> get_blend_weights();
    void set_blend_weights(Vector<float> p_blend_weights);
    Array get_instance_materials();
    void set_instance_materials(Array p_instance_materials);
};
#endif // GLTF_MESH_H
