// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.xr.regular;

import com.rebeltoolbox.rebelengine.RebelEngine;
import com.rebeltoolbox.rebelengine.utils.GLUtils;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

/**
 * Factory used to setup the opengl context for pancake games.
 */
public class RegularContextFactory implements GLSurfaceView.EGLContextFactory {
    private static final String TAG =
        RegularContextFactory.class.getSimpleName();

    private static final int _EGL_CONTEXT_FLAGS_KHR            = 0x30FC;
    private static final int _EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR = 0x00000001;

    private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

    public EGLContext createContext(
        EGL10 egl,
        EGLDisplay display,
        EGLConfig eglConfig
    ) {
        String driver_name =
            RebelEngine.getGlobal("rendering/quality/driver/driver_name");
        if (GLUtils.use_gl3 && !driver_name.equals("GLES3")) {
            GLUtils.use_gl3 = false;
        }
        if (GLUtils.use_gl3)
            Log.w(TAG, "creating OpenGL ES 3.0 context :");
        else
            Log.w(TAG, "creating OpenGL ES 2.0 context :");

        GLUtils.checkEglError(TAG, "Before eglCreateContext", egl);
        EGLContext context;
        if (GLUtils.use_debug_opengl) {
            int[] attrib_list2 = {
                EGL_CONTEXT_CLIENT_VERSION,
                2,
                _EGL_CONTEXT_FLAGS_KHR,
                _EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
                EGL10.EGL_NONE
            };
            int[] attrib_list3 = {
                EGL_CONTEXT_CLIENT_VERSION,
                3,
                _EGL_CONTEXT_FLAGS_KHR,
                _EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
                EGL10.EGL_NONE
            };
            context = egl.eglCreateContext(
                display,
                eglConfig,
                EGL10.EGL_NO_CONTEXT,
                GLUtils.use_gl3 ? attrib_list3 : attrib_list2
            );
        } else {
            int[] attrib_list2 = {
                EGL_CONTEXT_CLIENT_VERSION,
                2,
                EGL10.EGL_NONE
            };
            int[] attrib_list3 = {
                EGL_CONTEXT_CLIENT_VERSION,
                3,
                EGL10.EGL_NONE
            };
            context = egl.eglCreateContext(
                display,
                eglConfig,
                EGL10.EGL_NO_CONTEXT,
                GLUtils.use_gl3 ? attrib_list3 : attrib_list2
            );
        }
        GLUtils.checkEglError(TAG, "After eglCreateContext", egl);
        return context;
    }

    public void destroyContext(
        EGL10 egl,
        EGLDisplay display,
        EGLContext context
    ) {
        egl.eglDestroyContext(display, context);
    }
}
