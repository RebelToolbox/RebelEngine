// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "fbx_bone.h"

#include "fbx_node.h"
#include "import_state.h"

Ref<FBXNode> FBXSkinDeformer::get_link(const ImportState& state) const {
    print_verbose("bone name: " + bone->bone_name);

    // safe for when deformers must be polyfilled when skin has different count
    // of binds to bones in the scene ;)
    if (!cluster) {
        return nullptr;
    }

    ERR_FAIL_COND_V_MSG(
        cluster->TargetNode() == nullptr,
        nullptr,
        "bone has invalid target node"
    );

    Ref<FBXNode> link_node;
    uint64_t id = cluster->TargetNode()->ID();
    if (state.fbx_target_map.has(id)) {
        link_node = state.fbx_target_map[id];
    } else {
        print_error("link node not found for " + itos(id));
    }

    // the node in space this is for, like if it's FOR a target.
    return link_node;
}
