// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#if defined(TOOLS_ENABLED) && defined(WEB_ENABLED)
#include "web_tools_editor_plugin.h"

#include "core/engine.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/project_settings.h"
#include "editor/editor_node.h"

#include <emscripten/emscripten.h>

// JavaScript functions defined in library_rebel_editor_tools.js
extern "C" {
extern void rebel_js_os_download_buffer(
    const uint8_t* p_buf,
    int p_buf_size,
    const char* p_name,
    const char* p_mime
);
}

static void _web_editor_init_callback() {
    EditorNode::get_singleton()->add_editor_plugin(
        memnew(WebToolsEditorPlugin(EditorNode::get_singleton()))
    );
}

void WebToolsEditorPlugin::initialize() {
    EditorNode::add_init_callback(_web_editor_init_callback);
}

WebToolsEditorPlugin::WebToolsEditorPlugin(EditorNode* p_editor) {
    Variant v;
    add_tool_menu_item("Download Project Source", this, "_download_zip", v);
}

void WebToolsEditorPlugin::_download_zip(Variant p_v) {
    if (!Engine::get_singleton()
        || !Engine::get_singleton()->is_editor_hint()) {
        WARN_PRINT("Project download is only available in Editor mode");
        return;
    }
    String resource_path =
        ProjectSettings::get_singleton()->get_resource_path();

    FileAccess* src_f;
    zlib_filefunc_def io = zipio_create_io_from_file(&src_f);
    zipFile zip = zipOpen2("/tmp/project.zip", APPEND_STATUS_CREATE, NULL, &io);
    String base_path = resource_path.substr(0, resource_path.rfind("/")) + "/";
    _zip_recursive(resource_path, base_path, zip);
    zipClose(zip, NULL);
    FileAccess* f = FileAccess::open("/tmp/project.zip", FileAccess::READ);
    ERR_FAIL_COND_MSG(!f, "Unable to create zip file");
    Vector<uint8_t> buf;
    buf.resize(f->get_len());
    f->get_buffer(buf.ptrw(), buf.size());
    rebel_js_os_download_buffer(
        buf.ptr(),
        buf.size(),
        "project.zip",
        "application/zip"
    );
}

void WebToolsEditorPlugin::_bind_methods() {
    ClassDB::bind_method("_download_zip", &WebToolsEditorPlugin::_download_zip);
}

void WebToolsEditorPlugin::_zip_file(
    String p_path,
    String p_base_path,
    zipFile p_zip
) {
    FileAccess* f = FileAccess::open(p_path, FileAccess::READ);
    if (!f) {
        WARN_PRINT("Unable to open file for zipping: " + p_path);
        return;
    }
    Vector<uint8_t> data;
    uint64_t len = f->get_len();
    data.resize(len);
    f->get_buffer(data.ptrw(), len);
    f->close();
    memdelete(f);

    String path = p_path.replace_first(p_base_path, "");
    zipOpenNewFileInZip(
        p_zip,
        path.utf8().get_data(),
        NULL,
        NULL,
        0,
        NULL,
        0,
        NULL,
        Z_DEFLATED,
        Z_DEFAULT_COMPRESSION
    );
    zipWriteInFileInZip(p_zip, data.ptr(), data.size());
    zipCloseFileInZip(p_zip);
}

void WebToolsEditorPlugin::_zip_recursive(
    String p_path,
    String p_base_path,
    zipFile p_zip
) {
    DirAccess* dir = DirAccess::open(p_path);
    if (!dir) {
        WARN_PRINT("Unable to open dir for zipping: " + p_path);
        return;
    }
    dir->list_dir_begin();
    String cur = dir->get_next();
    String project_data_dir_name =
        ProjectSettings::get_singleton()->get_project_data_dir_name();
    while (!cur.empty()) {
        String cs = p_path.plus_file(cur);
        if (cur == "." || cur == ".." || cur == project_data_dir_name) {
            // Skip
        } else if (dir->current_is_dir()) {
            String path = cs.replace_first(p_base_path, "") + "/";
            zipOpenNewFileInZip(
                p_zip,
                path.utf8().get_data(),
                NULL,
                NULL,
                0,
                NULL,
                0,
                NULL,
                Z_DEFLATED,
                Z_DEFAULT_COMPRESSION
            );
            zipCloseFileInZip(p_zip);
            _zip_recursive(cs, p_base_path, p_zip);
        } else {
            _zip_file(cs, p_base_path, p_zip);
        }
        cur = dir->get_next();
    }
}
#endif
