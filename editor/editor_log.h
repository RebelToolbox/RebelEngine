// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_LOG_H
#define EDITOR_LOG_H

#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/texture_button.h"
// #include "scene/gui/empty_control.h"
#include "core/os/thread.h"
#include "pane_drag.h"
#include "scene/gui/box_container.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tool_button.h"

class EditorLog : public VBoxContainer {
    GDCLASS(EditorLog, VBoxContainer);

    Button* clearbutton;
    Button* copybutton;
    Label* title;
    RichTextLabel* log;
    HBoxContainer* title_hb;
    // PaneDrag *pd;
    ToolButton* tool_button;

    static void _error_handler(
        void* p_self,
        const char* p_func,
        const char* p_file,
        int p_line,
        const char* p_error,
        const char* p_errorexp,
        ErrorHandlerType p_type
    );

    ErrorHandlerList eh;

    Thread::ID current;

    // void _dragged(const Point2& p_ofs);
    void _clear_request();
    void _copy_request();
    static void _undo_redo_cbk(void* p_self, const String& p_name);

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    enum MessageType {
        MSG_TYPE_STD,
        MSG_TYPE_ERROR,
        MSG_TYPE_WARNING,
        MSG_TYPE_EDITOR
    };

    void add_message(const String& p_msg, MessageType p_type = MSG_TYPE_STD);
    void set_tool_button(ToolButton* p_tool_button);
    void deinit();

    void clear();
    void copy();
    EditorLog();
    ~EditorLog();
};

#endif // EDITOR_LOG_H
