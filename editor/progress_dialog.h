// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/popup.h"
#include "scene/gui/progress_bar.h"

class BackgroundProgress : public HBoxContainer {
    GDCLASS(BackgroundProgress, HBoxContainer);

    _THREAD_SAFE_CLASS_

    struct Task {
        HBoxContainer* hb;
        ProgressBar* progress;
    };

    Map<String, Task> tasks;
    Map<String, int> updates;
    void _update();

protected:
    void _add_task(const String& p_task, const String& p_label, int p_steps);
    void _task_step(const String& p_task, int p_step = -1);
    void _end_task(const String& p_task);

    static void _bind_methods();

public:
    void add_task(const String& p_task, const String& p_label, int p_steps);
    void task_step(const String& p_task, int p_step = -1);
    void end_task(const String& p_task);

    BackgroundProgress() {}
};

class ProgressDialog : public Popup {
    GDCLASS(ProgressDialog, Popup);

    struct Task {
        String task;
        VBoxContainer* vb;
        ProgressBar* progress;
        Label* state;
        uint64_t last_progress_tick;
    };

    HBoxContainer* cancel_hb;
    Button* cancel;

    Map<String, Task> tasks;
    VBoxContainer* main;

    static ProgressDialog* singleton;
    void _popup();

    void _cancel_pressed();
    bool cancelled;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    static ProgressDialog* get_singleton() {
        return singleton;
    }

    void add_task(
        const String& p_task,
        const String& p_label,
        int p_steps,
        bool p_can_cancel = false
    );
    bool task_step(
        const String& p_task,
        const String& p_state,
        int p_step          = -1,
        bool p_force_redraw = true
    );
    void end_task(const String& p_task);

    ProgressDialog();
};

#endif // PROGRESS_DIALOG_H
