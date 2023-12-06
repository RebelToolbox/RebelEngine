// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDSCRIPT_LANGUAGE_SERVER_H
#define GDSCRIPT_LANGUAGE_SERVER_H

#include "../gdscript_parser.h"
#include "editor/editor_plugin.h"
#include "gdscript_language_protocol.h"

class GDScriptLanguageServer : public EditorPlugin {
    GDCLASS(GDScriptLanguageServer, EditorPlugin);

    GDScriptLanguageProtocol protocol;

    Thread thread;
    bool thread_running;
    bool started;
    bool use_thread;
    String host;
    int port;
    static void thread_main(void* p_userdata);

private:
    void _notification(int p_what);
    void _iteration();

public:
    Error parse_script_file(const String& p_path);
    GDScriptLanguageServer();
    void start();
    void stop();
};

void register_lsp_types();

#endif // GDSCRIPT_LANGUAGE_SERVER_H
