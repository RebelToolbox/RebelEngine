// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine;

import com.rebeltoolbox.rebelengine.plugin.RebelPlugin;
import com.rebeltoolbox.rebelengine.plugin.RebelPluginRegistry;
import com.rebeltoolbox.rebelengine.utils.GLUtils;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class RebelRenderer implements GLSurfaceView.Renderer {
    private final RebelPluginRegistry rebelPluginRegistry;
    private boolean activityJustResumed = false;

    RebelRenderer() {
        this.rebelPluginRegistry = RebelPluginRegistry.getPluginRegistry();
    }

    public void onDrawFrame(GL10 gl) {
        if (activityJustResumed) {
            RebelEngine.onRendererResumed();
            activityJustResumed = false;
        }

        RebelEngine.step();
        for (int i = 0; i < RebelFragment.singleton_count; i++) {
            RebelFragment.singletons[i].onGLDrawFrame(gl);
        }
        for (RebelPlugin rebelPlugin :
             rebelPluginRegistry.getAllRebelPlugins()) {
            rebelPlugin.onGLDrawFrame(gl);
        }
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        RebelEngine.resize(width, height);
        for (int i = 0; i < RebelFragment.singleton_count; i++) {
            RebelFragment.singletons[i].onGLSurfaceChanged(gl, width, height);
        }
        for (RebelPlugin rebelPlugin :
             rebelPluginRegistry.getAllRebelPlugins()) {
            rebelPlugin.onGLSurfaceChanged(gl, width, height);
        }
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        RebelEngine.newcontext(GLUtils.use_32);
        for (RebelPlugin rebelPlugin :
             rebelPluginRegistry.getAllRebelPlugins()) {
            rebelPlugin.onGLSurfaceCreated(gl, config);
        }
    }

    void onActivityResumed() {
        // We defer invoking RebelEngine.onRendererResumed() until the first
        // draw frame call. This ensures we have a valid GL context and surface
        // when we do so.
        activityJustResumed = true;
    }

    void onActivityPaused() {
        RebelEngine.onRendererPaused();
    }
}
