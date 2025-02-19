// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "extract_zip_file_dialog.h"

#include "core/io/zip_io.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_themes.h"
#include "scene/gui/container.h"

namespace {
String get_zipped_project_folder(const unzFile& unz_file) {
    int error = unzGoToFirstFile(unz_file);
    while (error == UNZ_OK) {
        unz_file_info file_info;
        char file_name_buffer[16384];
        unzGetCurrentFileInfo(
            unz_file,
            &file_info,
            file_name_buffer,
            16384,
            nullptr,
            0,
            nullptr,
            0
        );
        String file_name = String::utf8(file_name_buffer);
        if (file_name.ends_with("project.rebel")) {
            return file_name.substr(0, file_name.rfind("project.rebel"));
        }
        error = unzGoToNextFile(unz_file);
    }
    return {};
}

void create_folder(
    const String& folder_path,
    const String& zipped_project_folder,
    const String& project_folder
) {
    String folder_name    = folder_path.substr(0, folder_path.length() - 1);
    String relative_path  = folder_name.substr(zipped_project_folder.length());
    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    dir_access->make_dir(project_folder.plus_file(relative_path));
    memdelete(dir_access);
}

void uncompress_file(
    const unzFile& unz_file,
    const String& file_name,
    const unsigned long uncompressed_size,
    const String& zipped_project_folder,
    const String& project_folder
) {
    Vector<uint8_t> data;
    data.resize(uncompressed_size);
    unzOpenCurrentFile(unz_file);
    unzReadCurrentFile(unz_file, data.ptrw(), uncompressed_size);
    unzCloseCurrentFile(unz_file);
    String relative_path    = file_name.substr(zipped_project_folder.length());
    FileAccess* file_access = FileAccess::open(
        project_folder.plus_file(relative_path),
        FileAccess::WRITE
    );
    if (file_access) {
        file_access->store_buffer(data.ptr(), uncompressed_size);
        memdelete(file_access);
    }
}

void unzip_project_files(
    const unzFile& unz_file,
    const String& zipped_project_folder,
    const String& project_folder
) {
    int error = unzGoToFirstFile(unz_file);
    while (error == UNZ_OK) {
        unz_file_info file_info;
        char file_name_buffer[16384];
        unzGetCurrentFileInfo(
            unz_file,
            &file_info,
            file_name_buffer,
            16384,
            nullptr,
            0,
            nullptr,
            0
        );
        String file_name = String::utf8(file_name_buffer);
        if (file_name.empty() || file_name == zipped_project_folder
            || !zipped_project_folder.is_subsequence_of(file_name)) {
            // Ignore.
        } else if (file_name.ends_with("/")) {
            create_folder(file_name, zipped_project_folder, project_folder);
        } else {
            uncompress_file(
                unz_file,
                file_name,
                file_info.uncompressed_size,
                zipped_project_folder,
                project_folder
            );
        }
        error = unzGoToNextFile(unz_file);
    }
}
} // namespace

ExtractZipFileDialog::ExtractZipFileDialog() {
    set_theme(create_custom_theme());
    get_ok()->set_text(TTR("Add & Edit"));

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

void ExtractZipFileDialog::show_dialog(
    const String& p_zip_file_path,
    const String& p_project_name
) {
    zip_file_path = p_zip_file_path;

    String zip_file_name  = zip_file_path.get_file();
    String project_name   = p_project_name;
    String project_folder = EditorSettings::get_singleton()->get(
        "filesystem/directories/default_project_path"
    );

    String title = TTR("Extract zip file: ");
    set_title(title + zip_file_name);

    if (project_name.empty()) {
        project_name =
            zip_file_name.substr(0, zip_file_name.length() - 4).capitalize();
    }
    project_name_line_edit->set_text(project_name);

    if (project_folder.empty()) {
        DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
        project_folder        = dir_access->get_current_dir();
        memdelete(dir_access);
    }
    project_folder_line_edit->set_text(project_folder);

    _test_project_folder();
    popup_centered_minsize(Size2(500, 0) * EDSCALE);
}

void ExtractZipFileDialog::_bind_methods() {
    ClassDB::bind_method(
        "_on_browse_button_pressed",
        &ExtractZipFileDialog::_on_browse_button_pressed
    );
    ClassDB::bind_method(
        "_on_create_folder_button_pressed",
        &ExtractZipFileDialog::_on_create_folder_button_pressed
    );
    ClassDB::bind_method(
        "_on_project_folder_text_changed",
        &ExtractZipFileDialog::_on_project_folder_text_changed
    );
    ClassDB::bind_method(
        "_on_project_folder_selected",
        &ExtractZipFileDialog::_on_project_folder_selected
    );

    ADD_SIGNAL(MethodInfo("project_added"));
}

void ExtractZipFileDialog::cancel_pressed() {
    _remove_created_folder();
}

void ExtractZipFileDialog::ok_pressed() {
    String zip_file_name  = zip_file_path.get_file();
    String project_folder = project_folder_line_edit->get_text();

    FileAccess* dummy    = nullptr;
    zlib_filefunc_def io = zipio_create_io_from_file(&dummy);
    unzFile unz_file     = unzOpen2(zip_file_path.utf8().get_data(), &io);
    if (!unz_file) {
        String message = TTR("Unable to open zip file: ");
        popup_message->set_text(message + zip_file_name);
        popup_message->popup_centered_minsize();
        get_ok()->set_disabled(true);
        return;
    }

    String zipped_project_folder = get_zipped_project_folder(unz_file);
    unzip_project_files(unz_file, zipped_project_folder, project_folder);
    unzClose(unz_file);
    hide();
    emit_signal("project_added", project_folder);
}

void ExtractZipFileDialog::_on_browse_button_pressed() {
    select_project_folder_dialog->set_current_dir(
        project_folder_line_edit->get_text()
    );
    select_project_folder_dialog->popup_centered_ratio();
}

void ExtractZipFileDialog::_on_create_folder_button_pressed() {
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

void ExtractZipFileDialog::_on_project_folder_selected(const String& p_folder) {
    String project_folder = p_folder.simplify_path();
    project_folder_line_edit->set_text(project_folder);
    _on_project_folder_text_changed(project_folder);
    get_ok()->call_deferred("grab_focus");
}

void ExtractZipFileDialog::_on_project_folder_text_changed(const String& p_text
) {
    if (created_folder != p_text) {
        _remove_created_folder();
    }
    _test_project_folder();
}

void ExtractZipFileDialog::_remove_created_folder() {
    if (created_folder.empty()) {
        return;
    }

    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    dir_access->remove(created_folder);
    memdelete(dir_access);
    create_folder_button->set_disabled(false);
    created_folder = String();
}

void ExtractZipFileDialog::_set_error_message(const String& p_error_message) {
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

void ExtractZipFileDialog::_test_project_folder() {
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
