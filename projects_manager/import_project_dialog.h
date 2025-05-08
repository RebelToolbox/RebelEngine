// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IMPORT_PROJECT_DIALOG_H
#define IMPORT_PROJECT_DIALOG_H

#include "scene/gui/dialogs.h"

class FileDialog;
class Label;
class LineEdit;
class OptionButton;
class TextureRect;
class String;

class ImportProjectDialog : public ConfirmationDialog {
    GDCLASS(ImportProjectDialog, ConfirmationDialog);

public:
    ImportProjectDialog();

    void show_dialog();

protected:
    static void _bind_methods();
    virtual void ok_pressed();

private:
    enum class ProjectType {
        GODOT,
    };

    AcceptDialog* invalid_destination_folder_error;
    AcceptDialog* invalid_project_file_error;

    FileDialog* destination_folder_dialog;
    FileDialog* project_file_dialog;

    Label* destination_folder_error_message_label;
    Label* project_file_error_message_label;

    LineEdit* destination_folder_line_edit;
    LineEdit* project_file_line_edit;

    OptionButton* project_type_option_button;

    TextureRect* destination_folder_error_icon;
    TextureRect* project_file_error_icon;

    ProjectType project_type;

    bool _is_destination_folder_valid();
    bool _is_project_file_valid();
    void _on_destination_folder_browse_buton_pressed();
    void _on_destination_folder_selected(const String& path);
    void _on_destination_folder_text_changed(const String&);
    void _on_project_file_browse_buton_pressed();
    void _on_project_file_selected(const String& path);
    void _on_project_file_text_changed(const String&);
    void _on_project_type_selected(int index);
    void _set_project_file_dialog_filters();
    void _update_destination_folder_status(bool valid);
    void _update_project_file_status(bool valid);
    void _update_status();
};

#endif // IMPORT_PROJECT_DIALOG_H
