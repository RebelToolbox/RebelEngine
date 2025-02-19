// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "rename_project_dialog.h"

#include "core/io/config_file.h"
#include "editor/editor_scale.h"
#include "editor/editor_themes.h"
#include "scene/gui/container.h"

RenameProjectDialog::RenameProjectDialog() {
    set_theme(create_custom_theme());
    set_title(TTR("Rename Project"));
    get_ok()->set_text(TTR("Rename"));

    VBoxContainer* dialog_container = memnew(VBoxContainer);
    add_child(dialog_container);

    Label* project_name_heading = memnew(Label);
    project_name_heading->set_text(TTR("Project Name:"));
    dialog_container->add_child(project_name_heading);

    project_name_line_edit = memnew(LineEdit);
    project_name_line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
    dialog_container->add_child(project_name_line_edit);

    Label* project_folder_heading = memnew(Label);
    project_folder_heading->set_text(TTR("Project Folder:"));
    dialog_container->add_child(project_folder_heading);

    HBoxContainer* project_path_container = memnew(HBoxContainer);
    dialog_container->add_child(project_path_container);

    project_folder_label = memnew(Label);
    project_folder_label->set_h_size_flags(SIZE_EXPAND_FILL);
    project_path_container->add_child(project_folder_label);

    project_path_error_icon = memnew(TextureRect);
    project_path_error_icon->set_texture(get_icon("StatusError", "EditorIcons")
    );
    project_path_error_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED
    );
    project_path_container->add_child(project_path_error_icon);

    error_message = memnew(Label);
    error_message->set_text(
        TTR("Couldn't load project.rebel in the project folder.\n"
            "The file or folder is missing or corrupt.")
    );
    error_message->set_align(Label::ALIGN_CENTER);
    error_message->add_color_override(
        "font_color",
        get_color("error_color", "Editor")
    );
    dialog_container->add_child(error_message);

    set_hide_on_ok(false);
}

void RenameProjectDialog::show_dialog(const String& p_folder) {
    project_folder = p_folder;
    project_folder_label->set_text(project_folder);

    Ref<ConfigFile> settings_file = memnew(ConfigFile);
    String settings_file_name     = project_folder.plus_file("project.rebel");
    Error error                   = settings_file->load(settings_file_name);
    if (error == OK) {
        String current_name =
            settings_file->get_value("application", "config/name");
        project_name_line_edit->set_text(current_name);
        project_path_error_icon->hide();
        error_message->hide();
        get_ok()->set_disabled(false);
    } else {
        project_path_error_icon->show();
        error_message->show();
        get_ok()->set_disabled(true);
    }

    project_name_line_edit->call_deferred("grab_focus");
    set_size(get_minimum_size());
    popup_centered_minsize(Size2(500, 0) * EDSCALE);
}

void RenameProjectDialog::_bind_methods() {
    ADD_SIGNAL(MethodInfo("projects_updated"));
}

void RenameProjectDialog::ok_pressed() {
    Ref<ConfigFile> settings_file = memnew(ConfigFile);
    String settings_file_name     = project_folder.plus_file("project.rebel");
    Error error                   = settings_file->load(settings_file_name);
    if (error != OK) {
        get_ok()->set_disabled(true);
        project_path_error_icon->show();
        error_message->show();
        set_size(get_minimum_size());
        return;
    }

    String new_name = project_name_line_edit->get_text().strip_edges();
    settings_file->set_value("application", "config/name", new_name);
    settings_file->save(settings_file_name);

    hide();
    emit_signal("projects_updated");
}
