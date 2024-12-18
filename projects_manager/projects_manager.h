// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PROJECTS_MANAGER_H
#define PROJECTS_MANAGER_H

#include "core/list.h"
#include "core/os/input_event.h"
#include "core/ustring.h"
#include "core/variant.h"
#include "editor/editor_about.h"
#include "editor/plugins/asset_library_editor_plugin.h"
#include "projects_manager/add_project_dialog.h"
#include "projects_manager/extract_zip_file_dialog.h"
#include "projects_manager/new_project_dialog.h"
#include "projects_manager/projects_list.h"
#include "projects_manager/rename_project_dialog.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/link_button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel.h"
#include "scene/gui/tab_container.h"

class ProjectsManager : public Panel {
    GDCLASS(ProjectsManager, Panel);

public:
    ProjectsManager();
    ~ProjectsManager();

protected:
    static void _bind_methods();
    void _notification(int p_what);

private:
    AcceptDialog* newer_settings_file_version_error;
    AcceptDialog* no_assets_folder_error;
    AcceptDialog* no_main_scene_defined_error;
    AcceptDialog* no_settings_file_error;

    Button* edit_button;
    Button* remove_button;
    Button* remove_missing_button;
    Button* rename_button;
    Button* run_button;

    CheckBox* delete_project_folder_check_box;

    ConfirmationDialog* add_multiple_files_confirmation;
    ConfirmationDialog* edit_multiple_confirmation;
    ConfirmationDialog* open_asset_library_confirmation;
    ConfirmationDialog* remove_confirmation;
    ConfirmationDialog* remove_missing_confirmation;
    ConfirmationDialog* restart_confirmation;
    ConfirmationDialog* run_multiple_confirmation;
    ConfirmationDialog* upgrade_settings_confirmation;

    AddProjectDialog* add_project_dialog;
    ExtractZipFileDialog* extract_zip_file_dialog;
    NewProjectDialog* new_project_dialog;
    RenameProjectDialog* rename_project_dialog;

    EditorAbout* editor_about;
    EditorAssetLibrary* asset_library;

    FileDialog* select_search_folder;

    Label* remove_confirmation_label;

    LinkButton* version_label;

    OptionButton* language_options;

    ProjectsList* projects_list;

    TabContainer* tabs;

    bool editing_project = false;

    Control* _create_buttons();
    void _create_dialogs();
    Control* _create_add_multiple_files_confirmation();
    Control* _create_add_project_dialog();
    Control* _create_edit_multiple_confirmation();
    Control* _create_extract_zip_file_dialog();
    Control* _create_language_options();
    Control* _create_new_project_dialog();
    Control* _create_newer_settings_file_version_error();
    Control* _create_no_assets_folder_error();
    Control* _create_no_main_scene_defined_error();
    Control* _create_no_settings_file_error();
    Control* _create_open_asset_library_confirmation();
    Control* _create_projects_tab();
    Control* _create_remove_confirmation();
    Control* _create_remove_missing_confirmation();
    Control* _create_rename_project_dialog();
    Control* _create_restart_confirmation();
    Control* _create_run_multiple_confirmation();
    Control* _create_select_search_folder();
    Control* _create_tabs();
    Control* _create_tools();
    Control* _create_upgrade_settings_confirmation();
    void _add_file(const String& p_file);
    void _add_multiple_files(const PoolStringArray& p_files);
    void _add_project(const String& p_folder);
    void _dim_window();
    void _edit_project(const String& project_key);
    void _edit_selected_projects();
    void _edit_selected_projects_requested();
    void _extract_zip_file(
        const String& p_zip_file_path,
        const String& p_project_name = String()
    );
    void _on_about_button_pressed();
    void _on_add_button_pressed();
    void _on_add_multiple_files_confirmed(const PoolStringArray& p_files);
    void _on_edit_button_pressed();
    void _on_edit_multiple_confirmed();
    void _on_extract_zip_file(const String& p_zip_file_path);
    void _on_files_dropped(const PoolStringArray& p_files, int);
    void _on_global_menu_action(const Variant& p_id, const Variant& p_meta);
    void _on_install_asset(const String& p_zip_path, const String& p_title);
    void _on_item_double_clicked();
    void _on_language_selected(int p_id);
    void _on_new_project_button_pressed();
    void _on_open_asset_library_confirmed();
    void _on_project_added(const String& p_project_folder);
    void _on_projects_updated();
    void _on_rename_button_pressed();
    void _on_remove_button_pressed();
    void _on_remove_confirmed();
    void _on_remove_missing_button_pressed();
    void _on_remove_missing_confirmed();
    void _on_restart_confirmed();
    void _on_run_button_pressed();
    void _on_run_multiple_confirmed();
    void _on_search_button_pressed();
    void _on_search_folder_selected(const String& p_folder);
    void _on_selection_changed();
    void _on_tab_changed(int p_tab);
    void _on_upgrade_settings_confirmed(const String& p_project_key);
    void _on_version_label_pressed();
    void _open_asset_library();
    void _popup_add_multiple_files_confirmation(const PoolStringArray& p_files);
    void _popup_newer_settings_file_version_error(const String& p_file);
    void _popup_no_settings_file_error(const String& p_file);
    void _popup_upgrade_settings_confirmation(
        const String& p_project_key,
        const String& p_settings_file
    );
    void _quit();
    void _run_selected();
    void _search_folder(const String& p_base);
    void _show_editor_about();
    void _unhandled_input(const Ref<InputEvent>& p_event);
    void _update_project_buttons();
};

#endif // PROJECTS_MANAGER_H
