// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef THEME_EDITOR_PREVIEW_H
#define THEME_EDITOR_PREVIEW_H

#include "editor/editor_node.h"
#include "scene/gui/box_container.h"
#include "scene/gui/check_box.h"
#include "scene/gui/check_button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/color_rect.h"
#include "scene/gui/label.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel.h"
#include "scene/gui/progress_bar.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/tree.h"
#include "scene/resources/theme.h"

class ThemeEditorPreview : public VBoxContainer {
    GDCLASS(ThemeEditorPreview, VBoxContainer);

    ScrollContainer* preview_container;
    ColorRect* preview_bg;
    MarginContainer* preview_overlay;
    Control* picker_overlay;
    Control* hovered_control = nullptr;

    struct ThemeCache {
        Ref<StyleBox> preview_picker_overlay;
        Color preview_picker_overlay_color;
        Ref<StyleBox> preview_picker_label;
        Ref<Font> preview_picker_font;
    } theme_cache;

    double time_left = 0;

    void _propagate_redraw(Control* p_at);
    void _refresh_interval();
    void _preview_visibility_changed();

    void _picker_button_cbk();
    Control* _find_hovered_control(Control* p_parent, Vector2 p_mouse_position);

    void _draw_picker_overlay();
    void _gui_input_picker_overlay(const Ref<InputEvent>& p_event);
    void _reset_picker_overlay();

protected:
    HBoxContainer* preview_toolbar;
    MarginContainer* preview_content;
    Button* picker_button;

    void add_preview_overlay(Control* p_overlay);

    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_preview_theme(const Ref<Theme>& p_theme);

    ThemeEditorPreview();
};

class DefaultThemeEditorPreview : public ThemeEditorPreview {
    GDCLASS(DefaultThemeEditorPreview, ThemeEditorPreview);

public:
    DefaultThemeEditorPreview();
};

class SceneThemeEditorPreview : public ThemeEditorPreview {
    GDCLASS(SceneThemeEditorPreview, ThemeEditorPreview);

    Ref<PackedScene> loaded_scene;

    Button* reload_scene_button;

    void _reload_scene();

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    bool set_preview_scene(const String& p_path);
    String get_preview_scene_path() const;

    SceneThemeEditorPreview();
};

#endif // THEME_EDITOR_PREVIEW_H
