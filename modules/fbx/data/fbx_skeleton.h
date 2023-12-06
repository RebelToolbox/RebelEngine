// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FBX_SKELETON_H
#define FBX_SKELETON_H

#include "core/reference.h"
#include "fbx_bone.h"
#include "fbx_node.h"
#include "model_abstraction.h"
#include "scene/3d/skeleton.h"

struct FBXNode;
struct ImportState;
struct FBXBone;

struct FBXSkeleton : Reference {
    Ref<FBXNode> fbx_node               = Ref<FBXNode>();
    Vector<Ref<FBXBone>> skeleton_bones = Vector<Ref<FBXBone>>();
    Skeleton* skeleton                  = nullptr;

    void init_skeleton(const ImportState& state);
};

#endif // FBX_SKELETON_H
