// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_QUICK_OPEN_H
#define EDITOR_QUICK_OPEN_H

#include "core/pair.h"
#include "editor_file_system.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"

class EditorQuickOpen : public ConfirmationDialog {
    GDCLASS(EditorQuickOpen, ConfirmationDialog);

    LineEdit* search_box;
    Tree* search_options;
    StringName base_type;
    StringName ei;
    StringName ot;

    void _update_search();

    void _sbox_input(const Ref<InputEvent>& p_ie);
    void _parse_fs(
        EditorFileSystemDirectory* efsd,
        Vector<Pair<String, Ref<Texture>>>& list
    );
    Vector<Pair<String, Ref<Texture>>> _sort_fs(
        Vector<Pair<String, Ref<Texture>>>& list
    );
    float _path_cmp(String search, String path) const;

    void _confirmed();
    void _text_changed(const String& p_newtext);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    StringName get_base_type() const;

    String get_selected() const;
    Vector<String> get_selected_files() const;

    void popup_dialog(
        const StringName& p_base,
        bool p_enable_multi = false,
        bool p_dontclear    = false
    );
    EditorQuickOpen();
};

#endif // EDITOR_QUICK_OPEN_H
