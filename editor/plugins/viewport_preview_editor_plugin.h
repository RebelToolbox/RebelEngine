// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VIEWPORT_PREVIEW_EDITOR_PLUGIN_H
#define VIEWPORT_PREVIEW_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/plugins/texture_editor_plugin.h"
#include "scene/main/viewport.h"

class EditorInspectorPluginViewportPreview :
    public EditorInspectorPluginTexture {
    GDCLASS(EditorInspectorPluginViewportPreview, EditorInspectorPluginTexture);

public:
    virtual bool can_handle(Object* p_object);
    virtual void parse_begin(Object* p_object);
};

class ViewportPreviewEditorPlugin : public EditorPlugin {
    GDCLASS(ViewportPreviewEditorPlugin, EditorPlugin);

public:
    virtual String get_name() const {
        return "SubViewportPreview";
    }

    ViewportPreviewEditorPlugin(EditorNode* p_node);
};

#endif // VIEWPORT_PREVIEW_EDITOR_PLUGIN_H
