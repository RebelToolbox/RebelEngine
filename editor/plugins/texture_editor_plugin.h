// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TEXTURE_EDITOR_PLUGIN_H
#define TEXTURE_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/resources/texture.h"

class TexturePreview : public MarginContainer {
    GDCLASS(TexturePreview, MarginContainer);

private:
    TextureRect* texture_display = nullptr;

    TextureRect* checkerboard = nullptr;
    Label* metadata_label     = nullptr;

    void _update_metadata_label_text();

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    TextureRect* get_texture_display();
    TexturePreview(Ref<Texture> p_texture, bool p_show_metadata);
};

class EditorInspectorPluginTexture : public EditorInspectorPlugin {
    GDCLASS(EditorInspectorPluginTexture, EditorInspectorPlugin);

public:
    virtual bool can_handle(Object* p_object);
    virtual void parse_begin(Object* p_object);
};

class TextureEditorPlugin : public EditorPlugin {
    GDCLASS(TextureEditorPlugin, EditorPlugin);

public:
    virtual String get_name() const {
        return "Texture";
    }

    TextureEditorPlugin(EditorNode* p_node);
};

#endif // TEXTURE_EDITOR_PLUGIN_H
