// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PROJECTS_LIST_H
#define PROJECTS_LIST_H

#include "core/set.h"
#include "core/ustring.h"
#include "core/vector.h"
#include "projects_manager/projects_list_item.h"
#include "scene/gui/box_container.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/scroll_container.h"

class ProjectsList : public VBoxContainer {
    GDCLASS(ProjectsList, VBoxContainer)

public:
    enum MenuOptions {
        GLOBAL_NEW_WINDOW,
        GLOBAL_OPEN_PROJECT
    };

    ProjectsList();

    void add_project(const String& project_key);
    int get_project_count() const;
    Vector<ProjectsListItem*> get_selected_project_items() const;
    bool is_any_project_missing() const;
    bool key_pressed(Ref<InputEventKey> key_event);
    void refresh_selected_projects();
    void remove_missing_projects();
    void remove_selected_projects(bool p_delete_project_folder);
    void set_search_focus();
    void show_loading();
    void update_dock_menu();

protected:
    static void _bind_methods();
    void _notification(int p_what);

private:
    Label* loading_label;

    LineEdit* search_box;
    OptionButton* sort_order_options;

    ProjectsListItem::SortOrder current_sort_order =
        ProjectsListItem::SortOrder::NAME;

    ScrollContainer* scroll_container;
    VBoxContainer* projects_container;

    Vector<ProjectsListItem*> projects;
    Vector<ProjectsListItem*> selected_projects;
    ProjectsListItem* first_selected_item = nullptr;

    int icon_load_index = 0;

    void _add_item_to_selection(ProjectsListItem* p_item);
    void _clear_selection();
    ProjectsListItem* _create_item(
        const String& project_key,
        bool favorite = false
    );
    void _ensure_item_visible(const ProjectsListItem* p_item);
    void _filter_projects();
    ProjectsListItem* _get_item(const String& project_key);
    void _load_project_icon(int p_index);
    void _load_projects();
    void _on_item_double_clicked();
    void _on_search_text_changed(const String&);
    void _on_selection_changed(
        bool p_shift_pressed,
        bool p_control_pressed,
        Node* p_node
    );
    void _on_sort_order_selected(int p_index);
    void _on_item_updated(const Node* p_node);
    void _refresh_projects_list();
    void _remove_project(ProjectsListItem* item);
    void _select_item(ProjectsListItem* p_item);
    void _select_range(ProjectsListItem* p_selected_item);
    void _sort_projects();
    void _toggle_item_selected(ProjectsListItem* p_item);
    void _update_icons_async();
};

#endif // PROJECTS_LIST_H
