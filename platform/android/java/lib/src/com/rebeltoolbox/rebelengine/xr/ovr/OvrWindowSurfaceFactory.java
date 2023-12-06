// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.xr.ovr;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

/**
 * EGL window surface factory for the Oculus mobile VR SDK.
 */
public class OvrWindowSurfaceFactory
    implements GLSurfaceView.EGLWindowSurfaceFactory {
    private final static int[] SURFACE_ATTRIBS =
        {EGL10.EGL_WIDTH, 16, EGL10.EGL_HEIGHT, 16, EGL10.EGL_NONE};

    @Override
    public EGLSurface createWindowSurface(
        EGL10 egl,
        EGLDisplay display,
        EGLConfig config,
        Object nativeWindow
    ) {
        return egl.eglCreatePbufferSurface(display, config, SURFACE_ATTRIBS);
    }

    @Override
    public void destroySurface(
        EGL10 egl,
        EGLDisplay display,
        EGLSurface surface
    ) {
        egl.eglDestroySurface(display, surface);
    }
}
