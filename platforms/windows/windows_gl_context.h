// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WINDOWS_GL_CONTEXT_H
#define WINDOWS_GL_CONTEXT_H

#if defined(OPENGL_ENABLED) || defined(GLES_ENABLED)

#include "core/error_list.h"
#include "core/os/os.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef bool(APIENTRY* PFNWGLSWAPINTERVALEXTPROC)(int interval);
typedef int(APIENTRY* PFNWGLGETSWAPINTERVALEXTPROC)(void);

class WindowsGLContext {
    HDC hDC;
    HGLRC hRC;
    unsigned int pixel_format;
    HWND hWnd;
    bool opengl_3_context;
    bool use_vsync;
    bool vsync_via_compositor;

    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
    PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

    static bool should_vsync_via_compositor();

public:
    void release_current();

    void make_current();

    HDC get_hdc();
    HGLRC get_hglrc();

    int get_window_width();
    int get_window_height();
    void swap_buffers();

    Error initialize();

    void set_use_vsync(bool p_use);
    bool is_using_vsync() const;

    WindowsGLContext(HWND hwnd, bool p_opengl_3_context);
    ~WindowsGLContext();
};

#endif

#endif // WINDOWS_GL_CONTEXT_H
