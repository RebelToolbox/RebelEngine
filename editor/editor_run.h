// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_RUN_H
#define EDITOR_RUN_H

#include "core/os/os.h"
#include "scene/main/node.h"

class EditorRun {
public:
    enum Status {
        STATUS_PLAY,
        STATUS_PAUSED,
        STATUS_STOP
    };

    OS::ProcessID pid;

private:
    bool debug_collisions;
    bool debug_navigation;
    Status status;
    String running_scene;

public:
    Status get_status() const;
    String get_running_scene() const;
    Error run(
        const String& p_scene,
        const String& p_custom_args,
        const List<String>& p_breakpoints,
        const bool& p_skip_breakpoints = false
    );

    void run_native_notify() {
        status = STATUS_PLAY;
    }

    void stop();

    OS::ProcessID get_pid() const {
        return pid;
    }

    void set_debug_collisions(bool p_debug);
    bool get_debug_collisions() const;

    void set_debug_navigation(bool p_debug);
    bool get_debug_navigation() const;

    EditorRun();
};

#endif // EDITOR_RUN_H
