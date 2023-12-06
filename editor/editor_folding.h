// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_FOLDING_H
#define EDITOR_FOLDING_H

#include "scene/main/node.h"

class EditorFolding {
    PoolVector<String> _get_unfolds(const Object* p_object);
    void _set_unfolds(Object* p_object, const PoolVector<String>& p_unfolds);

    void _fill_folds(
        const Node* p_root,
        const Node* p_node,
        Array& p_folds,
        Array& resource_folds,
        Array& nodes_folded,
        Set<RES>& resources
    );

    void _do_object_unfolds(Object* p_object, Set<RES>& resources);
    void _do_node_unfolds(Node* p_root, Node* p_node, Set<RES>& resources);

public:
    void save_resource_folding(const RES& p_resource, const String& p_path);
    void load_resource_folding(RES p_resource, const String& p_path);

    void save_scene_folding(const Node* p_scene, const String& p_path);
    void load_scene_folding(Node* p_scene, const String& p_path);

    void unfold_scene(Node* p_scene);

    bool has_folding_data(const String& p_path);

    EditorFolding();
};

#endif // EDITOR_FOLDING_H
