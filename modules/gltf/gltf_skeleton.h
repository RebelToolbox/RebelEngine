// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_SKELETON_H
#define GLTF_SKELETON_H

#include "core/resource.h"
#include "gltf_document.h"

class GLTFSkeleton : public Resource {
    GDCLASS(GLTFSkeleton, Resource);
    friend class GLTFDocument;

private:
    // The *synthesized* skeletons joints
    PoolVector<GLTFNodeIndex> joints;

    // The roots of the skeleton. If there are multiple, each root must have the
    // same parent (ie roots are siblings)
    PoolVector<GLTFNodeIndex> roots;

    // The created Skeleton for the scene
    Skeleton* skeleton = nullptr;

    // Set of unique bone names for the skeleton
    Set<String> unique_names;

    Map<int32_t, GLTFNodeIndex> bone_node;

    PoolVector<BoneAttachment*> bone_attachments;

protected:
    static void _bind_methods();

public:
    PoolVector<GLTFNodeIndex> get_joints();
    void set_joints(PoolVector<GLTFNodeIndex> p_joints);

    PoolVector<GLTFNodeIndex> get_roots();
    void set_roots(PoolVector<GLTFNodeIndex> p_roots);

    Skeleton* get_skeleton();

    Array get_unique_names();
    void set_unique_names(Array p_unique_names);

    Dictionary get_bone_node();
    void set_bone_node(Dictionary p_indict);

    BoneAttachment* get_bone_attachment(int idx);

    int32_t get_bone_attachment_count();
};
#endif // GLTF_SKELETON_H
