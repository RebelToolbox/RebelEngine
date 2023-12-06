// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "multimesh_instance.h"

void MultiMeshInstance::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_multimesh", "multimesh"),
        &MultiMeshInstance::set_multimesh
    );
    ClassDB::bind_method(
        D_METHOD("get_multimesh"),
        &MultiMeshInstance::get_multimesh
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::OBJECT,
            "multimesh",
            PROPERTY_HINT_RESOURCE_TYPE,
            "MultiMesh"
        ),
        "set_multimesh",
        "get_multimesh"
    );
}

void MultiMeshInstance::set_multimesh(const Ref<MultiMesh>& p_multimesh) {
    multimesh = p_multimesh;
    if (multimesh.is_valid()) {
        set_base(multimesh->get_rid());
    } else {
        set_base(RID());
    }
}

Ref<MultiMesh> MultiMeshInstance::get_multimesh() const {
    return multimesh;
}

PoolVector<Face3> MultiMeshInstance::get_faces(uint32_t p_usage_flags) const {
    return PoolVector<Face3>();
}

AABB MultiMeshInstance::get_aabb() const {
    if (multimesh.is_null()) {
        return AABB();
    } else {
        return multimesh->get_aabb();
    }
}

MultiMeshInstance::MultiMeshInstance() {}

MultiMeshInstance::~MultiMeshInstance() {}
