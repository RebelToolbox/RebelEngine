// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CAMERA_EDITOR_PLUGIN_H
#define CAMERA_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/3d/camera.h"

class CameraEditor : public Control {
    GDCLASS(CameraEditor, Control);

    Panel* panel;
    Button* preview;
    Node* node;

    void _pressed();

protected:
    void _node_removed(Node* p_node);
    static void _bind_methods();

public:
    void edit(Node* p_camera);
    CameraEditor();
};

class CameraEditorPlugin : public EditorPlugin {
    GDCLASS(CameraEditorPlugin, EditorPlugin);

    // CameraEditor *camera_editor;
    EditorNode* editor;

public:
    virtual String get_name() const {
        return "Camera";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    CameraEditorPlugin(EditorNode* p_node);
    ~CameraEditorPlugin();
};

#endif // CAMERA_EDITOR_PLUGIN_H
