// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.xr.regular;

import com.rebeltoolbox.rebelengine.utils.GLUtils;

import android.util.Log;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

/* Fallback if 32bit View is not supported*/
public class RegularFallbackConfigChooser extends RegularConfigChooser {
    private static final String TAG =
        RegularFallbackConfigChooser.class.getSimpleName();

    private RegularConfigChooser fallback;

    public RegularFallbackConfigChooser(
        int r,
        int g,
        int b,
        int a,
        int depth,
        int stencil,
        RegularConfigChooser fallback
    ) {
        super(r, g, b, a, depth, stencil);
        this.fallback = fallback;
    }

    @Override
    public EGLConfig chooseConfig(
        EGL10 egl,
        EGLDisplay display,
        EGLConfig[] configs
    ) {
        EGLConfig ec = super.chooseConfig(egl, display, configs);
        if (ec == null) {
            Log.w(TAG, "Trying ConfigChooser fallback");
            ec             = fallback.chooseConfig(egl, display, configs);
            GLUtils.use_32 = false;
        }
        return ec;
    }
}
