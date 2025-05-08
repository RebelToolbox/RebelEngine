// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "import_project_dialog.h"

#include "core/ustring.h"
#include "core/version.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_themes.h"
#include "scene/gui/button.h"
#include "scene/gui/container.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/texture_rect.h"

ImportProjectDialog::ImportProjectDialog() {
    set_theme(create_custom_theme());
    set_title(TTR("Import Project"));
    get_ok()->set_text(TTR("Import"));

    VBoxContainer* dialog_container = memnew(VBoxContainer);
    add_child(dialog_container);

    HBoxContainer* project_type_container = memnew((HBoxContainer));
    dialog_container->add_child(project_type_container);

    Label* project_type_heading = memnew(Label);
    project_type_heading->set_text(TTR("Project Type:"));
    project_type_container->add_child(project_type_heading);

    project_type_option_button = memnew(OptionButton);
    project_type_option_button->add_item(
        TTR("Godot Project"),
        (int)ProjectType::GODOT
    );
    project_type_option_button
        ->connect("item_selected", this, "_on_project_type_selected");
    project_type_container->add_child(project_type_option_button);

    Label* project_file_heading = memnew(Label);
    project_file_heading->set_text(TTR("Project File:"));
    dialog_container->add_child(project_file_heading);

    HBoxContainer* project_file_container = memnew(HBoxContainer);
    dialog_container->add_child(project_file_container);

    project_file_line_edit = memnew(LineEdit);
    project_file_line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
    project_file_line_edit
        ->connect("text_changed", this, "_on_project_file_text_changed");
    project_file_container->add_child(project_file_line_edit);

    project_file_error_icon = memnew(TextureRect);
    project_file_error_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED
    );
    project_file_container->add_child(project_file_error_icon);

    Button* project_file_browse_button = memnew(Button);
    project_file_browse_button->set_text(TTR("Browse"));
    project_file_browse_button
        ->connect("pressed", this, "_on_project_file_browse_buton_pressed");
    project_file_container->add_child(project_file_browse_button);

    project_file_error_message_label = memnew(Label);
    project_file_error_message_label->set_text(
        TTR("Please choose a \"project.godot\" file.")
    );
    project_file_error_message_label->set_align(Label::ALIGN_CENTER);
    project_file_error_message_label->add_color_override(
        "font_color",
        get_color("error_color", "Editor")
    );
    dialog_container->add_child(project_file_error_message_label);

    Label* destination_folder_heading = memnew(Label);
    destination_folder_heading->set_text(TTR("Destination Folder:"));
    dialog_container->add_child(destination_folder_heading);

    HBoxContainer* destination_folder_container = memnew(HBoxContainer);
    dialog_container->add_child(destination_folder_container);

    destination_folder_line_edit = memnew(LineEdit);
    destination_folder_line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
    destination_folder_line_edit
        ->connect("text_changed", this, "_on_destination_folder_text_changed");
    destination_folder_container->add_child(destination_folder_line_edit);

    destination_folder_error_icon = memnew(TextureRect);
    destination_folder_error_icon->set_stretch_mode(
        TextureRect::STRETCH_KEEP_CENTERED
    );
    destination_folder_container->add_child(destination_folder_error_icon);

    Button* destination_folder_browse_button = memnew(Button);
    destination_folder_browse_button->set_text(TTR("Browse"));
    destination_folder_browse_button->connect(
        "pressed",
        this,
        "_on_destination_folder_browse_buton_pressed"
    );
    destination_folder_container->add_child(destination_folder_browse_button);

    destination_folder_error_message_label = memnew(Label);
    destination_folder_error_message_label->set_text(
        TTR("Please choose an empty folder.")
    );
    destination_folder_error_message_label->set_align(Label::ALIGN_CENTER);
    destination_folder_error_message_label->add_color_override(
        "font_color",
        get_color("error_color", "Editor")
    );
    dialog_container->add_child(destination_folder_error_message_label);

    invalid_destination_folder_error = memnew(AcceptDialog);
    invalid_destination_folder_error->set_text(
        TTR("Please select a valid destination folder.")
    );
    add_child(invalid_destination_folder_error);

    invalid_project_file_error = memnew(AcceptDialog);
    invalid_project_file_error->set_text(
        TTR("Please select a valid project file.")
    );
    add_child(invalid_project_file_error);

    destination_folder_dialog = memnew(FileDialog);
    destination_folder_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
    destination_folder_dialog->set_mode(FileDialog::MODE_OPEN_DIR);
    destination_folder_dialog
        ->connect("dir_selected", this, "_on_destination_folder_selected");
    add_child(destination_folder_dialog);

    project_file_dialog = memnew(FileDialog);
    project_file_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
    project_file_dialog->set_mode(FileDialog::MODE_OPEN_FILE);
    project_file_dialog
        ->connect("file_selected", this, "_on_project_file_selected");
    _set_project_file_dialog_filters();
    add_child(project_file_dialog);
}

void ImportProjectDialog::show_dialog() {
    String default_project_path = EditorSettings::get_singleton()->get(
        "filesystem/directories/default_project_path"
    );
    if (default_project_path.empty()) {
        DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
        default_project_path  = dir_access->get_current_dir();
        memdelete(dir_access);
    }
    destination_folder_line_edit->set_text(default_project_path);
    destination_folder_dialog->set_current_dir(default_project_path);
    project_file_line_edit->call_deferred("grab_focus");
    _update_status();

    popup_centered_minsize(Size2(500, 0) * EDSCALE);
}

void ImportProjectDialog::_bind_methods() {
    ClassDB::bind_method(
        "_on_destination_folder_browse_buton_pressed",
        &ImportProjectDialog::_on_destination_folder_browse_buton_pressed
    );
    ClassDB::bind_method(
        "_on_destination_folder_selected",
        &ImportProjectDialog::_on_destination_folder_selected
    );
    ClassDB::bind_method(
        "_on_destination_folder_text_changed",
        &ImportProjectDialog::_on_destination_folder_text_changed
    );

    ClassDB::bind_method(
        "_on_project_file_browse_buton_pressed",
        &ImportProjectDialog::_on_project_file_browse_buton_pressed
    );
    ClassDB::bind_method(
        "_on_project_file_selected",
        &ImportProjectDialog::_on_project_file_selected
    );
    ClassDB::bind_method(
        "_on_project_file_text_changed",
        &ImportProjectDialog::_on_project_file_text_changed
    );
    ClassDB::bind_method(
        "_on_project_type_selected",
        &ImportProjectDialog::_on_project_type_selected
    );

    ADD_SIGNAL(MethodInfo("import_godot_project"));
}

void ImportProjectDialog::ok_pressed() {
    if (!_is_project_file_valid()) {
        invalid_project_file_error->show_modal();
        return;
    }
    if (!_is_destination_folder_valid()) {
        invalid_destination_folder_error->show_modal();
        return;
    }
    String project_file       = project_file_line_edit->get_text();
    String destination_folder = destination_folder_line_edit->get_text();
    switch (project_type) {
        case ProjectType::GODOT: {
            emit_signal(
                "import_godot_project",
                project_file,
                destination_folder
            );
        } break;
    }
}

bool ImportProjectDialog::_is_destination_folder_valid() {
    String destination_folder = destination_folder_line_edit->get_text();
    DirAccess* dir_access     = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (dir_access->change_dir(destination_folder) != OK) {
        memdelete(dir_access);
        return false;
    }
    // Check if the folder is empty.
    dir_access->list_dir_begin();
    String file_name = dir_access->get_next();
    while (!file_name.empty()) {
        if (file_name.begins_with(".")) {
            // Allow `.`, `..` (reserved current/parent folder names)
            // and hidden files/folders to be present.
            file_name = dir_access->get_next();
            continue;
        }
        memdelete(dir_access);
        return false;
    }
    memdelete(dir_access);
    return true;
}

bool ImportProjectDialog::_is_project_file_valid() {
    String project_file   = project_file_line_edit->get_text();
    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (!dir_access->file_exists(project_file)) {
        memdelete(dir_access);
        return false;
    }
    switch (project_type) {
        case ProjectType::GODOT: {
            if (!project_file.ends_with("project.godot")) {
                memdelete(dir_access);
                return false;
            }
        } break;
    }
    memdelete(dir_access);
    return true;
}

void ImportProjectDialog::_on_destination_folder_browse_buton_pressed() {
    destination_folder_dialog->set_current_dir(
        destination_folder_line_edit->get_text()
    );
    destination_folder_dialog->popup_centered_ratio();
}

void ImportProjectDialog::_on_destination_folder_selected(const String& path) {
    destination_folder_line_edit->set_text(path.simplify_path());
    _update_status();
}

void ImportProjectDialog::_on_destination_folder_text_changed(const String&) {
    _update_status();
}

void ImportProjectDialog::_on_project_file_browse_buton_pressed() {
    project_file_dialog->set_current_dir(project_file_line_edit->get_text());
    project_file_dialog->popup_centered_ratio();
}

void ImportProjectDialog::_on_project_file_selected(const String& path) {
    project_file_line_edit->set_text(path.simplify_path());
    _update_status();
}

void ImportProjectDialog::_on_project_file_text_changed(const String&) {
    _update_status();
}

void ImportProjectDialog::_on_project_type_selected(int index) {
    project_type = (ProjectType)project_type_option_button->get_item_id(index);
    _set_project_file_dialog_filters();
}

void ImportProjectDialog::_set_project_file_dialog_filters() {
    project_file_dialog->clear_filters();
    switch (project_type) {
        case ProjectType::GODOT: {
            project_file_dialog->add_filter(
                vformat("project.godot ; %s", TTR("Godot Project"))
            );
        } break;
    }
}

void ImportProjectDialog::_update_destination_folder_status(bool valid) {
    if (valid) {
        destination_folder_error_icon->set_texture(
            get_icon("StatusSuccess", "EditorIcons")
        );
        destination_folder_error_message_label->hide();
    } else {
        destination_folder_error_icon->set_texture(
            get_icon("StatusError", "EditorIcons")
        );
        destination_folder_error_message_label->show();
    }
    set_size(Size2(500, 0) * EDSCALE);
}

void ImportProjectDialog::_update_project_file_status(bool valid) {
    if (valid) {
        project_file_error_icon->set_texture(
            get_icon("StatusSuccess", "EditorIcons")
        );
        project_file_error_message_label->hide();
    } else {
        project_file_error_icon->set_texture(
            get_icon("StatusError", "EditorIcons")
        );
        project_file_error_message_label->show();
    }
    set_size(Size2(500, 0) * EDSCALE);
}

void ImportProjectDialog::_update_status() {
    bool project_file_valid = _is_project_file_valid();
    _update_project_file_status(project_file_valid);
    bool destination_folder_valid = _is_destination_folder_valid();
    _update_destination_folder_status(destination_folder_valid);
    Button* import_button = get_ok();
    if (project_file_valid && destination_folder_valid) {
        import_button->set_disabled(false);
        import_button->call_deferred("grab_focus");
    } else {
        import_button->set_disabled(true);
    }
}
