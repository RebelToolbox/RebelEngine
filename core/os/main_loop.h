// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "core/os/input_event.h"
#include "core/reference.h"
#include "core/script_language.h"

class MainLoop : public Object {
    GDCLASS(MainLoop, Object);
    OBJ_CATEGORY("Main Loop");

    Ref<Script> init_script;

protected:
    static void _bind_methods();

public:
    enum {
        // make sure these are replicated in Node
        NOTIFICATION_WM_MOUSE_ENTER      = 1002,
        NOTIFICATION_WM_MOUSE_EXIT       = 1003,
        NOTIFICATION_WM_FOCUS_IN         = 1004,
        NOTIFICATION_WM_FOCUS_OUT        = 1005,
        NOTIFICATION_WM_QUIT_REQUEST     = 1006,
        NOTIFICATION_WM_GO_BACK_REQUEST  = 1007,
        NOTIFICATION_WM_UNFOCUS_REQUEST  = 1008,
        NOTIFICATION_OS_MEMORY_WARNING   = 1009,
        NOTIFICATION_TRANSLATION_CHANGED = 1010,
        NOTIFICATION_WM_ABOUT            = 1011,
        NOTIFICATION_CRASH               = 1012,
        NOTIFICATION_OS_IME_UPDATE       = 1013,
        NOTIFICATION_APP_RESUMED         = 1014,
        NOTIFICATION_APP_PAUSED          = 1015,
    };

    virtual void input_event(const Ref<InputEvent>& p_event);
    virtual void input_text(const String& p_text);

    virtual void init();
    virtual bool iteration(float p_time);
    virtual bool idle(float p_time);
    virtual void finish();

    virtual void drop_files(
        const Vector<String>& p_files,
        int p_from_screen = 0
    );
    virtual void global_menu_action(const Variant& p_id, const Variant& p_meta);

    void set_init_script(const Ref<Script>& p_init_script);

    MainLoop();
    virtual ~MainLoop();
};

#endif
