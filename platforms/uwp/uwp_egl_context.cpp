// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "uwp_egl_context.h"

#include "EGL/eglext.h"

using Platform::Exception;

void UwpGLContext::release_current() {
    eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, mEglContext);
};

void UwpGLContext::make_current() {
    eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext);
};

int UwpGLContext::get_window_width() {
    return width;
};

int UwpGLContext::get_window_height() {
    return height;
};

void UwpGLContext::reset() {
    cleanup();

    window = CoreWindow::GetForCurrentThread();
    initialize();
};

void UwpGLContext::swap_buffers() {
    if (eglSwapBuffers(mEglDisplay, mEglSurface) != EGL_TRUE) {
        cleanup();

        window = CoreWindow::GetForCurrentThread();
        initialize();

        // tell rasterizer to reload textures and stuff?
    }
};

Error UwpGLContext::initialize() {
    EGLint configAttribList[] = {
        EGL_RED_SIZE,
        8,
        EGL_GREEN_SIZE,
        8,
        EGL_BLUE_SIZE,
        8,
        EGL_ALPHA_SIZE,
        8,
        EGL_DEPTH_SIZE,
        8,
        EGL_STENCIL_SIZE,
        8,
        EGL_SAMPLE_BUFFERS,
        0,
        EGL_NONE
    };

    EGLint surfaceAttribList[] = {EGL_NONE, EGL_NONE};

    EGLint numConfigs   = 0;
    EGLint majorVersion = 1;
    EGLint minorVersion;
    if (driver == GLES_2_0) {
        minorVersion = 0;
    } else {
        minorVersion = 5;
    }
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLContext context = EGL_NO_CONTEXT;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLConfig config   = nullptr;
    EGLint contextAttribs[3];
    if (driver == GLES_2_0) {
        contextAttribs[0] = EGL_CONTEXT_CLIENT_VERSION;
        contextAttribs[1] = 2;
        contextAttribs[2] = EGL_NONE;
    } else {
        contextAttribs[0] = EGL_CONTEXT_CLIENT_VERSION;
        contextAttribs[1] = 3;
        contextAttribs[2] = EGL_NONE;
    }

    try {
        const EGLint displayAttributes[] = {
            /*EGL_PLATFORM_ANGLE_TYPE_ANGLE,
            EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 9,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 3,
            EGL_NONE,*/
            // These are the default display attributes, used to request ANGLE's
            // D3D11 renderer. eglInitialize will only succeed with these
            // attributes if the hardware supports D3D11 Feature Level 10_0+.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE,
            EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,

            // EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER is an optimization
            // that can have large performance benefits on mobile devices. Its
            // syntax is subject to change, though. Please update your Visual
            // Studio templates if you experience compilation issues with it.
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER,
            EGL_TRUE,

            // EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE is an option that
            // enables ANGLE to automatically call the IDXGIDevice3::Trim method
            // on behalf of the application when it gets suspended. Calling
            // IDXGIDevice3::Trim when an application is suspended is a Windows
            // Store application certification requirement.
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE,
            EGL_TRUE,
            EGL_NONE,
        };

        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
            reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
                eglGetProcAddress("eglGetPlatformDisplayEXT")
            );

        if (!eglGetPlatformDisplayEXT) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to get function eglGetPlatformDisplayEXT"
            );
        }

        display = eglGetPlatformDisplayEXT(
            EGL_PLATFORM_ANGLE_ANGLE,
            EGL_DEFAULT_DISPLAY,
            displayAttributes
        );

        if (display == EGL_NO_DISPLAY) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to get default EGL display"
            );
        }

        if (eglInitialize(display, &majorVersion, &minorVersion) == EGL_FALSE) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to initialize EGL"
            );
        }

        if (eglGetConfigs(display, NULL, 0, &numConfigs) == EGL_FALSE) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to get EGLConfig count"
            );
        }

        if (eglChooseConfig(display, configAttribList, &config, 1, &numConfigs)
            == EGL_FALSE) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to choose first EGLConfig count"
            );
        }

        surface = eglCreateWindowSurface(
            display,
            config,
            reinterpret_cast<IInspectable*>(window),
            surfaceAttribList
        );
        if (surface == EGL_NO_SURFACE) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to create EGL fullscreen surface"
            );
        }

        context =
            eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        if (context == EGL_NO_CONTEXT) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to create EGL context"
            );
        }

        if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
            throw Exception::CreateException(
                E_FAIL,
                L"Failed to make fullscreen EGLSurface current"
            );
        }
    } catch (...) {
        return FAILED;
    };

    mEglDisplay = display;
    mEglSurface = surface;
    mEglContext = context;

    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    return OK;
};

void UwpGLContext::cleanup() {
    if (mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(mEglDisplay, mEglSurface);
        mEglSurface = EGL_NO_SURFACE;
    }

    if (mEglDisplay != EGL_NO_DISPLAY && mEglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(mEglDisplay, mEglContext);
        mEglContext = EGL_NO_CONTEXT;
    }

    if (mEglDisplay != EGL_NO_DISPLAY) {
        eglTerminate(mEglDisplay);
        mEglDisplay = EGL_NO_DISPLAY;
    }
};

UwpGLContext::UwpGLContext(CoreWindow ^ p_window, Driver p_driver) :
    mEglDisplay(EGL_NO_DISPLAY),
    mEglContext(EGL_NO_CONTEXT),
    mEglSurface(EGL_NO_SURFACE),
    driver(p_driver),
    window(p_window) {}

UwpGLContext::~UwpGLContext() {
    cleanup();
};
