// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_DIR_DIALOG_H
#define EDITOR_DIR_DIALOG_H

#include "core/os/dir_access.h"
#include "editor/editor_file_system.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"

class EditorDirDialog : public ConfirmationDialog {
    GDCLASS(EditorDirDialog, ConfirmationDialog);

    ConfirmationDialog* makedialog;
    LineEdit* makedirname;
    AcceptDialog* mkdirerr;

    Button* makedir;
    Set<String> opened_paths;

    Tree* tree;
    bool updating;

    void _item_collapsed(Object* p_item);
    void _update_dir(
        TreeItem* p_item,
        EditorFileSystemDirectory* p_dir,
        const String& p_select_path = String()
    );

    void _make_dir();
    void _make_dir_confirm();

    void ok_pressed();

    bool must_reload;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void reload(const String& p_path = "");
    EditorDirDialog();
};

#endif // EDITOR_DIR_DIALOG_H
