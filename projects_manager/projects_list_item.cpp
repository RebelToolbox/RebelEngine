// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "projects_list_item.h"

#include "core/os/os.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_themes.h"
#include "scene/gui/label.h"

ProjectsListItem::ProjectsListItem(
    const String& p_project_key,
    bool p_favorite
) :
    project_key(p_project_key),
    favorite(p_favorite) {
    set_theme(create_custom_theme());
    set_focus_mode(FocusMode::FOCUS_ALL);
    add_constant_override("separation", 10 * EDSCALE);

    Ref<Texture> favorite_icon = get_icon("Favorites", "EditorIcons");
    Color font_color           = get_color("font_color", "Tree");

    // Used to vertically center the favorite icon.
    VBoxContainer* favorite_button_container = memnew(VBoxContainer);
    favorite_button_container->set_alignment(BoxContainer::ALIGN_CENTER);
    add_child(favorite_button_container);

    favorite_button = memnew(TextureButton);
    favorite_button->set_normal_texture(favorite_icon);
    // Disables hover focus; so the whole item remains highlighted.
    favorite_button->set_mouse_filter(MOUSE_FILTER_PASS);
    if (!favorite) {
        favorite_button->set_modulate(Color(1, 1, 1, 0.2f));
    }
    favorite_button->connect("pressed", this, "_on_favorite_pressed");
    favorite_button_container->add_child(favorite_button);

    icon_texture = memnew(TextureRect);
    // Project icons are loaded asynchronously; so use the project loading icon.
    icon_texture->set_texture(get_icon("ProjectIconLoading", "EditorIcons"));
    icon_texture->set_v_size_flags(SIZE_SHRINK_CENTER);
    add_child(icon_texture);

    project_details_container = memnew(VBoxContainer);
    project_details_container->set_h_size_flags(SIZE_EXPAND_FILL);
    add_child(project_details_container);

    project_name_label = memnew(Label());
    project_name_label->add_font_override(
        "font",
        get_font("title", "EditorFonts")
    );
    project_name_label->add_color_override("font_color", font_color);
    project_name_label->set_clip_text(true);
    project_details_container->add_child(project_name_label);

    HBoxContainer* project_folder_container = memnew(HBoxContainer);
    project_folder_container->set_h_size_flags(SIZE_EXPAND_FILL);
    project_details_container->add_child(project_folder_container);

    show_folder_button = memnew(Button);
    project_folder_container->add_child(show_folder_button);

    project_folder_label = memnew(Label());
    project_folder_label->set_h_size_flags(SIZE_EXPAND_FILL);
    project_folder_label->set_modulate(Color(1, 1, 1, 0.5));
    project_folder_label->add_color_override("font_color", font_color);
    project_folder_label->set_clip_text(true);
    project_folder_container->add_child(project_folder_label);

    _configure_item();
}

void ProjectsListItem::refresh_item() {
    _reset_item();
    _configure_item();
}

void ProjectsListItem::set_icon_texture(const Ref<Texture>& new_icon_texture) {
    icon_texture->set_texture(new_icon_texture);
}

void ProjectsListItem::_bind_methods() {
    ClassDB::bind_method("_on_draw", &ProjectsListItem::_on_draw);
    ClassDB::bind_method("_on_gui_input", &ProjectsListItem::_on_gui_input);
    ClassDB::bind_method(
        "_on_favorite_pressed",
        &ProjectsListItem::_on_favorite_pressed
    );
    ClassDB::bind_method(
        "_on_show_folder_pressed",
        &ProjectsListItem::_on_show_folder_pressed
    );

    ADD_SIGNAL(MethodInfo("item_double_clicked"));
    ADD_SIGNAL(MethodInfo("item_updated"));
    ADD_SIGNAL(MethodInfo(
        "selection_changed",
        PropertyInfo(Variant::BOOL, "shift_pressed"),
        PropertyInfo(Variant::BOOL, "control_pressed")
    ));
}

void ProjectsListItem::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_MOUSE_ENTER: {
            hover = true;
            update();
        } break;
        case NOTIFICATION_MOUSE_EXIT: {
            hover = false;
            update();
        } break;
        case NOTIFICATION_DRAW: {
            _on_draw();
        } break;
    }
}

void ProjectsListItem::_configure_item() {
    _extract_project_values();

    project_folder_label->set_text(project_folder);
    set_tooltip(description);

    if (disabled) {
        show_folder_button->set_modulate(Color(1, 1, 1, 1));
        project_details_container->set_modulate(Color(1, 1, 1, 0.5));
    } else {
        show_folder_button->set_modulate(Color(1, 1, 1, 0.5));
        project_details_container->set_modulate(Color(1, 1, 1, 1));
    }

    if (missing) {
        project_name_label->set_text(TTR("Missing project"));
        icon_texture->set_modulate(Color(1, 1, 1, 0.5));
        show_folder_button->set_icon(get_icon("FileBroken", "EditorIcons"));
        show_folder_button->set_tooltip(TTR("Error: Project settings not found")
        );
    } else {
        project_name_label->set_text(project_name);
        icon_texture->set_modulate(Color(1, 1, 1, 1));
        show_folder_button->set_icon(get_icon("Load", "EditorIcons"));
        show_folder_button->set_tooltip(TTR("Show in File Manager"));
        show_folder_button->connect(
            "pressed",
            this,
            "_on_show_folder_pressed",
            varray(project_folder)
        );
    }
}

void ProjectsListItem::_extract_project_values() {
    project_folder = project_key.replace(":::", ":/");
    project_folder = project_folder.replace("::", "/");

    Ref<ConfigFile> settings_file = memnew(ConfigFile);
    String settings_file_name     = project_folder.plus_file("project.rebel");
    Error settings_file_error     = settings_file->load(settings_file_name);
    if (settings_file_error == OK) {
        project_name = settings_file->get_value(
            "application",
            "config/name",
            TTR("Unnamed project")
        );
        description =
            settings_file->get_value("application", "config/description", "");
        icon_path = settings_file->get_value("application", "config/icon", "");
        main_scene =
            settings_file->get_value("application", "run/main_scene", "");
        version = (int)settings_file->get_value("", "config_version", 0);
    }

    if (version > ProjectSettings::CONFIG_VERSION) {
        // It comes from an more recent, non-backward compatible version.
        disabled = true;
    }

    if (FileAccess::exists(settings_file_name)) {
        last_modified  = FileAccess::get_modified_time(settings_file_name);
        String fscache = project_folder.plus_file(".fscache");
        if (FileAccess::exists(fscache)) {
            uint64_t cache_modified = FileAccess::get_modified_time(fscache);
            if (cache_modified > last_modified) {
                last_modified = cache_modified;
            }
        }
    } else {
        disabled = true;
        missing  = true;
        print_line("Project settings file is missing: " + settings_file_name);
    }
}

void ProjectsListItem::_on_draw() {
    Size2 item_size = get_size();

    draw_line(
        Point2(0, item_size.y + 1),
        Point2(item_size.x - 10, item_size.y + 1),
        get_color("guide_color", "Tree")
    );

    if (hover) {
        draw_style_box(
            get_stylebox("hover", "Tree"),
            Rect2(Point2(), get_size() - Size2(10, 0) * EDSCALE)
        );
    }

    if (selected) {
        draw_style_box(
            get_stylebox("selected", "Tree"),
            Rect2(Point2(), item_size - Size2(10, 0) * EDSCALE)
        );
    }
}

void ProjectsListItem::_on_gui_input(const Ref<InputEvent>& p_input_event) {
    Ref<InputEventMouseButton> mouse_button_event = p_input_event;
    if (!mouse_button_event.is_valid() || !mouse_button_event->is_pressed()
        || mouse_button_event->get_button_index() != BUTTON_LEFT) {
        return;
    }

    bool control_pressed = mouse_button_event->get_control();
    bool shift_pressed   = mouse_button_event->get_shift();
    bool double_clicked  = mouse_button_event->is_doubleclick();

    emit_signal("selection_changed", shift_pressed, control_pressed);

    if (!control_pressed && double_clicked) {
        emit_signal("item_double_clicked");
    }
}

void ProjectsListItem::_on_favorite_pressed() {
    favorite = !favorite;

    if (favorite) {
        favorite_button->set_modulate(Color(1, 1, 1, 1));
        EditorSettings::get_singleton()->set(
            "favorite_projects/" + project_key,
            project_folder
        );
    } else {
        favorite_button->set_modulate(Color(1, 1, 1, 0.2f));
        EditorSettings::get_singleton()->erase(
            "favorite_projects/" + project_key
        );
    }
    EditorSettings::get_singleton()->save();

    emit_signal("item_updated");
}

void ProjectsListItem::_on_show_folder_pressed(const String& p_folder) {
    OS::get_singleton()->shell_open(String("file://") + p_folder);
}

void ProjectsListItem::_reset_item() {
    icon_loaded = false;
    disabled    = false;
    missing     = false;
    if (show_folder_button
            ->is_connected("pressed", this, "_on_show_folder_pressed")) {
        show_folder_button
            ->disconnect("pressed", this, "_on_show_folder_pressed");
    }
}

// operator<
bool ProjectsListItemComparator::operator()(
    const ProjectsListItem* a,
    const ProjectsListItem* b
) const {
    if (a->favorite && !b->favorite) {
        return true;
    }
    if (b->favorite && !a->favorite) {
        return false;
    }
    switch (sort_order) {
        case ProjectsListItem::SortOrder::NAME:
            if (a->project_name.empty()) {
                return false;
            }
            if (b->project_name.empty()) {
                return true;
            }
            return a->project_name < b->project_name;
        case ProjectsListItem::SortOrder::PATH:
            return a->project_key < b->project_key;
        case ProjectsListItem::SortOrder::LAST_MODIFIED:
            return a->last_modified > b->last_modified;
        default:
            ERR_FAIL_V_MSG(false, "Unrecognised sort order");
    }
}
