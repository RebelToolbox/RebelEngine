// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EXTRACT_ZIP_FILE_DIALOG_H
#define EXTRACT_ZIP_FILE_DIALOG_H

#include "core/ustring.h"
#include "scene/gui/button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/texture_rect.h"

class ExtractZipFileDialog : public ConfirmationDialog {
    GDCLASS(ExtractZipFileDialog, ConfirmationDialog);

public:
    ExtractZipFileDialog();

    void show_dialog(
        const String& p_zip_file_path,
        const String& p_project_name
    );

protected:
    static void _bind_methods();
    virtual void cancel_pressed();
    virtual void ok_pressed();

private:
    String created_folder;
    String zip_file_path;

    AcceptDialog* popup_message;

    Button* create_folder_button;
    Button* project_folder_browse_button;

    FileDialog* select_project_folder_dialog;

    Label* error_message_label;

    LineEdit* project_name_line_edit;
    LineEdit* project_folder_line_edit;

    TextureRect* project_folder_error_icon;

    void _on_browse_button_pressed();
    void _on_create_folder_button_pressed();
    void _on_project_folder_selected(const String& p_folder);
    void _on_project_folder_text_changed(const String& p_text);
    void _remove_created_folder();
    void _set_error_message(const String& p_error_message);
    void _test_project_folder();
};

#endif // EXTRACT_ZIP_FILE_DIALOG_H
