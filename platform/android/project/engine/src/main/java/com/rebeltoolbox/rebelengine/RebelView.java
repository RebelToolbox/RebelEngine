// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine;

import com.rebeltoolbox.rebelengine.input.RebelGestureListener;
import com.rebeltoolbox.rebelengine.input.RebelInputHandler;
import com.rebeltoolbox.rebelengine.utils.GLUtils;
import com.rebeltoolbox.rebelengine.xr.XRMode;
import com.rebeltoolbox.rebelengine.xr.ovr.OvrConfigChooser;
import com.rebeltoolbox.rebelengine.xr.ovr.OvrContextFactory;
import com.rebeltoolbox.rebelengine.xr.ovr.OvrWindowSurfaceFactory;
import com.rebeltoolbox.rebelengine.xr.regular.RegularConfigChooser;
import com.rebeltoolbox.rebelengine.xr.regular.RegularContextFactory;
import com.rebeltoolbox.rebelengine.xr.regular.RegularFallbackConfigChooser;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MotionEvent;

/**
 * A simple GLSurfaceView sub-class that demonstrate how to perform
 * OpenGL ES 2.0 rendering into a GL Surface. Note the following important
 * details:
 *
 * - The class must use a custom context factory to enable 2.0 rendering.
 *   See ContextFactory class definition below.
 *
 * - The class must use a custom EGLConfigChooser to be able to select
 *   an EGLConfig that supports 2.0. This is done by providing a config
 *   specification to eglChooseConfig() that has the attribute
 *   EGL10.ELG_RENDERABLE_TYPE containing the EGL_OPENGL_ES2_BIT flag
 *   set. See ConfigChooser class definition below.
 *
 * - The class must select the surface's format, then choose an EGLConfig
 *   that matches it exactly (with regards to red/green/blue/alpha channels
 *   bit depths). Failure to do so would result in an EGL_BAD_MATCH error.
 */
public class RebelView extends GLSurfaceView {
    private static String TAG = RebelView.class.getSimpleName();

    private final RebelFragment rebelFragment;
    private final RebelInputHandler rebelInputHandler;
    private final GestureDetector gestureDetector;
    private final RebelRenderer rebelRenderer;

    public RebelView(
        Context context,
        RebelFragment rebelFragment,
        XRMode xrMode,
        boolean p_use_gl3,
        boolean p_use_32_bits,
        boolean p_use_debug_opengl,
        boolean p_translucent
    ) {
        super(context);
        GLUtils.use_gl3          = p_use_gl3;
        GLUtils.use_32           = p_use_32_bits;
        GLUtils.use_debug_opengl = p_use_debug_opengl;

        this.rebelFragment     = rebelFragment;
        this.rebelInputHandler = new RebelInputHandler(this);
        this.gestureDetector =
            new GestureDetector(context, new RebelGestureListener(this));
        this.rebelRenderer = new RebelRenderer();

        init(xrMode, p_translucent, 16, 0);
    }

    public void initInputDevices() {
        rebelInputHandler.initInputDevices();
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        super.onTouchEvent(event);
        gestureDetector.onTouchEvent(event);
        return rebelInputHandler.onTouchEvent(event);
    }

    @Override
    public boolean onKeyUp(final int keyCode, KeyEvent event) {
        return rebelInputHandler.onKeyUp(keyCode, event)
     || super.onKeyUp(keyCode, event);
    }

    @Override
    public boolean onKeyDown(final int keyCode, KeyEvent event) {
        return rebelInputHandler.onKeyDown(keyCode, event)
     || super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        return rebelInputHandler.onGenericMotionEvent(event)
     || super.onGenericMotionEvent(event);
    }

    private void init(
        XRMode xrMode,
        boolean translucent,
        int depth,
        int stencil
    ) {
        setPreserveEGLContextOnPause(true);
        setFocusableInTouchMode(true);
        switch (xrMode) {
            case OVR:
            case OPENXR:
                // Replace the default egl config chooser.
                setEGLConfigChooser(new OvrConfigChooser());

                // Replace the default context factory.
                setEGLContextFactory(new OvrContextFactory());

                // Replace the default window surface factory.
                setEGLWindowSurfaceFactory(new OvrWindowSurfaceFactory());
                break;

            case REGULAR:
            default:
                /* By default, GLSurfaceView() creates a RGB_565 opaque surface.
                 * If we want a translucent one, we should change the surface's
                 * format here, using PixelFormat.TRANSLUCENT for GL Surfaces
                 * is interpreted as any 32-bit surface with alpha by
                 * SurfaceFlinger.
                 */
                if (translucent) {
                    this.setZOrderOnTop(true);
                    this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
                }

                /* Setup the context factory for 2.0 rendering.
                 * See ContextFactory class definition below
                 */
                setEGLContextFactory(new RegularContextFactory());

                /* We need to choose an EGLConfig that matches the format of
                 * our surface exactly. This is going to be done in our
                 * custom config chooser. See ConfigChooser class definition
                 * below.
                 */

                if (GLUtils.use_32) {
                    setEGLConfigChooser(
                        translucent ? new RegularFallbackConfigChooser(
                            8,
                            8,
                            8,
                            8,
                            24,
                            stencil,
                            new RegularConfigChooser(8, 8, 8, 8, 16, stencil)
                        )
                                    : new RegularFallbackConfigChooser(
                                        8,
                                        8,
                                        8,
                                        8,
                                        24,
                                        stencil,
                                        new RegularConfigChooser(
                                            5,
                                            6,
                                            5,
                                            0,
                                            16,
                                            stencil
                                        )
                                    )
                    );

                } else {
                    setEGLConfigChooser(
                        translucent
                            ? new RegularConfigChooser(8, 8, 8, 8, 16, stencil)
                            : new RegularConfigChooser(5, 6, 5, 0, 16, stencil)
                    );
                }
                break;
        }

        /* Set the renderer responsible for frame rendering */
        setRenderer(rebelRenderer);
    }

    public void onBackPressed() {
        rebelFragment.onBackPressed();
    }

    public RebelInputHandler getRebelInputHandler() {
        return rebelInputHandler;
    }

    @Override
    public void onResume() {
        super.onResume();

        queueEvent(() -> {
            // Resume the renderer
            rebelRenderer.onActivityResumed();
            RebelEngine.focusin();
        });
    }

    @Override
    public void onPause() {
        super.onPause();

        queueEvent(() -> {
            RebelEngine.focusout();
            // Pause the renderer
            rebelRenderer.onActivityPaused();
        });
    }
}
