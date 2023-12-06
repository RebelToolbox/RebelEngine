// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "skeleton_2d_editor_plugin.h"

#include "canvas_item_editor_plugin.h"
#include "scene/2d/mesh_instance_2d.h"
#include "scene/gui/box_container.h"
#include "thirdparty/misc/clipper.hpp"

void Skeleton2DEditor::_node_removed(Node* p_node) {
    if (p_node == node) {
        node = nullptr;
        options->hide();
    }
}

void Skeleton2DEditor::edit(Skeleton2D* p_sprite) {
    node = p_sprite;
}

void Skeleton2DEditor::_menu_option(int p_option) {
    if (!node) {
        return;
    }

    switch (p_option) {
        case MENU_OPTION_SET_REST: {
            if (node->get_bone_count() == 0) {
                err_dialog->set_text(
                    TTR("This skeleton has no bones, create some children "
                        "Bone2D nodes.")
                );
                err_dialog->popup_centered_minsize();
                return;
            }
            UndoRedo* ur = EditorNode::get_singleton()->get_undo_redo();
            ur->create_action(TTR("Set Rest Pose to Bones"));
            for (int i = 0; i < node->get_bone_count(); i++) {
                Bone2D* bone = node->get_bone(i);
                ur->add_do_method(bone, "set_transform", bone->get_rest());
                ur->add_undo_method(
                    bone,
                    "set_transform",
                    bone->get_transform()
                );
            }
            ur->commit_action();

        } break;
        case MENU_OPTION_MAKE_REST: {
            if (node->get_bone_count() == 0) {
                err_dialog->set_text(
                    TTR("This skeleton has no bones, create some children "
                        "Bone2D nodes.")
                );
                err_dialog->popup_centered_minsize();
                return;
            }
            UndoRedo* ur = EditorNode::get_singleton()->get_undo_redo();
            ur->create_action(TTR("Create Rest Pose from Bones"));
            for (int i = 0; i < node->get_bone_count(); i++) {
                Bone2D* bone = node->get_bone(i);
                ur->add_do_method(bone, "set_rest", bone->get_transform());
                ur->add_undo_method(bone, "set_rest", bone->get_rest());
            }
            ur->commit_action();

        } break;
    }
}

void Skeleton2DEditor::_bind_methods() {
    ClassDB::bind_method("_menu_option", &Skeleton2DEditor::_menu_option);
}

Skeleton2DEditor::Skeleton2DEditor() {
    options = memnew(MenuButton);

    CanvasItemEditor::get_singleton()->add_control_to_menu_panel(options);

    options->set_text(TTR("Skeleton2D"));
    options->set_icon(EditorNode::get_singleton()->get_gui_base()->get_icon(
        "Skeleton2D",
        "EditorIcons"
    ));

    options->get_popup()->add_item(
        TTR("Reset to Rest Pose"),
        MENU_OPTION_SET_REST
    );
    options->get_popup()->add_separator();
    // Use the "Overwrite" word to highlight that this is a destructive
    // operation.
    options->get_popup()->add_item(
        TTR("Overwrite Rest Pose"),
        MENU_OPTION_MAKE_REST
    );
    options->set_switch_on_hover(true);

    options->get_popup()->connect("id_pressed", this, "_menu_option");

    err_dialog = memnew(AcceptDialog);
    add_child(err_dialog);
}

void Skeleton2DEditorPlugin::edit(Object* p_object) {
    sprite_editor->edit(Object::cast_to<Skeleton2D>(p_object));
}

bool Skeleton2DEditorPlugin::handles(Object* p_object) const {
    return p_object->is_class("Skeleton2D");
}

void Skeleton2DEditorPlugin::make_visible(bool p_visible) {
    if (p_visible) {
        sprite_editor->options->show();
    } else {
        sprite_editor->options->hide();
        sprite_editor->edit(nullptr);
    }
}

Skeleton2DEditorPlugin::Skeleton2DEditorPlugin(EditorNode* p_node) {
    editor        = p_node;
    sprite_editor = memnew(Skeleton2DEditor);
    editor->get_viewport()->add_child(sprite_editor);
    make_visible(false);

    // sprite_editor->options->hide();
}

Skeleton2DEditorPlugin::~Skeleton2DEditorPlugin() {}
