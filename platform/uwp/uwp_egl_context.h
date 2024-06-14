// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef UWP_EGL_CONTEXT_H
#define UWP_EGL_CONTEXT_H

#include "core/error_list.h"
#include "core/os/os.h"

#include <EGL/egl.h>
#include <wrl.h>

using namespace Windows::UI::Core;

class UwpGLContext {
public:
    enum Driver {
        GLES_2_0,
        GLES_3_0,
    };

private:
    CoreWindow ^ window;

    EGLDisplay mEglDisplay;
    EGLContext mEglContext;
    EGLSurface mEglSurface;

    EGLint width;
    EGLint height;

    bool vsync;

    Driver driver;

public:
    void release_current();

    void make_current();

    int get_window_width();
    int get_window_height();
    void swap_buffers();

    void set_use_vsync(bool use) {
        vsync = use;
    }

    bool is_using_vsync() const {
        return vsync;
    }

    Error initialize();
    void reset();

    void cleanup();

    UwpGLContext(CoreWindow ^ p_window, Driver p_driver);
    ~UwpGLContext();
};

#endif // UWP_EGL_CONTEXT_H
