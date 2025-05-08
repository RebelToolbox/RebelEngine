// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "projects_manager.h"

#include "core/io/config_file.h"
#include "core/io/resource_saver.h"
#include "core/io/stream_peer_ssl.h"
#include "core/io/zip_io.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/keyboard.h"
#include "core/os/os.h"
#include "core/translation.h"
#include "core/version.h"
#include "core/version_hash.gen.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_themes.h"
#include "projects_manager/projects_list_item.h"
#include "scene/gui/center_container.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tool_button.h"

// Used to test for GLES3 support.
#ifndef SERVER_ENABLED
#include "drivers/gles3/rasterizer_gles3.h"
#endif

namespace {
void apply_editor_settings() {
    if (!EditorSettings::get_singleton()) {
        EditorSettings::create();
    }
    EditorSettings* editor_settings = EditorSettings::get_singleton();

    editor_settings->set_optimize_save(false);
    int display_scale = editor_settings->get("interface/editor/display_scale");
    float custom_display_scale =
        editor_settings->get("interface/editor/custom_display_scale");
    switch (display_scale) {
        case 0:
            // Try applying a suitable display scale automatically.
            editor_set_scale(editor_settings->get_auto_display_scale());
            break;
        case 1:
            editor_set_scale(0.75);
            break;
        case 2:
            editor_set_scale(1.0);
            break;
        case 3:
            editor_set_scale(1.25);
            break;
        case 4:
            editor_set_scale(1.5);
            break;
        case 5:
            editor_set_scale(1.75);
            break;
        case 6:
            editor_set_scale(2.0);
            break;
        default:
            editor_set_scale(custom_display_scale);
            break;
    }

    FileDialog::set_default_show_hidden_files(
        editor_settings->get("filesystem/file_dialog/show_hidden_files")
    );
}

void apply_window_settings() {
    OS* os = OS::get_singleton();
    os->set_min_window_size(Size2(750, 420) * EDSCALE);
    // TODO: Automatically resize hiDPI display windows on Windows and Linux.
    os->set_window_size(os->get_window_size() * MAX(1, EDSCALE));
    // TRANSLATORS: Projects Manager is the application used to manage projects.
    os->set_window_title(
        VERSION_NAME + String(" - ") + TTR("Projects Manager")
    );
    os->set_low_processor_usage_mode(true);
}

void create_project_directories() {
    DirAccessRef dir_access =
        DirAccess::create(DirAccess::AccessType::ACCESS_FILESYSTEM);

    String default_project_path = EditorSettings::get_singleton()->get(
        "filesystem/directories/default_project_path"
    );
    Error error = dir_access->make_dir_recursive(default_project_path);
    if (error != OK) {
        ERR_PRINT(
            "Could not create default project directory: "
            + default_project_path
        );
    }

    String auto_search_project_path = EditorSettings::get_singleton()->get(
        "filesystem/directories/auto_search_project_path"
    );
    error = dir_access->make_dir_recursive(auto_search_project_path);
    if (error != OK) {
        ERR_PRINT(
            "Could not create auto search project path: "
            + auto_search_project_path
        );
    }
}

void edit_project(const String& project_name, const String& project_folder) {
    print_line(vformat("Editing project: %s (%s)", project_name, project_folder)
    );
    OS* os = OS::get_singleton();
    List<String> args;
    args.push_back("--path");
    args.push_back(project_folder);
    args.push_back("--editor");
    if (os->is_stdout_debug_enabled()) {
        args.push_back("--debug");
    }
    if (os->is_stdout_verbose()) {
        args.push_back("--verbose");
    }
    if (os->is_disable_crash_handler()) {
        args.push_back("--disable-crash-handler");
    }
    String exec       = os->get_executable_path();
    OS::ProcessID pid = 0;
    Error error       = os->execute(exec, args, false, &pid);
    ERR_FAIL_COND(error);
}

void run_project(const String& project_name, const String& project_folder) {
    print_line(vformat("Running project: %s (%s)", project_name, project_folder)
    );
    OS* os = OS::get_singleton();
    List<String> args;
    args.push_back("--path");
    args.push_back(project_folder);
    if (os->is_disable_crash_handler()) {
        args.push_back("--disable-crash-handler");
    }
    String exec       = os->get_executable_path();
    OS::ProcessID pid = 0;
    Error error       = os->execute(exec, args, false, &pid);
    ERR_FAIL_COND(error);
}
} // namespace

ProjectsManager::ProjectsManager() {
    apply_editor_settings();
    apply_window_settings();
    create_project_directories();

    set_anchors_and_margins_preset(Control::PRESET_WIDE);
    set_theme(create_custom_theme());
    add_style_override("panel", get_stylebox("Background", "EditorStyles"));

    VBoxContainer* panel_container = memnew(VBoxContainer);
    panel_container->set_anchors_and_margins_preset(
        Control::PRESET_WIDE,
        Control::PRESET_MODE_MINSIZE,
        8 * EDSCALE
    );
    add_child(panel_container);

    Control* center_box = memnew(Control);
    center_box->set_v_size_flags(SIZE_EXPAND_FILL);
    center_box->add_child(_create_tabs());
    center_box->add_child(_create_tools());
    panel_container->add_child(center_box);

    _create_dialogs();

    SceneTree::get_singleton()
        ->connect("files_dropped", this, "_on_files_dropped");
    SceneTree::get_singleton()
        ->connect("global_menu_action", this, "_on_global_menu_action");

    String auto_search_project_path = EditorSettings::get_singleton()->get(
        "filesystem/directories/auto_search_project_path"
    );
    if (!auto_search_project_path.empty()
        && DirAccess::exists(auto_search_project_path)) {
        _search_folder(auto_search_project_path);
    }

    _update_project_buttons();
}

ProjectsManager::~ProjectsManager() {
    if (EditorSettings::get_singleton()) {
        EditorSettings::destroy();
    }
}

void ProjectsManager::_bind_methods() {
    ClassDB::bind_method(
        "_on_about_button_pressed",
        &ProjectsManager::_on_about_button_pressed
    );
    ClassDB::bind_method(
        "_on_add_button_pressed",
        &ProjectsManager::_on_add_button_pressed
    );
    ClassDB::bind_method(
        D_METHOD("_on_add_multiple_files_confirmed", "folders"),
        &ProjectsManager::_on_add_multiple_files_confirmed
    );
    ClassDB::bind_method(
        "_on_extract_zip_file",
        &ProjectsManager::_on_extract_zip_file
    );
    ClassDB::bind_method(
        "_on_edit_button_pressed",
        &ProjectsManager::_on_edit_button_pressed
    );
    ClassDB::bind_method(
        "_on_edit_multiple_confirmed",
        &ProjectsManager::_on_edit_multiple_confirmed
    );
    ClassDB::bind_method(
        "_on_files_dropped",
        &ProjectsManager::_on_files_dropped
    );
    ClassDB::bind_method(
        D_METHOD("_on_global_menu_action"),
        &ProjectsManager::_on_global_menu_action,
        DEFVAL(Variant())
    );
    ClassDB::bind_method(
        "_on_import_button_pressed",
        &ProjectsManager::_on_import_button_pressed
    );
    ClassDB::bind_method(
        "_on_import_godot_project",
        &ProjectsManager::_on_import_godot_project
    );
    ClassDB::bind_method(
        "_on_install_asset",
        &ProjectsManager::_on_install_asset
    );
    ClassDB::bind_method(
        "_on_item_double_clicked",
        &ProjectsManager::_on_item_double_clicked
    );
    ClassDB::bind_method(
        "_on_language_selected",
        &ProjectsManager::_on_language_selected
    );
    ClassDB::bind_method(
        "_on_open_asset_library_confirmed",
        &ProjectsManager::_on_open_asset_library_confirmed
    );
    ClassDB::bind_method(
        "_on_new_project_button_pressed",
        &ProjectsManager::_on_new_project_button_pressed
    );
    ClassDB::bind_method(
        "_on_project_added",
        &ProjectsManager::_on_project_added
    );
    ClassDB::bind_method(
        "_on_projects_updated",
        &ProjectsManager::_on_projects_updated
    );
    ClassDB::bind_method(
        "_on_rename_button_pressed",
        &ProjectsManager::_on_rename_button_pressed
    );
    ClassDB::bind_method(
        "_on_remove_button_pressed",
        &ProjectsManager::_on_remove_button_pressed
    );
    ClassDB::bind_method(
        "_on_remove_confirmed",
        &ProjectsManager::_on_remove_confirmed
    );
    ClassDB::bind_method(
        "_on_remove_missing_button_pressed",
        &ProjectsManager::_on_remove_missing_button_pressed
    );
    ClassDB::bind_method(
        "_on_remove_missing_confirmed",
        &ProjectsManager::_on_remove_missing_confirmed
    );
    ClassDB::bind_method(
        "_on_restart_confirmed",
        &ProjectsManager::_on_restart_confirmed
    );
    ClassDB::bind_method(
        "_on_run_button_pressed",
        &ProjectsManager::_on_run_button_pressed
    );
    ClassDB::bind_method(
        "_on_run_multiple_confirmed",
        &ProjectsManager::_on_run_multiple_confirmed
    );
    ClassDB::bind_method(
        "_on_search_button_pressed",
        &ProjectsManager::_on_search_button_pressed
    );
    ClassDB::bind_method(
        "_on_search_folder_selected",
        &ProjectsManager::_on_search_folder_selected
    );
    ClassDB::bind_method(
        "_on_selection_changed",
        &ProjectsManager::_on_selection_changed
    );
    ClassDB::bind_method("_on_tab_changed", &ProjectsManager::_on_tab_changed);
    ClassDB::bind_method(
        "_on_upgrade_settings_confirmed",
        &ProjectsManager::_on_upgrade_settings_confirmed
    );
    ClassDB::bind_method(
        "_on_version_label_pressed",
        &ProjectsManager::_on_version_label_pressed
    );

    ClassDB::bind_method(
        "_unhandled_input",
        &ProjectsManager::_unhandled_input
    );
}

void ProjectsManager::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            Engine::get_singleton()->set_editor_hint(false);
        } break;
        case NOTIFICATION_RESIZED: {
            if (open_asset_library_confirmation->is_visible()) {
                open_asset_library_confirmation->popup_centered_minsize();
            }
        } break;
        case NOTIFICATION_READY: {
            if (projects_list->get_project_count() == 0
                && StreamPeerSSL::is_available()) {
                open_asset_library_confirmation->popup_centered_minsize();
            }
        } break;
        case NOTIFICATION_PROCESS: {
            if (editing_project) {
                _quit();
            }
        } break;
        case NOTIFICATION_VISIBILITY_CHANGED: {
            set_process_unhandled_input(is_visible_in_tree());
        } break;
        case NOTIFICATION_WM_QUIT_REQUEST: {
            _dim_window();
        } break;
        case NOTIFICATION_WM_ABOUT: {
            _show_editor_about();
        } break;
    }
}

Control* ProjectsManager::_create_buttons() {
    VBoxContainer* buttons_container = memnew(VBoxContainer);
    buttons_container->set_custom_minimum_size(Size2(120, 120));

    Button* new_project_button = memnew(Button);
    new_project_button->set_text(TTR("New"));
    new_project_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/new_project",
        TTR("Create new project"),
        KEY_MASK_CMD | KEY_N
    ));
    new_project_button
        ->connect("pressed", this, "_on_new_project_button_pressed");
    buttons_container->add_child(new_project_button);

    Button* add_button = memnew(Button);
    add_button->set_text(TTR("Add"));
    add_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/add_project",
        TTR("Add existing project"),
        KEY_MASK_CMD | KEY_A
    ));
    add_button->connect("pressed", this, "_on_add_button_pressed");
    buttons_container->add_child(add_button);

    Button* import_button = memnew(Button);
    import_button->set_text(TTR("Import"));
    import_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/import_project",
        TTR("Import non-Rebel project"),
        KEY_MASK_CMD | KEY_I
    ));
    import_button->connect("pressed", this, "_on_import_button_pressed");
    buttons_container->add_child(import_button);

    Button* search_button = memnew(Button);
    search_button->set_text(TTR("Search"));
    search_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/projects_search",
        TTR("Search for existing projects"),
        KEY_MASK_CMD | KEY_S
    ));
    search_button->connect("pressed", this, "_on_search_button_pressed");
    buttons_container->add_child(search_button);

    remove_missing_button = memnew(Button);
    remove_missing_button->set_text(TTR("Remove Missing"));
    remove_missing_button
        ->connect("pressed", this, "_on_remove_missing_button_pressed");
    buttons_container->add_child(remove_missing_button);

    buttons_container->add_child(memnew(HSeparator));

    edit_button = memnew(Button);
    edit_button->set_text(TTR("Edit"));
    edit_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/edit_project",
        TTR("Edit Project"),
        KEY_MASK_CMD | KEY_E
    ));
    edit_button->connect("pressed", this, "_on_edit_button_pressed");
    buttons_container->add_child(edit_button);

    run_button = memnew(Button);
    run_button->set_text(TTR("Run"));
    run_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/run_project",
        TTR("Run Project"),
        KEY_MASK_CMD | KEY_R
    ));
    run_button->connect("pressed", this, "_on_run_button_pressed");
    buttons_container->add_child(run_button);

    rename_button = memnew(Button);
    rename_button->set_text(TTR("Rename"));
    rename_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/rename_project",
        TTR("Rename Project"),
        KEY_F2
    ));
    rename_button->connect("pressed", this, "_on_rename_button_pressed");
    buttons_container->add_child(rename_button);

    remove_button = memnew(Button);
    remove_button->set_text(TTR("Remove"));
    remove_button->set_shortcut(ED_SHORTCUT(
        "projects_manager/remove_project",
        TTR("Remove Project"),
        KEY_DELETE
    ));
    remove_button->connect("pressed", this, "_on_remove_button_pressed");
    buttons_container->add_child(remove_button);

    buttons_container->add_child(memnew(HSeparator));

    Button* about_button = memnew(Button);
    about_button->set_text(TTR("About"));
    about_button->connect("pressed", this, "_on_about_button_pressed");
    buttons_container->add_child(about_button);

    return buttons_container;
}

void ProjectsManager::_create_dialogs() {
    add_child(_create_add_multiple_files_confirmation());
    add_child(_create_edit_multiple_confirmation());
    add_child(_create_open_asset_library_confirmation());
    add_child(_create_remove_confirmation());
    add_child(_create_remove_missing_confirmation());
    add_child(_create_restart_confirmation());
    add_child(_create_run_multiple_confirmation());
    add_child(_create_upgrade_settings_confirmation());

    add_child(_create_newer_settings_file_version_error());
    add_child(_create_no_assets_folder_error());
    add_child(_create_no_main_scene_defined_error());
    add_child(_create_no_settings_file_error());

    add_child(_create_add_project_dialog());
    add_child(_create_import_project_dialog());
    add_child(_create_extract_zip_file_dialog());
    add_child(_create_new_project_dialog());
    add_child(_create_rename_project_dialog());

    add_child(_create_select_search_folder());

    editor_about = memnew(EditorAbout);
    add_child(editor_about);
}

Control* ProjectsManager::_create_add_multiple_files_confirmation() {
    add_multiple_files_confirmation = memnew(ConfirmationDialog);
    add_multiple_files_confirmation->set_text(TTR(
        "Are you sure you want to add or search multiple files and folders?"
    ));
    Button* ok_button = add_multiple_files_confirmation->get_ok();
    ok_button->set_text(TTR("Add"));
    ok_button->connect(
        "pressed",
        this,
        "_on_add_multiple_files_confirmed",
        varray()
    );
    return add_multiple_files_confirmation;
}

Control* ProjectsManager::_create_add_project_dialog() {
    add_project_dialog = memnew(AddProjectDialog);
    add_project_dialog
        ->connect("extract_zip_file", this, "_on_extract_zip_file");
    add_project_dialog->connect("project_added", this, "_on_project_added");
    return add_project_dialog;
}

Control* ProjectsManager::_create_edit_multiple_confirmation() {
    edit_multiple_confirmation = memnew(ConfirmationDialog);
    edit_multiple_confirmation->set_text(
        TTR("Are you sure you want to edit more than one project?")
    );
    edit_multiple_confirmation->get_ok()->set_text(TTR("Edit"));
    edit_multiple_confirmation->get_ok()
        ->connect("pressed", this, "_on_edit_multiple_confirmed");
    return edit_multiple_confirmation;
}

Control* ProjectsManager::_create_extract_zip_file_dialog() {
    extract_zip_file_dialog = memnew(ExtractZipFileDialog);
    extract_zip_file_dialog
        ->connect("project_added", this, "_on_project_added");
    return extract_zip_file_dialog;
}

Control* ProjectsManager::_create_import_project_dialog() {
    import_project_dialog = memnew(ImportProjectDialog);
    import_project_dialog
        ->connect("import_godot_project", this, "_on_import_godot_project");
    return import_project_dialog;
}

Control* ProjectsManager::_create_language_options() {
    language_options = memnew(OptionButton);
    language_options->set_flat(true);
    language_options->set_focus_mode(Control::FOCUS_NONE);
    language_options->set_icon(get_icon("Environment", "EditorIcons"));
    language_options->connect("item_selected", this, "_on_language_selected");

    Vector<String> language_codes;
    List<PropertyInfo> properties_list;
    EditorSettings::get_singleton()->get_property_list(&properties_list);
    for (List<PropertyInfo>::Element* E = properties_list.front(); E;
         E                              = E->next()) {
        PropertyInfo& property_info = E->get();
        if (property_info.name == "interface/editor/editor_language") {
            language_codes = property_info.hint_string.split(",");
        }
    }

    String current_language_code =
        EditorSettings::get_singleton()->get("interface/editor/editor_language"
        );
    for (int i = 0; i < language_codes.size(); i++) {
        const String& language_code = language_codes[i];
        String language_name =
            TranslationServer::get_singleton()->get_locale_name(language_code);
        language_options->add_item(
            language_name + " [" + language_code + "]",
            i
        );
        language_options->set_item_metadata(i, language_code);
        if (current_language_code == language_code) {
            language_options->select(i);
            language_options->set_text(language_code);
        }
    }

    return language_options;
}

Control* ProjectsManager::_create_new_project_dialog() {
    new_project_dialog = memnew(NewProjectDialog);
    new_project_dialog->connect("project_added", this, "_on_project_added");
    return new_project_dialog;
}

Control* ProjectsManager::_create_newer_settings_file_version_error() {
    newer_settings_file_version_error = memnew(AcceptDialog);
    return newer_settings_file_version_error;
}

Control* ProjectsManager::_create_no_assets_folder_error() {
    no_assets_folder_error = memnew(AcceptDialog);
    no_assets_folder_error->set_title(TTR("Can't run project"));
    no_assets_folder_error->set_text(
        TTR("Can't run project: Assets need to be imported.\n"
            "Please edit the project to trigger the initial import.")
    );
    return no_assets_folder_error;
}

Control* ProjectsManager::_create_no_main_scene_defined_error() {
    no_main_scene_defined_error = memnew(AcceptDialog);
    no_main_scene_defined_error->set_title(TTR("Can't run project"));
    no_main_scene_defined_error->set_text(
        TTR("Can't run project: no main scene defined.\n"
            "Please edit the project and set the main scene in the Project "
            "Settings under the \"Application\" category.")
    );
    return no_main_scene_defined_error;
}

Control* ProjectsManager::_create_no_settings_file_error() {
    no_settings_file_error = memnew(AcceptDialog);
    return no_settings_file_error;
}

Control* ProjectsManager::_create_open_asset_library_confirmation() {
    open_asset_library_confirmation = memnew(ConfirmationDialog);
    open_asset_library_confirmation->set_text(
        TTR("You currently don't have any projects.\n"
            "Would you like to explore official example projects in the Asset "
            "Library?")
    );
    open_asset_library_confirmation->get_ok()->set_text(TTR("Open Asset Library"
    ));
    open_asset_library_confirmation
        ->connect("confirmed", this, "_on_open_asset_library_confirmed");
    return open_asset_library_confirmation;
}

Control* ProjectsManager::_create_projects_tab() {
    HBoxContainer* projects_tab_container = memnew(HBoxContainer);
    projects_tab_container->set_name(TTR("Local Projects"));

    projects_list = memnew(ProjectsList);
    projects_list->connect("selection_changed", this, "_on_selection_changed");
    projects_list
        ->connect("item_double_clicked", this, "_on_item_double_clicked");
    projects_tab_container->add_child(projects_list);

    projects_tab_container->add_child(_create_buttons());

    return projects_tab_container;
}

Control* ProjectsManager::_create_remove_confirmation() {
    remove_confirmation = memnew(ConfirmationDialog);
    remove_confirmation->get_ok()->set_text(TTR("Remove"));
    remove_confirmation->get_ok()
        ->connect("pressed", this, "_on_remove_confirmed");

    VBoxContainer* remove_confirmation_vb = memnew(VBoxContainer);
    remove_confirmation->add_child(remove_confirmation_vb);

    remove_confirmation_label = memnew(Label);
    remove_confirmation_vb->add_child(remove_confirmation_label);

    delete_project_folder_check_box = memnew(CheckBox);
    delete_project_folder_check_box->set_text(TTR("Also delete project folder?")
    );
    remove_confirmation_vb->add_child(delete_project_folder_check_box);

    return remove_confirmation;
}

Control* ProjectsManager::_create_remove_missing_confirmation() {
    remove_missing_confirmation = memnew(ConfirmationDialog);
    remove_missing_confirmation->set_text(
        TTR("Remove all missing projects from the list?\n"
            "The project folders' contents won't be modified.")
    );
    remove_missing_confirmation->get_ok()->set_text(TTR("Remove All"));
    remove_missing_confirmation->get_ok()
        ->connect("pressed", this, "_on_remove_missing_confirmed");
    return remove_missing_confirmation;
}

Control* ProjectsManager::_create_rename_project_dialog() {
    rename_project_dialog = memnew(RenameProjectDialog);
    rename_project_dialog
        ->connect("projects_updated", this, "_on_projects_updated");
    return rename_project_dialog;
}

Control* ProjectsManager::_create_restart_confirmation() {
    restart_confirmation = memnew(ConfirmationDialog);
    restart_confirmation->set_text(
        TTR("Language changed.\n"
            "The interface will be updated after restarting Projects Manager.")
    );
    restart_confirmation->get_ok()->set_text(TTR("Restart Now"));
    restart_confirmation->get_ok()
        ->connect("pressed", this, "_on_restart_confirmed");
    restart_confirmation->get_cancel()->set_text(TTR("Continue"));
    return restart_confirmation;
}

Control* ProjectsManager::_create_run_multiple_confirmation() {
    run_multiple_confirmation = memnew(ConfirmationDialog);
    run_multiple_confirmation->get_ok()->set_text(TTR("Run"));
    run_multiple_confirmation->get_ok()
        ->connect("pressed", this, "_on_run_multiple_confirmed");
    return run_multiple_confirmation;
}

Control* ProjectsManager::_create_select_search_folder() {
    select_search_folder = memnew(FileDialog);
    select_search_folder->set_access(FileDialog::ACCESS_FILESYSTEM);
    select_search_folder->set_mode(FileDialog::MODE_OPEN_DIR);
    select_search_folder->set_title(TTR("Select a Folder to Search"));
    select_search_folder->set_current_dir(EditorSettings::get_singleton()->get(
        "filesystem/directories/default_project_path"
    ));
    select_search_folder
        ->connect("dir_selected", this, "_on_search_folder_selected");
    return select_search_folder;
}

Control* ProjectsManager::_create_tabs() {
    tabs = memnew(TabContainer);
    tabs->set_anchors_and_margins_preset(Control::PRESET_WIDE);
    tabs->set_tab_align(TabContainer::ALIGN_LEFT);
    tabs->connect("tab_changed", this, "_on_tab_changed");

    tabs->add_child(_create_projects_tab());
    if (StreamPeerSSL::is_available()) {
        asset_library = memnew(EditorAssetLibrary(true));
        asset_library->set_name(TTR("Asset Library Projects"));
        asset_library->connect("install_asset", this, "_on_install_asset");
        tabs->add_child(asset_library);
    } else {
        WARN_PRINT("Asset Library not available, as it requires SSL to work.");
    }
    tabs->set_current_tab(0);

    return tabs;
}

Control* ProjectsManager::_create_tools() {
    HBoxContainer* tools_container = memnew(HBoxContainer);
    tools_container->set_anchors_and_margins_preset(Control::PRESET_TOP_RIGHT);
    tools_container->set_alignment(BoxContainer::ALIGN_END);
    tools_container->set_h_grow_direction(Control::GROW_DIRECTION_BEGIN);

    // Clickable version label.
    VBoxContainer* version_container = memnew(VBoxContainer);
    // Pushes the version label down.
    Control* version_spacer          = memnew(Control);
    version_container->add_child(version_spacer);
    version_label       = memnew(LinkButton);
    String version_hash = String(VERSION_HASH);
    if (!version_hash.empty()) {
        version_hash = vformat(" [%s]", version_hash.left(9));
    }
    version_label->set_text(vformat("v%s%s", VERSION_FULL_BUILD, version_hash));
    version_label->set_self_modulate(Color(1, 1, 1, 0.6));
    version_label->set_underline_mode(LinkButton::UNDERLINE_MODE_ON_HOVER);
    version_label->set_tooltip(TTR("Click to copy."));
    version_label->connect("pressed", this, "_on_version_label_pressed");
    version_container->add_child(version_label);
    tools_container->add_child(version_container);

    tools_container->add_spacer();

    tools_container->add_child(_create_language_options());

    return tools_container;
}

Control* ProjectsManager::_create_upgrade_settings_confirmation() {
    upgrade_settings_confirmation = memnew(ConfirmationDialog);
    upgrade_settings_confirmation->get_ok()
        ->connect("pressed", this, "_on_upgrade_settings_confirmed", varray());
    return upgrade_settings_confirmation;
}

void ProjectsManager::_add_file(const String& p_file) {
    DirAccessRef dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (dir_access->dir_exists(p_file)) {
        // File is a folder.
        _search_folder(p_file);
        return;
    }

    String folder    = p_file.get_base_dir();
    String file_name = p_file.get_file();
    if (file_name == "project.rebel") {
        _add_project(folder);
    } else if (file_name.ends_with(".zip")) {
        _extract_zip_file(p_file);
    }
}

void ProjectsManager::_add_multiple_files(const PoolStringArray& p_files) {
    for (int i = 0; i < p_files.size(); i++) {
        _add_file(p_files[i]);
    }
}

void ProjectsManager::_add_project(const String& p_folder) {
    String folder = p_folder.replace("\\", "/");
    if (folder.ends_with("/")) {
        folder = folder.substr(0, folder.length() - 1);
    }
    String project_key  = folder.replace("/", "::");
    String property_key = "projects/" + project_key;
    EditorSettings::get_singleton()->set(property_key, folder);
    EditorSettings::get_singleton()->save();

    projects_list->add_project(project_key);
}

void ProjectsManager::_dim_window() {
    float c         = 0.5f;
    Color dim_color = Color(c, c, c);
    set_modulate(dim_color);
}

void ProjectsManager::_edit_project(const String& project_key) {
    projects_list->show_loading();
    Vector<ProjectsListItem*> selected_project_items =
        projects_list->get_selected_project_items();
    for (int i = 0; i < selected_project_items.size(); i++) {
        ProjectsListItem* project = selected_project_items[i];
        if (project->project_key == project_key) {
            String project_name   = project->project_name;
            String project_folder = project->project_folder;
            edit_project(project_name, project_folder);
            break;
        }
    }
    // Quit projects manager on next iteration.
    editing_project = true;
    set_process(true);
}

void ProjectsManager::_edit_selected_projects() {
    Vector<ProjectsListItem*> selected_project_items =
        projects_list->get_selected_project_items();
    for (int i = 0; i < selected_project_items.size(); i++) {
        const ProjectsListItem* project = selected_project_items[i];
        int config_version              = project->version;
        String project_key              = project->project_key;
        String project_folder           = project->project_folder;
        String project_settings_file =
            project_folder.plus_file("project.rebel");

        if (project->missing) {
            continue;
        }
        if (!FileAccess::exists(project_settings_file)) {
            _popup_no_settings_file_error(project_settings_file);
            continue;
        }
        if (config_version > ProjectSettings::CONFIG_VERSION) {
            _popup_newer_settings_file_version_error(project_settings_file);
            continue;
        }
        if (config_version < ProjectSettings::CONFIG_VERSION) {
            _popup_upgrade_settings_confirmation(
                project_key,
                project_settings_file
            );
            continue;
        }

        _edit_project(project->project_key);
    }
}

void ProjectsManager::_edit_selected_projects_requested() {
    const Vector<ProjectsListItem*> selected_project_items =
        projects_list->get_selected_project_items();

    if (selected_project_items.empty()) {
        return;
    }

    if (selected_project_items.size() == 1) {
        _edit_selected_projects();
        return;
    }

    edit_multiple_confirmation->popup_centered_minsize();
}

void ProjectsManager::_extract_zip_file(
    const String& p_zip_file_path,
    const String& p_project_name
) {
    extract_zip_file_dialog->show_dialog(p_zip_file_path, p_project_name);
}

void ProjectsManager::_on_about_button_pressed() {
    _show_editor_about();
}

void ProjectsManager::_on_add_button_pressed() {
    add_project_dialog->show_dialog();
}

void ProjectsManager::_on_edit_button_pressed() {
    _edit_selected_projects_requested();
}

void ProjectsManager::_on_edit_multiple_confirmed() {
    _edit_selected_projects();
}

void ProjectsManager::_on_extract_zip_file(const String& p_zip_file_path) {
    _extract_zip_file(p_zip_file_path);
}

void ProjectsManager::_on_files_dropped(const PoolStringArray& p_files, int) {
    if (p_files.size() == 1) {
        _add_file(p_files[0]);
        return;
    }
    _popup_add_multiple_files_confirmation(p_files);
}

void ProjectsManager::_on_global_menu_action(
    const Variant& p_id,
    const Variant& p_meta
) {
    int id = (int)p_id;
    if (id == ProjectsList::GLOBAL_NEW_WINDOW) {
        List<String> args;
        args.push_back("-p");
        String exec = OS::get_singleton()->get_executable_path();

        OS::ProcessID pid = 0;
        OS::get_singleton()->execute(exec, args, false, &pid);
    } else if (id == ProjectsList::GLOBAL_OPEN_PROJECT) {
        String conf = (String)p_meta;

        if (!conf.empty()) {
            List<String> args;
            args.push_back(conf);
            String exec = OS::get_singleton()->get_executable_path();

            OS::ProcessID pid = 0;
            OS::get_singleton()->execute(exec, args, false, &pid);
        }
    }
}

void ProjectsManager::_on_import_button_pressed() {
    import_project_dialog->show_dialog();
}

void ProjectsManager::_on_import_godot_project(
    const String& p_project_file,
    const String& p_destination_folder
) {
    print_line(
        vformat("Importing: %s to %s", p_project_file, p_destination_folder)
    );
}

void ProjectsManager::_on_install_asset(
    const String& p_zip_path,
    const String& p_title
) {
    _extract_zip_file(p_zip_path, p_title);
}

void ProjectsManager::_on_item_double_clicked() {
    _edit_selected_projects_requested();
}

void ProjectsManager::_on_language_selected(int p_id) {
    String language_code = language_options->get_item_metadata(p_id);
    EditorSettings::get_singleton()->set(
        "interface/editor/editor_language",
        language_code
    );
    language_options->set_text(language_code);
    language_options->set_icon(get_icon("Environment", "EditorIcons"));
    restart_confirmation->popup_centered();
}

void ProjectsManager::_on_new_project_button_pressed() {
    new_project_dialog->show_dialog();
}

void ProjectsManager::_on_open_asset_library_confirmed() {
    _open_asset_library();
}

void ProjectsManager::_on_project_added(const String& p_project_folder) {
    _add_project(p_project_folder);
    _edit_selected_projects_requested();
}

void ProjectsManager::_on_projects_updated() {
    projects_list->refresh_selected_projects();
}

void ProjectsManager::_on_rename_button_pressed() {
    const Vector<ProjectsListItem*> project_items =
        projects_list->get_selected_project_items();

    for (int i = 0; i < project_items.size(); i++) {
        rename_project_dialog->show_dialog(project_items[i]->project_folder);
    }
}

void ProjectsManager::_on_remove_button_pressed() {
    const Vector<ProjectsListItem*> selected_project_items =
        projects_list->get_selected_project_items();

    if (selected_project_items.empty()) {
        return;
    }

    String confirm_message;
    if (selected_project_items.size() > 1) {
        confirm_message = vformat(
            TTR("Remove %d projects from the list?"),
            selected_project_items.size()
        );
    } else {
        confirm_message = TTR("Remove selected project from the list?");
    }

    remove_confirmation_label->set_text(confirm_message);
    delete_project_folder_check_box->set_pressed(false);
    remove_confirmation->popup_centered_minsize();
}

void ProjectsManager::_on_remove_confirmed() {
    projects_list->remove_selected_projects(
        delete_project_folder_check_box->is_pressed()
    );
    _update_project_buttons();
}

void ProjectsManager::_on_remove_missing_button_pressed() {
    remove_missing_confirmation->popup_centered_minsize();
}

void ProjectsManager::_on_remove_missing_confirmed() {
    projects_list->remove_missing_projects();
    _update_project_buttons();
}

void ProjectsManager::_on_restart_confirmed() {
    OS* os            = OS::get_singleton();
    List<String> args = os->get_cmdline_args();
    String exec       = os->get_executable_path();
    OS::ProcessID pid = 0;
    Error error       = os->execute(exec, args, false, &pid);
    ERR_FAIL_COND(error);

    _quit();
}

void ProjectsManager::_on_run_button_pressed() {
    const Vector<ProjectsListItem*> selected_project_items =
        projects_list->get_selected_project_items();

    if (selected_project_items.empty()) {
        return;
    }

    if (selected_project_items.size() > 1) {
        run_multiple_confirmation->set_text(vformat(
            TTR("Are you sure to run %d projects at once?"),
            selected_project_items.size()
        ));
        run_multiple_confirmation->popup_centered_minsize();
    } else {
        _run_selected();
    }
}

void ProjectsManager::_on_run_multiple_confirmed() {
    _run_selected();
}

void ProjectsManager::_on_search_button_pressed() {
    select_search_folder->popup_centered_ratio();
}

void ProjectsManager::_on_add_multiple_files_confirmed(
    const PoolStringArray& p_files
) {
    _add_multiple_files(p_files);
}

void ProjectsManager::_on_search_folder_selected(const String& p_folder) {
    _search_folder(p_folder);
}

void ProjectsManager::_on_selection_changed() {
    _update_project_buttons();
}

void ProjectsManager::_on_tab_changed(int p_tab) {
    if (p_tab == 0) { // Projects
        projects_list->set_search_focus();
    }

    // The Templates tab's search field is focused on display in the asset
    // library editor plugin code.
}

void ProjectsManager::_on_upgrade_settings_confirmed(const String& p_project_key
) {
    _edit_project(p_project_key);
}

void ProjectsManager::_on_version_label_pressed() {
    OS::get_singleton()->set_clipboard(version_label->get_text());
}

void ProjectsManager::_open_asset_library() {
    asset_library->disable_community_support();
    tabs->set_current_tab(1);
}

void ProjectsManager::_popup_add_multiple_files_confirmation(
    const PoolStringArray& p_files
) {
    Button* ok_button = upgrade_settings_confirmation->get_ok();
    ok_button->disconnect("pressed", this, "_on_add_multiple_files_confirmed");
    ok_button->connect(
        "pressed",
        this,
        "_on_add_multiple_files_confirmed",
        varray(p_files)
    );
    add_multiple_files_confirmation->popup_centered_minsize();
}

void ProjectsManager::_popup_newer_settings_file_version_error(
    const String& p_file
) {
    String first_line = TTR("Can't open project settings file ");
    String second_line =
        TTR("The project settings were created with a newer version of Rebel "
            "Engine.\n");
    newer_settings_file_version_error->set_text(
        first_line + p_file + "\n" + second_line
    );
    newer_settings_file_version_error->popup_centered_minsize();
}

void ProjectsManager::_popup_no_settings_file_error(const String& p_file) {
    no_settings_file_error->set_text(
        vformat(TTR("Can't open project at '%s'."), p_file)
    );
    no_settings_file_error->popup_centered_minsize();
}

void ProjectsManager::_popup_upgrade_settings_confirmation(
    const String& p_project_key,
    const String& p_settings_file
) {
    upgrade_settings_confirmation->set_text(vformat(
        TTR("The following project settings file %s was created with an older "
            "version of Rebel Editor.\n"
            "Do you want to upgrade the settings file?\n"
            "Warning: You will not be able to open the project with the "
            "previous version of Rebel Editor again."),
        p_settings_file
    ));
    Button* ok_button = upgrade_settings_confirmation->get_ok();
    ok_button->disconnect("pressed", this, "_on_upgrade_settings_confirmed");
    ok_button->connect(
        "pressed",
        this,
        "_on_upgrade_settings_confirmed",
        varray(p_project_key)
    );
    upgrade_settings_confirmation->popup_centered_minsize();
}

void ProjectsManager::_quit() {
    _dim_window();
    get_tree()->quit();
}

void ProjectsManager::_run_selected() {
    Vector<ProjectsListItem*> selected_project_items =
        projects_list->get_selected_project_items();

    for (int i = 0; i < selected_project_items.size(); ++i) {
        const ProjectsListItem* project = selected_project_items[i];
        const String& main_scene        = project->main_scene;
        if (main_scene.empty()) {
            no_main_scene_defined_error->popup_centered();
            continue;
        }

        const String& project_folder = project->project_folder;
        const String& project_data_dir_name =
            ProjectSettings::get_singleton()->get_project_data_dir_name();
        if (!DirAccess::exists(project_folder + "/" + project_data_dir_name)) {
            no_assets_folder_error->popup_centered();
            continue;
        }

        const String& project_name = project->project_name;
        run_project(project_name, project_folder);
    }
}

void ProjectsManager::_search_folder(const String& p_folder) {
    DirAccessRef dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    Error error             = dir_access->change_dir(p_folder);
    ERR_FAIL_COND_MSG(error != OK, "Could not search directory: " + p_folder);

    dir_access->list_dir_begin();
    String dir_entry = dir_access->get_next();
    while (!dir_entry.empty()) {
        if (dir_access->current_is_dir() && !dir_entry.begins_with(".")) {
            _search_folder(p_folder.plus_file(dir_entry));
        } else if (dir_entry == "project.rebel") {
            print_line("Found Rebel Project in " + p_folder);
            _add_project(p_folder);
        }
        dir_entry = dir_access->get_next();
    }
}

void ProjectsManager::_show_editor_about() {
    editor_about->popup_centered(Size2(780, 500) * EDSCALE);
}

void ProjectsManager::_unhandled_input(const Ref<InputEvent>& p_event) {
    Ref<InputEventKey> key_event = p_event;
    if (!key_event.is_valid() || !key_event->is_pressed()) {
        return;
    }

#ifndef MACOS_ENABLED
    // Pressing Command + Q quits the application on macOS.
    // Therefore, only action the shortcut on other platforms.
    if (key_event->get_scancode_with_modifiers() == (KEY_MASK_CMD | KEY_Q)) {
        _quit();
    }
#endif

    if (tabs->get_current_tab() != 0) {
        // Only manage key presses on the projects tab.
        return;
    }

    bool scancode_handled = false;
    switch (key_event->get_scancode()) {
        case KEY_ENTER: {
            _edit_selected_projects_requested();
            scancode_handled = true;
        } break;
        default: {
            scancode_handled = projects_list->key_pressed(key_event);
        }
    }

    if (scancode_handled) {
        _update_project_buttons();
        accept_event();
    }
}

void ProjectsManager::_update_project_buttons() {
    Vector<ProjectsListItem*> selected_projects =
        projects_list->get_selected_project_items();

    bool missing_projects         = projects_list->is_any_project_missing();
    bool nothing_selected         = selected_projects.empty();
    bool missing_project_selected = false;
    for (int i = 0; i < selected_projects.size(); ++i) {
        if (selected_projects[i]->missing) {
            missing_project_selected = true;
            break;
        }
    }

    remove_missing_button->set_disabled(!missing_projects);
    edit_button->set_disabled(nothing_selected || missing_project_selected);
    run_button->set_disabled(nothing_selected || missing_project_selected);
    rename_button->set_disabled(nothing_selected || missing_project_selected);
    remove_button->set_disabled(nothing_selected);
}
