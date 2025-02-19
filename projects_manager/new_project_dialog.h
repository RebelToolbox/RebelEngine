// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NEW_PROJECT_DIALOG_H
#define NEW_PROJECT_DIALOG_H

#include "core/ustring.h"
#include "scene/gui/button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/texture_rect.h"

class NewProjectDialog : public ConfirmationDialog {
    GDCLASS(NewProjectDialog, ConfirmationDialog);

public:
    NewProjectDialog();

    void show_dialog();

protected:
    static void _bind_methods();
    virtual void cancel_pressed();
    virtual void ok_pressed();

private:
    String created_folder;

    AcceptDialog* popup_message;

    Button* create_folder_button;
    Button* project_folder_browse_button;
    Ref<ButtonGroup> renderer_button_group;

    FileDialog* select_project_folder_dialog;

    Label* error_message_label;

    LineEdit* project_name_line_edit;
    LineEdit* project_folder_line_edit;

    TextureRect* project_folder_error_icon;

    void _on_browse_button_pressed();
    void _on_create_folder_button_pressed();
    void _on_project_folder_selected(const String& p_folder);
    void _on_project_folder_text_changed(const String& p_path);
    void _remove_created_folder();
    void _set_error_message(const String& p_error_message);
    void _test_project_folder();
};

#endif // NEW_PROJECT_DIALOG_H
