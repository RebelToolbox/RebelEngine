// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_SKIN_H
#define GLTF_SKIN_H

#include "core/resource.h"
#include "gltf_document.h"

class GLTFSkin : public Resource {
    GDCLASS(GLTFSkin, Resource);
    friend class GLTFDocument;

private:
    // The "skeleton" property defined in the gltf spec. -1 = Scene Root
    GLTFNodeIndex skin_root = -1;

    Vector<GLTFNodeIndex> joints_original;
    Vector<Transform> inverse_binds;

    // Note: joints + non_joints should form a complete subtree, or subtrees
    // with a common parent

    // All nodes that are skins that are caught in-between the original joints
    // (inclusive of joints_original)
    Vector<GLTFNodeIndex> joints;

    // All Nodes that are caught in-between skin joint nodes, and are not
    // defined as joints by any skin
    Vector<GLTFNodeIndex> non_joints;

    // The roots of the skin. In the case of multiple roots, their parent *must*
    // be the same (the roots must be siblings)
    Vector<GLTFNodeIndex> roots;

    // The GLTF Skeleton this Skin points to (after we determine skeletons)
    GLTFSkeletonIndex skeleton = -1;

    // A mapping from the joint indices (in the order of joints_original) to the
    // Rebel Skeleton's bone_indices
    Map<int, int> joint_i_to_bone_i;
    Map<int, StringName> joint_i_to_name;

    // The Actual Skin that will be created as a mapping between the IBM's of
    // this skin to the generated skeleton for the mesh instances.
    Ref<Skin> skin;

protected:
    static void _bind_methods();

public:
    GLTFNodeIndex get_skin_root();
    void set_skin_root(GLTFNodeIndex p_skin_root);

    Vector<GLTFNodeIndex> get_joints_original();
    void set_joints_original(Vector<GLTFNodeIndex> p_joints_original);

    Array get_inverse_binds();
    void set_inverse_binds(Array p_inverse_binds);

    Vector<GLTFNodeIndex> get_joints();
    void set_joints(Vector<GLTFNodeIndex> p_joints);

    Vector<GLTFNodeIndex> get_non_joints();
    void set_non_joints(Vector<GLTFNodeIndex> p_non_joints);

    Vector<GLTFNodeIndex> get_roots();
    void set_roots(Vector<GLTFNodeIndex> p_roots);

    int get_skeleton();
    void set_skeleton(int p_skeleton);

    Dictionary get_joint_i_to_bone_i();
    void set_joint_i_to_bone_i(Dictionary p_joint_i_to_bone_i);

    Dictionary get_joint_i_to_name();
    void set_joint_i_to_name(Dictionary p_joint_i_to_name);

    Ref<Skin> get_skin();
    void set_skin(Ref<Skin> p_skin);
};
#endif // GLTF_SKIN_H
