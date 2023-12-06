// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MULTI_NODE_EDIT_H
#define MULTI_NODE_EDIT_H

#include "scene/main/node.h"

class MultiNodeEdit : public Reference {
    GDCLASS(MultiNodeEdit, Reference);

    List<NodePath> nodes;

    struct PLData {
        int uses;
        PropertyInfo info;
    };

    bool _set_impl(
        const StringName& p_name,
        const Variant& p_value,
        const String& p_field
    );

protected:
    bool _set(const StringName& p_name, const Variant& p_value);
    bool _get(const StringName& p_name, Variant& r_ret) const;
    void _get_property_list(List<PropertyInfo>* p_list) const;

public:
    void clear_nodes();
    void add_node(const NodePath& p_node);

    int get_node_count() const;
    NodePath get_node(int p_index) const;

    void set_property_field(
        const StringName& p_property,
        const Variant& p_value,
        const String& p_field
    );

    MultiNodeEdit();
};

#endif // MULTI_NODE_EDIT_H
