// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gltf_skeleton.h"

void GLTFSkeleton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_joints"), &GLTFSkeleton::get_joints);
    ClassDB::bind_method(
        D_METHOD("set_joints", "joints"),
        &GLTFSkeleton::set_joints
    );
    ClassDB::bind_method(D_METHOD("get_roots"), &GLTFSkeleton::get_roots);
    ClassDB::bind_method(
        D_METHOD("set_roots", "roots"),
        &GLTFSkeleton::set_roots
    );
    ClassDB::bind_method(D_METHOD("get_skeleton"), &GLTFSkeleton::get_skeleton);
    ClassDB::bind_method(
        D_METHOD("get_unique_names"),
        &GLTFSkeleton::get_unique_names
    );
    ClassDB::bind_method(
        D_METHOD("set_unique_names", "unique_names"),
        &GLTFSkeleton::set_unique_names
    );
    ClassDB::bind_method(
        D_METHOD("get_bone_node"),
        &GLTFSkeleton::get_bone_node
    );
    ClassDB::bind_method(
        D_METHOD("set_bone_node", "bone_node"),
        &GLTFSkeleton::set_bone_node
    );
    ClassDB::bind_method(
        D_METHOD("get_bone_attachment_count"),
        &GLTFSkeleton::get_bone_attachment_count
    );
    ClassDB::bind_method(
        D_METHOD("get_bone_attachment", "idx"),
        &GLTFSkeleton::get_bone_attachment
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::POOL_INT_ARRAY, "joints"),
        "set_joints",
        "get_joints"
    ); // PoolVector<GLTFNodeIndex>
    ADD_PROPERTY(
        PropertyInfo(Variant::POOL_INT_ARRAY, "roots"),
        "set_roots",
        "get_roots"
    ); // PoolVector<GLTFNodeIndex>
    ADD_PROPERTY(
        PropertyInfo(
            Variant::ARRAY,
            "unique_names",
            PROPERTY_HINT_NONE,
            "",
            PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL
                | PROPERTY_USAGE_EDITOR
        ),
        "set_unique_names",
        "get_unique_names"
    ); // Set<String>
    ADD_PROPERTY(
        PropertyInfo(
            Variant::DICTIONARY,
            "bone_node",
            PROPERTY_HINT_NONE,
            "",
            PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL
                | PROPERTY_USAGE_EDITOR
        ),
        "set_bone_node",
        "get_bone_node"
    ); // Map<int32_t,
}

PoolVector<GLTFNodeIndex> GLTFSkeleton::get_joints() {
    return joints;
}

void GLTFSkeleton::set_joints(PoolVector<GLTFNodeIndex> p_joints) {
    joints = p_joints;
}

PoolVector<GLTFNodeIndex> GLTFSkeleton::get_roots() {
    return roots;
}

void GLTFSkeleton::set_roots(PoolVector<GLTFNodeIndex> p_roots) {
    roots = p_roots;
}

Skeleton* GLTFSkeleton::get_skeleton() {
    return skeleton;
}

Array GLTFSkeleton::get_unique_names() {
    return GLTFDocument::to_array(unique_names);
}

void GLTFSkeleton::set_unique_names(Array p_unique_names) {
    GLTFDocument::set_from_array(unique_names, p_unique_names);
}

Dictionary GLTFSkeleton::get_bone_node() {
    return GLTFDocument::to_dict(bone_node);
}

void GLTFSkeleton::set_bone_node(Dictionary p_indict) {
    GLTFDocument::set_from_dict(bone_node, p_indict);
}

BoneAttachment* GLTFSkeleton::get_bone_attachment(int idx) {
    ERR_FAIL_INDEX_V(idx, bone_attachments.size(), nullptr);
    return bone_attachments[idx];
}

int32_t GLTFSkeleton::get_bone_attachment_count() {
    return bone_attachments.size();
}
