// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_PRELOADER_H
#define RESOURCE_PRELOADER_H

#include "scene/main/node.h"

class ResourcePreloader : public Node {
    GDCLASS(ResourcePreloader, Node);

    Map<StringName, RES> resources;

    void _set_resources(const Array& p_data);
    Array _get_resources() const;
    PoolVector<String> _get_resource_list() const;

protected:
    static void _bind_methods();

public:
    void add_resource(const StringName& p_name, const RES& p_resource);
    void remove_resource(const StringName& p_name);
    void rename_resource(
        const StringName& p_from_name,
        const StringName& p_to_name
    );
    bool has_resource(const StringName& p_name) const;
    RES get_resource(const StringName& p_name) const;

    void get_resource_list(List<StringName>* p_list);

    ResourcePreloader();
};

#endif // RESOURCE_PRELOADER_H
