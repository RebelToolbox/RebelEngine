// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "projects_list.h"

#include "core/io/config_file.h"
#include "core/os/keyboard.h"
#include "core/os/os.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_themes.h"
#include "scene/gui/label.h"
#include "scene/gui/panel_container.h"

namespace {
Set<String> get_favorites(List<PropertyInfo>& properties) {
    Set<String> favorites;
    for (List<PropertyInfo>::Element* E = properties.front(); E;
         E                              = E->next()) {
        String property_key = E->get().name;
        if (property_key.begins_with("favorite_projects/")) {
            String project_key = property_key.get_slice("/", 1);
            favorites.insert(project_key);
        }
    }
    return favorites;
}
} // namespace

ProjectsList::ProjectsList() {
    set_theme(create_custom_theme());
    set_h_size_flags(SIZE_EXPAND_FILL);

    // Projects List Tools
    HBoxContainer* projects_list_tools_container = memnew(HBoxContainer);
    add_child(projects_list_tools_container);

    loading_label = memnew(Label(TTR("Loading, please wait...")));
    loading_label->add_font_override("font", get_font("bold", "EditorFonts"));
    loading_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    // Hide the label until it's needed.
    loading_label->set_modulate(Color(0, 0, 0, 0));
    projects_list_tools_container->add_child(loading_label);

    Label* sort_label = memnew(Label);
    sort_label->set_text(TTR("Sort:"));
    projects_list_tools_container->add_child(sort_label);

    sort_order_options = memnew(OptionButton);
    sort_order_options->set_clip_text(true);
    sort_order_options
        ->connect("item_selected", this, "_on_sort_order_selected");
    sort_order_options->set_custom_minimum_size(Size2(180, 10) * EDSCALE);
    sort_order_options->add_item(TTR("Name"));
    sort_order_options->add_item(TTR("Path"));
    sort_order_options->add_item(TTR("Last Modified"));
    int previous_sort_order =
        EditorSettings::get_singleton()->get("projects_manager/sorting_order");
    current_sort_order = (ProjectsListItem::SortOrder)previous_sort_order;
    sort_order_options->select(previous_sort_order);
    projects_list_tools_container->add_child(sort_order_options);

    search_box = memnew(LineEdit);
    search_box->set_placeholder(TTR("Filter projects"));
    search_box->set_tooltip(
        TTR("This field filters projects by name and last path component.\n"
            "To filter projects by name and full path, the query must contain "
            "at least one `/` character.")
    );
    search_box->connect("text_changed", this, "_on_search_text_changed");
    search_box->set_h_size_flags(SIZE_EXPAND_FILL);
    search_box->set_custom_minimum_size(Size2(280, 10) * EDSCALE);
    projects_list_tools_container->add_child(search_box);

    // Projects
    PanelContainer* panel_container = memnew(PanelContainer);
    panel_container->add_style_override("panel", get_stylebox("bg", "Tree"));
    panel_container->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(panel_container);

    scroll_container = memnew(ScrollContainer);
    scroll_container->set_enable_h_scroll(false);
    panel_container->add_child(scroll_container);

    projects_container = memnew(VBoxContainer);
    projects_container->set_h_size_flags(SIZE_EXPAND_FILL);
    scroll_container->add_child(projects_container);

    _load_projects();
}

void ProjectsList::add_project(const String& project_key) {
    ProjectsListItem* item = _get_item(project_key);
    if (item) {
        item->refresh_item();
    } else {
        item = _create_item(project_key);
    }
    // Select the project.
    search_box->clear();
    _refresh_projects_list();
    _select_index(item->get_index());
}

int ProjectsList::get_project_count() const {
    return projects.size();
}

Vector<ProjectsListItem*> ProjectsList::get_selected_project_items() const {
    return selected_projects;
}

bool ProjectsList::is_any_project_missing() const {
    for (int i = 0; i < projects.size(); ++i) {
        if (projects[i]->missing) {
            return true;
        }
    }
    return false;
}

bool ProjectsList::key_pressed(Ref<InputEventKey> key_event) {
    switch (key_event->get_scancode()) {
        case KEY_HOME: {
            if (get_project_count() > 0) {
                _select_index(0);
                return true;
            }
        } break;
        case KEY_END: {
            if (get_project_count() > 0) {
                _select_index(get_project_count() - 1);
                return true;
            }
        } break;
        case KEY_UP: {
            if (first_selected_project_index > 0 && !key_event->get_shift()) {
                _select_index(first_selected_project_index - 1);
                return true;
            }
        } break;
        case KEY_DOWN: {
            if (first_selected_project_index < get_project_count() - 2
                && !key_event->get_shift()) {
                _select_index(first_selected_project_index + 1);
                return true;
            }
        } break;
        case KEY_F: {
            if (key_event->get_command()) {
                set_search_focus();
                return true;
            }
        } break;
    }
    return false;
}

void ProjectsList::refresh_selected_projects() {
    if (selected_projects.empty()) {
        return;
    }

    Vector<ProjectsListItem*> selected_project_items =
        get_selected_project_items();
    for (int i = 0; i < selected_project_items.size(); ++i) {
        selected_project_items[i]->refresh_item();
        selected_project_items[i]->icon_loaded = false;
    }

    _sort_projects();
    _update_icons_async();
    _ensure_item_visible(selected_project_items[0]->get_index());
    update_dock_menu();
}

void ProjectsList::remove_missing_projects() {
    if (projects.empty()) {
        return;
    }

    for (int i = projects.size() - 1; i >= 0; i--) {
        ProjectsListItem* item = projects[i];
        if (item->missing) {
            selected_projects.erase(item);
            _remove_project(item);
        }
    }

    EditorSettings::get_singleton()->save();
}

void ProjectsList::remove_selected_projects(bool p_delete_project_folder) {
    if (selected_projects.empty()) {
        return;
    }

    for (int i = 0; i < selected_projects.size(); i++) {
        ProjectsListItem* item = selected_projects[i];
        if (item->is_visible()) {
            if (p_delete_project_folder) {
                OS::get_singleton()->move_to_trash(item->project_folder);
            }
            _remove_project(item);
        }
    }
    selected_projects.clear();

    EditorSettings::get_singleton()->save();
    update_dock_menu();
}

void ProjectsList::set_search_focus() {
    search_box->grab_focus();
}

void ProjectsList::show_loading() {
    loading_label->set_modulate(Color(1, 1, 1));
}

void ProjectsList::update_dock_menu() {
    OS* os = OS::get_singleton();
    os->global_menu_clear("_dock");

    bool add_favorite_separator = false;
    bool add_item_separator     = false;
    for (int i = 0; i < projects.size(); ++i) {
        const ProjectsListItem* project = projects[i];
        if (project->disabled || project->missing) {
            continue;
        }

        if (project->favorite) {
            add_favorite_separator = true;
        } else {
            if (add_favorite_separator) {
                os->global_menu_add_separator("_dock");
            }
            add_favorite_separator = false;
        }

        os->global_menu_add_item(
            "_dock",
            project->project_name + " ( " + project->project_folder + " )",
            GLOBAL_OPEN_PROJECT,
            Variant(project->project_folder.plus_file("project.rebel"))
        );
        add_item_separator = true;
    }
    if (add_item_separator) {
        os->global_menu_add_separator("_dock");
    }

    os->global_menu_add_item(
        "_dock",
        TTR("New Window"),
        GLOBAL_NEW_WINDOW,
        Variant()
    );
}

void ProjectsList::_bind_methods() {
    ClassDB::bind_method(
        "_on_item_double_clicked",
        &ProjectsList::_on_item_double_clicked
    );
    ClassDB::bind_method("_on_item_updated", &ProjectsList::_on_item_updated);
    ClassDB::bind_method(
        "_on_search_text_changed",
        &ProjectsList::_on_search_text_changed
    );
    ClassDB::bind_method(
        "_on_selection_changed",
        &ProjectsList::_on_selection_changed
    );
    ClassDB::bind_method(
        "_on_sort_order_selected",
        &ProjectsList::_on_sort_order_selected
    );

    ADD_SIGNAL(MethodInfo("selection_changed"));
    ADD_SIGNAL(MethodInfo("item_double_clicked"));
}

void ProjectsList::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_PROCESS: {
            // Load icons as a co-routine to speed up launch
            // when there are many projects.
            if (icon_load_index < projects.size()) {
                const ProjectsListItem* item = projects[icon_load_index];
                if (!item->icon_loaded) {
                    _load_project_icon(icon_load_index);
                }
                icon_load_index++;
            } else {
                set_process(false);
            }
        } break;

        case NOTIFICATION_READY: {
            if (get_project_count() >= 1) {
                // Focus on the search box immediately to allow the user
                // to search without having to reach for their mouse
                search_box->grab_focus();
            }
        } break;
    }
}

void ProjectsList::_add_item_to_selection(ProjectsListItem* p_item) {
    selected_projects.push_back(p_item);
    p_item->selected = true;
    p_item->update();
}

void ProjectsList::_clear_selection() {
    selected_projects.clear();
    for (int index = 0; index < projects.size(); index++) {
        projects[index]->selected = false;
        projects[index]->update();
    }
}

ProjectsListItem* ProjectsList::_create_item(
    const String& project_key,
    bool favorite
) {
    ProjectsListItem* item = memnew(ProjectsListItem(project_key, favorite));
    item->connect("item_double_clicked", this, "_on_item_double_clicked");
    item->connect("item_updated", this, "_on_item_updated", varray(item));
    item->connect(
        "selection_changed",
        this,
        "_on_selection_changed",
        varray(item)
    );
    item->connect("gui_input", item, "_on_gui_input");
    projects_container->add_child(item);
    projects.push_back(item);
    return item;
}

void ProjectsList::_ensure_item_visible(int p_index) {
    scroll_container->call_deferred(
        "ensure_control_visible",
        projects[p_index]
    );
}

void ProjectsList::_filter_projects() {
    String search_text = search_box->get_text();
    if (search_text.empty()) {
        for (int i = 0; i < projects.size(); ++i) {
            projects[i]->set_visible(true);
        }
        update_dock_menu();
        return;
    }

    bool use_full_path = search_text.find("/") != -1;
    for (int i = 0; i < projects.size(); ++i) {
        ProjectsListItem* item = projects[i];

        String project_path;
        if (use_full_path) {
            project_path = item->project_folder;
        } else {
            // Only search project folder name.
            project_path = item->project_folder.get_file();
        }

        bool visible = item->project_name.findn(search_text) != -1
                    || project_path.findn(search_text) != -1;
        item->set_visible(visible);
    }
    update_dock_menu();
}

ProjectsListItem* ProjectsList::_get_item(const String& project_key) {
    for (int index = 0; index < projects.size(); index++) {
        if (projects[index]->project_key == project_key) {
            return projects[index];
        }
    }
    return nullptr;
}

void ProjectsList::_load_project_icon(int p_index) {
    ProjectsListItem* item = projects[p_index];

    Ref<Texture> default_icon = get_icon("DefaultProjectIcon", "EditorIcons");
    Ref<Texture> icon;
    if (!item->icon_path.empty()) {
        Ref<Image> image;
        image.instance();
        Error err = image->load(
            item->icon_path.replace_first("res://", item->project_folder + "/")
        );
        if (err == OK) {
            image->resize(
                default_icon->get_width(),
                default_icon->get_height(),
                Image::INTERPOLATE_LANCZOS
            );
            Ref<ImageTexture> it = memnew(ImageTexture);
            it->create_from_image(image);
            icon = it;
        }
    }
    if (icon.is_null()) {
        icon = default_icon;
    }

    item->set_icon_texture(icon);
    item->icon_loaded = true;
}

void ProjectsList::_load_projects() {
    List<PropertyInfo> properties;
    EditorSettings::get_singleton()->get_property_list(&properties);

    Set<String> favorites = get_favorites(properties);

    for (List<PropertyInfo>::Element* E = properties.front(); E;
         E                              = E->next()) {
        String property_key = E->get().name;
        if (!property_key.begins_with("projects/")) {
            continue;
        }
        String project_key = property_key.get_slice("/", 1);
        bool favorite      = favorites.has(project_key);
        _create_item(project_key, favorite);
    }

    _refresh_projects_list();
    scroll_container->set_v_scroll(0);
}

void ProjectsList::_on_item_double_clicked() {
    emit_signal("item_double_clicked");
}

void ProjectsList::_on_search_text_changed(const String&) {
    _filter_projects();
}

void ProjectsList::_on_selection_changed(
    bool p_shift_pressed,
    bool p_control_pressed,
    Node* p_node
) {
    ProjectsListItem* item = Object::cast_to<ProjectsListItem>(p_node);

    if (p_shift_pressed) {
        _select_range(item);
    } else if (p_control_pressed) {
        _toggle_item_selected(item);
    } else {
        _select_index(item->get_index());
    }

    emit_signal("selection_changed");
}

void ProjectsList::_on_sort_order_selected(int p_index) {
    ProjectsListItem::SortOrder selected_sort_order =
        (ProjectsListItem::SortOrder)(p_index);
    if (current_sort_order == selected_sort_order) {
        return;
    }
    EditorSettings* editor_settings = EditorSettings::get_singleton();
    editor_settings->set("projects_manager/sorting_order", p_index);
    editor_settings->save();
    current_sort_order = selected_sort_order;
    _sort_projects();
}

void ProjectsList::_on_item_updated(const Node* p_node) {
    _sort_projects();

    const ProjectsListItem* item = Object::cast_to<ProjectsListItem>(p_node);
    if (item->favorite) {
        for (int i = 0; i < projects.size(); ++i) {
            if (projects[i]->project_key == item->project_key) {
                _ensure_item_visible(i);
                break;
            }
        }
    }
}

void ProjectsList::_refresh_projects_list() {
    _filter_projects();
    _sort_projects();
    _update_icons_async();
}

void ProjectsList::_remove_project(ProjectsListItem* item) {
    if (item->get_index() == first_selected_project_index) {
        first_selected_project_index = -1;
    }

    EditorSettings::get_singleton()->erase("projects/" + item->project_key);
    EditorSettings::get_singleton()->erase(
        "favorite_projects/" + item->project_key
    );

    projects.erase(item);
    memdelete(item);
    update_dock_menu();
}

void ProjectsList::_sort_projects() {
    SortArray<ProjectsListItem*, ProjectsListItemComparator> sorter;
    sorter.compare.sort_order = current_sort_order;
    sorter.sort(projects.ptrw(), projects.size());

    for (int i = 0; i < projects.size(); ++i) {
        ProjectsListItem* item = projects[i];
        item->get_parent()->move_child(item, i);
    }

    if (icon_load_index < projects.size()) {
        // Restart updating icons, because the order has changed.
        _update_icons_async();
    }
    update_dock_menu();
}

void ProjectsList::_select_index(int p_index) {
    _clear_selection();
    _add_item_to_selection(projects[p_index]);
    first_selected_project_index = p_index;
    _ensure_item_visible(p_index);
}

void ProjectsList::_select_range(ProjectsListItem* p_to_item) {
    int selected_index = p_to_item->get_index();
    if (first_selected_project_index == -1
        || selected_index == first_selected_project_index) {
        _select_index(selected_index);
        return;
    }

    _clear_selection();
    bool inside_range = false;
    for (int index = 0; index < projects.size(); index++) {
        ProjectsListItem* item = projects[index];
        int item_index         = item->get_index();
        if (item_index == first_selected_project_index
            || item_index == selected_index) {
            _add_item_to_selection(item);
            inside_range = !inside_range;
        } else if (inside_range) {
            _add_item_to_selection(item);
        }
    }
}

void ProjectsList::_toggle_item_selected(ProjectsListItem* p_item) {
    p_item->selected = !p_item->selected;
    if (p_item->selected) {
        selected_projects.push_back(p_item);
    } else {
        selected_projects.erase(p_item);
    }
    p_item->update();
}

void ProjectsList::_update_icons_async() {
    icon_load_index = 0;
    set_process(true);
}
