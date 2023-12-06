// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef DICTIONARY_PROPERTY_EDIT_H
#define DICTIONARY_PROPERTY_EDIT_H

#include "scene/main/node.h"

class DictionaryPropertyEdit : public Reference {
    GDCLASS(DictionaryPropertyEdit, Reference);

    ObjectID obj;
    StringName property;

    void _notif_change();
    void _notif_changev(const String& p_v);
    void _set_key(const Variant& p_old_key, const Variant& p_new_key);
    void _set_value(const Variant& p_key, const Variant& p_value);

    Variant get_dictionary() const;

    bool _dont_undo_redo();

protected:
    static void _bind_methods();
    bool _set(const StringName& p_name, const Variant& p_value);
    bool _get(const StringName& p_name, Variant& r_ret) const;
    void _get_property_list(List<PropertyInfo>* p_list) const;

public:
    void edit(Object* p_obj, const StringName& p_prop);

    Node* get_node();

    DictionaryPropertyEdit();
};

#endif // DICTIONARY_PROPERTY_EDIT_H
