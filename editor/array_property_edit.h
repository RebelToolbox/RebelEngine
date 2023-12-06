// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ARRAY_PROPERTY_EDIT_H
#define ARRAY_PROPERTY_EDIT_H

#include "scene/main/node.h"

class ArrayPropertyEdit : public Reference {
    GDCLASS(ArrayPropertyEdit, Reference);

    int page;
    ObjectID obj;
    StringName property;
    String vtypes;
    String subtype_hint_string;
    PropertyHint subtype_hint;
    Variant::Type subtype;
    Variant get_array() const;
    Variant::Type default_type;

    void _notif_change();
    void _notif_changev(const String& p_v);
    void _set_size(int p_size);
    void _set_value(int p_idx, const Variant& p_value);

    bool _dont_undo_redo();

protected:
    static void _bind_methods();
    bool _set(const StringName& p_name, const Variant& p_value);
    bool _get(const StringName& p_name, Variant& r_ret) const;
    void _get_property_list(List<PropertyInfo>* p_list) const;

public:
    void edit(
        Object* p_obj,
        const StringName& p_prop,
        const String& p_hint_string,
        Variant::Type p_deftype
    );

    Node* get_node();

    ArrayPropertyEdit();
};

#endif // ARRAY_PROPERTY_EDIT_H
