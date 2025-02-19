// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RENAME_PROJECT_DIALOG_H
#define RENAME_PROJECT_DIALOG_H

#include "core/ustring.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/texture_rect.h"

class RenameProjectDialog : public ConfirmationDialog {
    GDCLASS(RenameProjectDialog, ConfirmationDialog);

public:
    RenameProjectDialog();

    void show_dialog(const String& p_folder);

protected:
    static void _bind_methods();
    virtual void ok_pressed();

private:
    LineEdit* project_name_line_edit;
    Label* project_folder_label;
    TextureRect* project_path_error_icon;
    Label* error_message;

    String project_folder;
};

#endif // RENAME_PROJECT_DIALOG_H
