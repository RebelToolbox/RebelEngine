// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "viewport_preview_editor_plugin.h"

bool EditorInspectorPluginViewportPreview::can_handle(Object* p_object) {
    return Object::cast_to<Viewport>(p_object) != nullptr;
}

void EditorInspectorPluginViewportPreview::parse_begin(Object* p_object) {
    Viewport* viewport = Object::cast_to<Viewport>(p_object);

    TexturePreview* viewport_preview =
        memnew(TexturePreview(viewport->get_texture(), false));
    // Otherwise `viewport_preview`'s `texture_display` doesn't update properly
    // when `viewport`'s size changes.
    viewport->connect(
        "size_changed",
        viewport_preview->get_texture_display(),
        "update"
    );
    add_custom_control(viewport_preview);
}

ViewportPreviewEditorPlugin::ViewportPreviewEditorPlugin(EditorNode* p_node) {
    Ref<EditorInspectorPluginViewportPreview> plugin;
    plugin.instance();
    add_inspector_plugin(plugin);
}
