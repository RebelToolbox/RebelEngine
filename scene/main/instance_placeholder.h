// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef INSTANCE_PLACEHOLDER_H
#define INSTANCE_PLACEHOLDER_H

#include "scene/main/node.h"

class PackedScene;

class InstancePlaceholder : public Node {
    GDCLASS(InstancePlaceholder, Node);

    String path;

    struct PropSet {
        StringName name;
        Variant value;
    };

    List<PropSet> stored_values;

protected:
    bool _set(const StringName& p_name, const Variant& p_value);
    bool _get(const StringName& p_name, Variant& r_ret) const;
    void _get_property_list(List<PropertyInfo>* p_list) const;

    static void _bind_methods();

public:
    void set_instance_path(const String& p_name);
    String get_instance_path() const;

    Dictionary get_stored_values(bool p_with_order = false);

    Node* create_instance(
        bool p_replace                         = false,
        const Ref<PackedScene>& p_custom_scene = Ref<PackedScene>()
    );
    void replace_by_instance(
        const Ref<PackedScene>& p_custom_scene = Ref<PackedScene>()
    );

    InstancePlaceholder();
};

#endif // INSTANCE_PLACEHOLDER_H
