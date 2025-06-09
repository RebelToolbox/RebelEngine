// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "new_project_dialog.h"

#include "core/os/file_access.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_themes.h"
#include "scene/gui/check_box.h"
#include "scene/gui/container.h"
#include "scene/gui/separator.h"

#ifndef SERVER_ENABLED
#include "drivers/gles3/rasterizer_gles3.h"
#endif // SERVER_ENABLED

namespace {
bool create_project(
    const String& project_folder,
    const ProjectSettings::CustomSettings& project_settings
) {
    Error error = ProjectSettings::get_singleton()->save_custom(
        project_folder.plus_file("project.rebel"),
        project_settings,
        Vector<String>(),
        false
    );
    if (error != OK) {
        return false;
    }

    error = ResourceSaver::save(
        project_folder.plus_file("icon.png"),
        create_unscaled_default_project_icon()
    );
    if (error != OK) {
        return false;
    }

    FileAccess* default_environment_file = FileAccess::open(
        project_folder.plus_file("default_env.tres"),
        FileAccess::WRITE
    );
    if (!default_environment_file) {
        return false;
    }
    default_environment_file->store_line(
        "[gd_resource type=\"Environment\" load_steps=2 format=2]"
    );
    default_environment_file->store_line(
        "[sub_resource type=\"ProceduralSky\" id=1]"
    );
    default_environment_file->store_line("[resource]");
    default_environment_file->store_line("background_mode = 2");
    default_environment_file->store_line("background_sky = SubResource( 1 )");
    memdelete(default_environment_file);

    return true;
}
} // namespace

NewProjectDialog::NewProjectDialog() {
    set_theme(create_custom_theme());
    set_title(TTR("Create New Project"));
    get_ok()->set_text(TTR("Create & Edit"));

    VBoxContainer* dialog_container = memnew(VBoxContainer);
    add_child(dialog_container);

    Label* project_name_heading = memnew(Label);
    project_name_heading->set_text(TTR("Project Name:"));
    dialog_container->add_child(project_name_heading);

    HBoxContainer* project_name_container = memnew(HBoxContainer);
    dialog_container->add_child(project_name_container);

    project_name_line_edit = memnew(LineEdit);
    project_name_line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
    project_name_container->add_child(project_name_line_edit);

    create_folder_button = memnew(Button);
    create_folder_button->set_text(TTR("Create Folder"));
    create_folder_button
        ->connect("pressed", this, "_on_create_folder_button_pressed");
    project_name_container->add_child(create_folder_button);

    Label* project_folder_heading = memnew(Label);
    project_folder_heading->set_text(TTR("Project Folder:"));
    dialog_container->add_child(project_folder_heading);

    HBoxContainer* project_folder_container = memnew(HBoxContainer);
    dialog_container->add_child(project_folder_container);

    project_folder_line_edit = memnew(LineEdit);
    project_folder_line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
    project_folder_line_edit
        ->connect("text_changed", this, "_on_project_folder_text_changed");
    project_folder_container->add_child(project_folder_line_edit);

    project_folder_error_icon = memnew(TextureRect);
    project_folder_error_icon->set_stretch_mode(
        TextureRect::STRETCH_KEEP_CENTERED
    );
    project_folder_container->add_child(project_folder_error_icon);

    project_folder_browse_button = memnew(Button);
    project_folder_browse_button->set_text(TTR("Browse"));
    project_folder_browse_button
        ->connect("pressed", this, "_on_browse_button_pressed");
    project_folder_container->add_child(project_folder_browse_button);

    error_message_label = memnew(Label);
    error_message_label->set_align(Label::ALIGN_CENTER);
    error_message_label->add_color_override(
        "font_color",
        get_color("error_color", "Editor")
    );
    dialog_container->add_child(error_message_label);

#ifndef SERVER_ENABLED
    bool gles3_supported = RasterizerGLES3::is_viable() == OK;
#else
    bool gles3_supported = false;
#endif // SERVER_ENABLED
    renderer_button_group.instance();

    Label* renderer_heading = memnew(Label);
    renderer_heading->set_text(TTR("Renderer:"));
    dialog_container->add_child(renderer_heading);

    HBoxContainer* renderer_selection_container = memnew(HBoxContainer);
    dialog_container->add_child(renderer_selection_container);

    VBoxContainer* gles3_renderer_container = memnew(VBoxContainer);
    gles3_renderer_container->set_h_size_flags(SIZE_EXPAND_FILL);
    renderer_selection_container->add_child(gles3_renderer_container);

    CheckBox* gles3_renderer_check_box = memnew(CheckBox);
    gles3_renderer_check_box->set_button_group(renderer_button_group);
    gles3_renderer_check_box->set_text(TTR("OpenGL ES 3.0"));
    gles3_renderer_check_box->set_meta("driver_name", "GLES3");
    gles3_renderer_container->add_child(gles3_renderer_check_box);
    if (gles3_supported) {
        gles3_renderer_check_box->set_pressed(true);
    } else {
        gles3_renderer_check_box->set_disabled(true);
        Label* gles3_not_supported_label = memnew(Label);
        gles3_not_supported_label->set_text(
            TTR("OpenGL ES 3.0 is not supported by your graphics card.")
        );
        gles3_renderer_container->add_child(gles3_not_supported_label);
    }

    Label* gles3_description = memnew(Label);
    gles3_description->set_text(
        TTR("Higher visual quality\n"
            "All features available\n"
            "Incompatible with older hardware\n"
            "Not recommended for web games")
    );
    gles3_renderer_container->add_child(gles3_description);

    renderer_selection_container->add_child(memnew(VSeparator));

    VBoxContainer* gles2_renderer_container = memnew(VBoxContainer);
    gles2_renderer_container->set_h_size_flags(SIZE_EXPAND_FILL);
    renderer_selection_container->add_child(gles2_renderer_container);

    CheckBox* gles2_renderer_check_box = memnew(CheckBox);
    gles2_renderer_check_box->set_button_group(renderer_button_group);
    gles2_renderer_check_box->set_text(TTR("OpenGL ES 2.0"));
    gles2_renderer_check_box->set_meta("driver_name", "GLES2");
    gles2_renderer_check_box->set_pressed(!gles3_supported);
    gles2_renderer_container->add_child(gles2_renderer_check_box);

    Label* gles2_description = memnew(Label);
    gles2_description->set_text(
        TTR("Lower visual quality\n"
            "Some features not available\n"
            "Works on most hardware\n"
            "Recommended for web games")
    );
    gles2_renderer_container->add_child(gles2_description);

    Label* renderer_changing_label = memnew(Label);
    renderer_changing_label->set_text(TTR(
        "Renderer can be changed later, but scenes may need to be adjusted."
    ));
    renderer_changing_label->set_align(Label::ALIGN_CENTER);
    dialog_container->add_child(renderer_changing_label);

    select_project_folder_dialog = memnew(FileDialog);
    select_project_folder_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
    select_project_folder_dialog->set_mode(FileDialog::MODE_OPEN_DIR);
    select_project_folder_dialog
        ->connect("dir_selected", this, "_on_project_folder_selected");
    add_child(select_project_folder_dialog);

    popup_message = memnew(AcceptDialog);
    add_child(popup_message);

    set_hide_on_ok(false);
}

void NewProjectDialog::show_dialog() {
    project_name_line_edit->set_text(TTR("My Game"));
    project_name_line_edit->call_deferred("grab_focus");
    project_name_line_edit->call_deferred("select_all");

    String project_folder = EditorSettings::get_singleton()->get(
        "filesystem/directories/default_project_path"
    );
    if (project_folder.empty()) {
        DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
        project_folder        = dir_access->get_current_dir();
        memdelete(dir_access);
    }
    project_folder_line_edit->set_text(project_folder);

    _test_project_folder();
    popup_centered_minsize(Size2(500, 0) * EDSCALE);
}

void NewProjectDialog::_bind_methods() {
    ClassDB::bind_method(
        "_on_browse_button_pressed",
        &NewProjectDialog::_on_browse_button_pressed
    );
    ClassDB::bind_method(
        "_on_create_folder_button_pressed",
        &NewProjectDialog::_on_create_folder_button_pressed
    );
    ClassDB::bind_method(
        "_on_project_folder_text_changed",
        &NewProjectDialog::_on_project_folder_text_changed
    );
    ClassDB::bind_method(
        "_on_project_folder_selected",
        &NewProjectDialog::_on_project_folder_selected
    );

    ADD_SIGNAL(MethodInfo("project_added"));
}

void NewProjectDialog::cancel_pressed() {
    _remove_created_folder();
}

void NewProjectDialog::ok_pressed() {
    String project_name   = project_name_line_edit->get_text().strip_edges();
    String project_folder = project_folder_line_edit->get_text();

    if (project_name.empty()) {
        popup_message->set_text(TTR("Please give your project a name."));
        popup_message->popup_centered_minsize();
        return;
    }

    ProjectSettings::CustomSettings project_settings;
    project_settings["application/config/name"] = project_name;
    if (renderer_button_group->get_pressed_button()->get_meta("driver_name")
        == "GLES3") {
        project_settings["rendering/quality/driver/driver_name"] = "GLES3";
    } else {
        project_settings["rendering/quality/driver/driver_name"]   = "GLES2";
        project_settings["rendering/vram_compression/import_etc2"] = false;
        project_settings["rendering/vram_compression/import_etc"]  = true;
    }
    project_settings["application/config/icon"] = "res://icon.png";
    project_settings["rendering/environment/default_environment"] =
        "res://default_env.tres";
    project_settings["physics/common/enable_pause_aware_picking"] = true;

    if (!create_project(project_folder, project_settings)) {
        _set_error_message(
            TTR("Couldn't create a project in this project folder.")
        );
        return;
    }

    hide();
    emit_signal("project_added", project_folder);
}

void NewProjectDialog::_on_browse_button_pressed() {
    select_project_folder_dialog->set_current_dir(
        project_folder_line_edit->get_text()
    );
    select_project_folder_dialog->popup_centered_ratio();
}

void NewProjectDialog::_on_create_folder_button_pressed() {
    String project_name   = project_name_line_edit->get_text().strip_edges();
    String project_folder = project_folder_line_edit->get_text().strip_edges();

    if (project_name.empty() || project_name.ends_with(".")) {
        popup_message->set_text(
            TTR("Cannot create a folder with this project name.")
        );
        popup_message->popup_centered_minsize();
        return;
    }

    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (dir_access->change_dir(project_folder) != OK) {
        popup_message->set_text(TTR("Cannot open the current project folder."));
        popup_message->popup_centered_minsize();
        memdelete(dir_access);
        return;
    }
    if (dir_access->dir_exists(project_name)) {
        popup_message->set_text(
            TTR("There is already a folder with this project name.")
        );
        popup_message->popup_centered_minsize();
        memdelete(dir_access);
        return;
    }
    if (dir_access->make_dir(project_name) != OK) {
        popup_message->set_text(
            TTR("Couldn't create a folder with this project name.")
        );
        popup_message->popup_centered_minsize();
        memdelete(dir_access);
        return;
    }

    dir_access->change_dir(project_name);
    created_folder = dir_access->get_current_dir();
    memdelete(dir_access);

    project_folder_line_edit->set_text(created_folder);
    create_folder_button->set_disabled(true);
    _test_project_folder();
}

void NewProjectDialog::_on_project_folder_selected(const String& p_folder) {
    String project_folder = p_folder.simplify_path();
    project_folder_line_edit->set_text(project_folder);
    _on_project_folder_text_changed(project_folder);
    get_ok()->call_deferred("grab_focus");
}

void NewProjectDialog::_on_project_folder_text_changed(const String& p_text) {
    if (created_folder != p_text) {
        _remove_created_folder();
    }
    _test_project_folder();
}

void NewProjectDialog::_remove_created_folder() {
    if (created_folder.empty()) {
        return;
    }

    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    dir_access->remove(created_folder);
    memdelete(dir_access);
    create_folder_button->set_disabled(false);
    created_folder = String();
}

void NewProjectDialog::_set_error_message(const String& p_error_message) {
    error_message_label->set_text(p_error_message);
    if (p_error_message.empty()) {
        project_folder_error_icon->set_texture(
            get_icon("StatusSuccess", "EditorIcons")
        );
        error_message_label->hide();
        get_ok()->set_disabled(false);
    } else {
        project_folder_error_icon->set_texture(
            get_icon("StatusError", "EditorIcons")
        );
        error_message_label->show();
        get_ok()->set_disabled(true);
    }
    set_size(Size2(500, 0) * EDSCALE);
}

void NewProjectDialog::_test_project_folder() {
    String project_folder = project_folder_line_edit->get_text().strip_edges();

    // Check if the folder exists.
    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (dir_access->change_dir(project_folder) != OK) {
        _set_error_message(TTR("The project folder does not exist."));
        memdelete(dir_access);
        return;
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
        _set_error_message(TTR("Please choose an empty folder."));
        memdelete(dir_access);
        return;
    }

    _set_error_message(String());
    memdelete(dir_access);
}
