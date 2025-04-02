// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FBX_NODE_H
#define FBX_NODE_H

#include "fbx_parser/FBXDocument.h"
#include "fbx_skeleton.h"
#include "model_abstraction.h"
#include "pivot_transform.h"

class Spatial;
struct PivotTransform;

struct FBXNode : Reference, ModelAbstraction {
    uint64_t current_node_id = 0;
    String node_name         = String();
    Spatial* rebel_node      = nullptr;

    // used to parent the skeleton once the tree is built.
    Ref<FBXSkeleton> skeleton_node = Ref<FBXSkeleton>();

    void set_parent(Ref<FBXNode> p_parent) {
        fbx_parent = p_parent;
    }

    void set_pivot_transform(Ref<PivotTransform> p_pivot_transform) {
        pivot_transform = p_pivot_transform;
    }

    Ref<PivotTransform> pivot_transform =
        Ref<PivotTransform>();                // local and global xform data
    Ref<FBXNode> fbx_parent = Ref<FBXNode>(); // parent node
};

#endif // FBX_NODE_H
