// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PROJECTS_LIST_ITEM_H
#define PROJECTS_LIST_ITEM_H

#include "core/ustring.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/texture_button.h"
#include "scene/gui/texture_rect.h"

class ProjectsListItem : public HBoxContainer {
    GDCLASS(ProjectsListItem, HBoxContainer)

public:
    enum class SortOrder {
        NAME,
        PATH,
        LAST_MODIFIED,
    };

    String project_key;
    String project_name;
    String project_folder;
    String description;
    String icon_path;
    String main_scene;
    int version            = 0;
    uint64_t last_modified = 0;
    bool selected          = false;
    bool favorite          = false;
    bool disabled          = false;
    bool missing           = false;
    bool icon_loaded       = false;

    ProjectsListItem(const String& p_project_key, bool p_favorite);

    void refresh_item();
    void set_icon_texture(const Ref<Texture>& new_icon_texture);

protected:
    static void _bind_methods();
    void _notification(int p_what);

private:
    Button* show_folder_button;
    Label* project_folder_label;
    Label* project_name_label;
    TextureButton* favorite_button;
    TextureRect* icon_texture;
    VBoxContainer* project_details_container;

    bool hover = false;

    void _configure_item();
    void _extract_project_values();
    void _on_draw();
    void _on_gui_input(const Ref<InputEvent>& p_input_event);
    void _on_favorite_pressed();
    void _on_show_folder_pressed(const String& p_folder);
    void _reset_item();
};

class ProjectsListItemComparator {
public:
    ProjectsListItem::SortOrder sort_order;

    bool operator()(const ProjectsListItem* a, const ProjectsListItem* b) const;
};

#endif // PROJECTS_LIST_ITEM_H
