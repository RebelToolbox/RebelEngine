// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MULTIMESH_INSTANCE_H
#define MULTIMESH_INSTANCE_H

#include "scene/3d/visual_instance.h"
#include "scene/resources/multimesh.h"

class MultiMeshInstance : public GeometryInstance {
    GDCLASS(MultiMeshInstance, GeometryInstance);

    Ref<MultiMesh> multimesh;

protected:
    static void _bind_methods();
    // bind helpers

public:
    virtual PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;

    void set_multimesh(const Ref<MultiMesh>& p_multimesh);
    Ref<MultiMesh> get_multimesh() const;

    virtual AABB get_aabb() const;

    MultiMeshInstance();
    ~MultiMeshInstance();
};

#endif // MULTIMESH_INSTANCE_H
