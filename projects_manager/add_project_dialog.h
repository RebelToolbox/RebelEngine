// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ADD_PROJECT_DIALOG_H
#define ADD_PROJECT_DIALOG_H

#include "core/ustring.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/texture_rect.h"

class AddProjectDialog : public ConfirmationDialog {
    GDCLASS(AddProjectDialog, ConfirmationDialog);

public:
    AddProjectDialog();

    void show_dialog();

protected:
    static void _bind_methods();
    virtual void ok_pressed();

private:
    FileDialog* file_dialog;

    Label* error_message_label;

    LineEdit* project_file_line_edit;

    TextureRect* project_file_error_icon;

    void _check_file(const String& p_file);
    void _on_browse_buton_pressed();
    void _on_file_selected(const String& p_path);
    void _on_project_file_text_changed(const String& p_text);
};

#endif // ADD_PROJECT_DIALOG_H
