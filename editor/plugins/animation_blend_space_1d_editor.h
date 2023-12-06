// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANIMATION_BLEND_SPACE_1D_EDITOR_H
#define ANIMATION_BLEND_SPACE_1D_EDITOR_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/plugins/animation_tree_editor_plugin.h"
#include "editor/property_editor.h"
#include "scene/animation/animation_blend_space_1d.h"
#include "scene/gui/button.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/popup.h"
#include "scene/gui/tree.h"

class AnimationNodeBlendSpace1DEditor : public AnimationTreeNodeEditorPlugin {
    GDCLASS(AnimationNodeBlendSpace1DEditor, AnimationTreeNodeEditorPlugin);

    Ref<AnimationNodeBlendSpace1D> blend_space;

    HBoxContainer* goto_parent_hb;
    ToolButton* goto_parent;

    PanelContainer* panel;
    ToolButton* tool_blend;
    ToolButton* tool_select;
    ToolButton* tool_create;
    VSeparator* tool_erase_sep;
    ToolButton* tool_erase;
    ToolButton* snap;
    SpinBox* snap_value;

    LineEdit* label_value;
    SpinBox* max_value;
    SpinBox* min_value;

    HBoxContainer* edit_hb;
    SpinBox* edit_value;
    Button* open_editor;

    int selected_point;

    Control* blend_space_draw;

    PanelContainer* error_panel;
    Label* error_label;

    bool updating;

    UndoRedo* undo_redo;

    static AnimationNodeBlendSpace1DEditor* singleton;

    void _blend_space_gui_input(const Ref<InputEvent>& p_event);
    void _blend_space_draw();

    void _update_space();

    void _config_changed(double);
    void _labels_changed(String);
    void _snap_toggled();

    PopupMenu* menu;
    PopupMenu* animations_menu;
    Vector<String> animations_to_add;
    float add_point_pos;
    Vector<float> points;

    bool dragging_selected_attempt;
    bool dragging_selected;
    Vector2 drag_from;
    Vector2 drag_ofs;

    void _add_menu_type(int p_index);
    void _add_animation_type(int p_index);

    void _tool_switch(int p_tool);
    void _update_edited_point_pos();
    void _update_tool_erase();
    void _erase_selected();
    void _edit_point_pos(double);
    void _open_editor();

    void _goto_parent();

    EditorFileDialog* open_file;
    Ref<AnimationNode> file_loaded;
    void _file_opened(const String& p_file);

    enum {
        MENU_LOAD_FILE         = 1000,
        MENU_PASTE             = 1001,
        MENU_LOAD_FILE_CONFIRM = 1002
    };

    StringName get_blend_position_path() const;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    static AnimationNodeBlendSpace1DEditor* get_singleton() {
        return singleton;
    }

    virtual bool can_edit(const Ref<AnimationNode>& p_node);
    virtual void edit(const Ref<AnimationNode>& p_node);
    AnimationNodeBlendSpace1DEditor();
};

#endif // ANIMATION_BLEND_SPACE_1D_EDITOR_H
