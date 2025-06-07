// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "add_project_dialog.h"

#include "core/version.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_themes.h"
#include "scene/gui/button.h"
#include "scene/gui/container.h"

AddProjectDialog::AddProjectDialog() {
    set_theme(create_custom_theme());
    set_title(TTR("Add Existing Project"));
    get_ok()->set_text(TTR("Add & Edit"));

    VBoxContainer* dialog_container = memnew(VBoxContainer);
    add_child(dialog_container);

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

    Button* browse_button = memnew(Button);
    browse_button->set_text(TTR("Browse"));
    browse_button->connect("pressed", this, "_on_browse_button_pressed");
    project_file_container->add_child(browse_button);

    error_message_label = memnew(Label);
    error_message_label->set_text(
        TTR("Please choose a \"project.rebel\" or \".zip\" file.")
    );
    error_message_label->set_align(Label::ALIGN_CENTER);
    error_message_label->add_color_override(
        "font_color",
        get_color("error_color", "Editor")
    );
    dialog_container->add_child(error_message_label);

    file_dialog = memnew(FileDialog);
    file_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
    file_dialog->connect("file_selected", this, "_on_file_selected");
    add_child(file_dialog);
}

void AddProjectDialog::show_dialog() {
    String default_dir = EditorSettings::get_singleton()->get(
        "filesystem/directories/default_project_path"
    );
    if (default_dir.empty()) {
        DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
        default_dir           = dir_access->get_current_dir();
        memdelete(dir_access);
    }
    project_file_line_edit->set_text(default_dir);
    file_dialog->set_current_dir(default_dir);
    _check_file(default_dir);

    popup_centered_minsize(Size2(500, 0) * EDSCALE);
}

void AddProjectDialog::_bind_methods() {
    ClassDB::bind_method(
        "_on_browse_button_pressed",
        &AddProjectDialog::_on_browse_button_pressed
    );
    ClassDB::bind_method(
        "_on_file_selected",
        &AddProjectDialog::_on_file_selected
    );
    ClassDB::bind_method(
        "_on_project_file_text_changed",
        &AddProjectDialog::_on_project_file_text_changed
    );

    ADD_SIGNAL(MethodInfo("extract_zip_file"));
    ADD_SIGNAL(MethodInfo("project_added"));
}

void AddProjectDialog::ok_pressed() {
    String project_file = project_file_line_edit->get_text();
    if (project_file.ends_with("zip")) {
        emit_signal("extract_zip_file", project_file);
    } else {
        emit_signal("project_added", project_file.get_base_dir());
    }
}

void AddProjectDialog::_check_file(const String& p_file) {
    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (dir_access->file_exists(p_file)
        && (p_file.ends_with("project.rebel") || p_file.ends_with(".zip"))) {
        project_file_error_icon->set_texture(
            get_icon("StatusSuccess", "EditorIcons")
        );
        error_message_label->hide();
        get_ok()->set_disabled(false);
        if (p_file.ends_with(".zip")) {
            get_ok()->set_text(TTR("Extract"));
        } else { // p_file.ends_with("project.rebel")
            get_ok()->set_text(TTR("Add & Edit"));
        }
        get_ok()->call_deferred("grab_focus");
    } else {
        project_file_error_icon->set_texture(
            get_icon("StatusError", "EditorIcons")
        );
        error_message_label->show();
        get_ok()->set_disabled(true);
        project_file_line_edit->call_deferred("grab_focus");
    }
    memdelete(dir_access);
    set_size(Size2(500, 0) * EDSCALE);
}

void AddProjectDialog::_on_browse_button_pressed() {
    file_dialog->set_current_dir(project_file_line_edit->get_text());
    file_dialog->set_mode(FileDialog::MODE_OPEN_FILE);
    file_dialog->clear_filters();
    file_dialog->add_filter(
        vformat("project.rebel ; %s %s", VERSION_NAME, TTR("Project"))
    );
    file_dialog->add_filter("*.zip ; " + TTR("ZIP File"));
    file_dialog->popup_centered_ratio();
}

void AddProjectDialog::_on_file_selected(const String& p_file) {
    _check_file(p_file);
    project_file_line_edit->set_text(p_file.simplify_path());
}

void AddProjectDialog::_on_project_file_text_changed(const String& p_text) {
    _check_file(p_text);
}
