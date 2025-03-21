// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WEB_TOOLS_EDITOR_PLUGIN_H
#define WEB_TOOLS_EDITOR_PLUGIN_H

#if defined(TOOLS_ENABLED) && defined(WEB_ENABLED)
#include "core/io/zip_io.h"
#include "editor/editor_plugin.h"

class WebToolsEditorPlugin : public EditorPlugin {
    GDCLASS(WebToolsEditorPlugin, EditorPlugin);

private:
    void _zip_file(String p_path, String p_base_path, zipFile p_zip);
    void _zip_recursive(String p_path, String p_base_path, zipFile p_zip);

protected:
    static void _bind_methods();

    void _download_zip(Variant p_v);

public:
    static void initialize();

    WebToolsEditorPlugin(EditorNode* p_editor);
};
#else
class WebToolsEditorPlugin {
public:
    static void initialize() {}
};
#endif

#endif // WEB_TOOLS_EDITOR_PLUGIN_H
