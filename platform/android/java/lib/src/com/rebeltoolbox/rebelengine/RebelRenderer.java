/*************************************************************************/
/*  RebelRenderer.java                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           REBEL ENGINE                                */
/*************************************************************************/
/* Copyright (c) 2022-Present Rebel Engine contributors                  */
/* Copyright (c) 2014-2022 Godot Engine contributors                     */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur                  */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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
