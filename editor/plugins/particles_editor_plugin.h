// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PARTICLES_EDITOR_PLUGIN_H
#define PARTICLES_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/3d/particles.h"
#include "scene/gui/spin_box.h"

class ParticlesEditorBase : public Control {
    GDCLASS(ParticlesEditorBase, Control);

protected:
    Spatial* base_node;
    Panel* panel;
    MenuButton* options;
    HBoxContainer* particles_editor_hb;

    EditorFileDialog* emission_file_dialog;
    SceneTreeDialog* emission_tree_dialog;

    ConfirmationDialog* emission_dialog;
    SpinBox* emission_amount;
    OptionButton* emission_fill;

    PoolVector<Face3> geometry;

    bool _generate(PoolVector<Vector3>& points, PoolVector<Vector3>& normals);
    virtual void _generate_emission_points() = 0;
    void _node_selected(const NodePath& p_path);

    static void _bind_methods();

public:
    ParticlesEditorBase();
};

class ParticlesEditor : public ParticlesEditorBase {
    GDCLASS(ParticlesEditor, ParticlesEditorBase);

    ConfirmationDialog* generate_aabb;
    SpinBox* generate_seconds;
    Particles* node;

    enum Menu {
        MENU_OPTION_GENERATE_AABB,
        MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_NODE,
        MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_MESH,
        MENU_OPTION_CLEAR_EMISSION_VOLUME,
        MENU_OPTION_CONVERT_TO_CPU_PARTICLES,
        MENU_OPTION_RESTART,
    };

    void _generate_aabb();

    void _menu_option(int);

    friend class ParticlesEditorPlugin;

    virtual void _generate_emission_points();

protected:
    void _notification(int p_notification);
    void _node_removed(Node* p_node);
    static void _bind_methods();

public:
    void edit(Particles* p_particles);
    ParticlesEditor();
};

class ParticlesEditorPlugin : public EditorPlugin {
    GDCLASS(ParticlesEditorPlugin, EditorPlugin);

    ParticlesEditor* particles_editor;
    EditorNode* editor;

public:
    virtual String get_name() const {
        return "Particles";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    ParticlesEditorPlugin(EditorNode* p_node);
    ~ParticlesEditorPlugin();
};

#endif // PARTICLES_EDITOR_PLUGIN_H
