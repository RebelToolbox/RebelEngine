// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.xr.ovr;

import android.opengl.EGL14;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

/**
 * EGL Context factory for the Oculus mobile VR SDK.
 */
public class OvrContextFactory implements GLSurfaceView.EGLContextFactory {
    private static final int[] CONTEXT_ATTRIBS = {
        EGL14.EGL_CONTEXT_CLIENT_VERSION,
        3,
        EGL10.EGL_NONE
    };

    @Override
    public EGLContext createContext(
        EGL10 egl,
        EGLDisplay display,
        EGLConfig eglConfig
    ) {
        return egl.eglCreateContext(
            display,
            eglConfig,
            EGL10.EGL_NO_CONTEXT,
            CONTEXT_ATTRIBS
        );
    }

    @Override
    public void destroyContext(
        EGL10 egl,
        EGLDisplay display,
        EGLContext context
    ) {
        egl.eglDestroyContext(display, context);
    }
}
