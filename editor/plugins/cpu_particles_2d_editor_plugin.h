// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CPU_PARTICLES_2D_EDITOR_PLUGIN_H
#define CPU_PARTICLES_2D_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/2d/collision_polygon_2d.h"
#include "scene/2d/cpu_particles_2d.h"
#include "scene/gui/box_container.h"
#include "scene/gui/file_dialog.h"

class CPUParticles2DEditorPlugin : public EditorPlugin {
    GDCLASS(CPUParticles2DEditorPlugin, EditorPlugin);

    enum {
        MENU_LOAD_EMISSION_MASK,
        MENU_CLEAR_EMISSION_MASK,
        MENU_RESTART
    };

    enum EmissionMode {
        EMISSION_MODE_SOLID,
        EMISSION_MODE_BORDER,
        EMISSION_MODE_BORDER_DIRECTED
    };

    CPUParticles2D* particles;

    EditorFileDialog* file;
    EditorNode* editor;

    HBoxContainer* toolbar;
    MenuButton* menu;

    SpinBox* epoints;

    ConfirmationDialog* emission_mask;
    OptionButton* emission_mask_mode;
    CheckBox* emission_colors;

    String source_emission_file;

    UndoRedo* undo_redo;
    void _file_selected(const String& p_file);
    void _menu_callback(int p_idx);
    void _generate_emission_mask();

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    virtual String get_name() const {
        return "CPUParticles2D";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    CPUParticles2DEditorPlugin(EditorNode* p_node);
    ~CPUParticles2DEditorPlugin();
};

#endif // CPU_PARTICLES_2D_EDITOR_PLUGIN_H
