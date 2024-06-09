// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LINUX_CONTEXT_GL_H
#define LINUX_CONTEXT_GL_H

#ifdef LINUX_ENABLED
#ifdef OPENGL_ENABLED

#include "core/os/os.h"

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

struct LinuxPrivateGLContext;

class LinuxGLContext {
public:
    enum ContextType {
        OLDSTYLE,
        GLES_2_0_COMPATIBLE,
        GLES_3_0_COMPATIBLE
    };

private:
    LinuxPrivateGLContext* p;
    OS::VideoMode default_video_mode;
    ::Display* x11_display;
    ::Window& x11_window;
    bool double_buffer;
    bool direct_render;
    int glx_minor, glx_major;
    bool use_vsync;
    ContextType context_type;

public:
    void release_current();
    void make_current();
    void swap_buffers();
    int get_window_width();
    int get_window_height();
    void* get_glx_context();

    Error initialize();

    void set_use_vsync(bool p_use);
    bool is_using_vsync() const;

    LinuxGLContext(
        ::Display* p_x11_display,
        ::Window& p_x11_window,
        const OS::VideoMode& p_default_video_mode,
        ContextType p_context_type
    );
    ~LinuxGLContext();
};

#endif // OPENGL_ENABLED
#endif // LINUX_ENABLED
#endif // LINUX_CONTEXT_GL_H
