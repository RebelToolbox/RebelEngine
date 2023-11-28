/*************************************************************************/
/*  RebelEngine.java                                                     */
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

import android.app.Activity;
import android.hardware.SensorEvent;

import javax.microedition.khronos.opengles.GL10;

/**
 * Wrapper for native library
 */
public class RebelEngine {
    public static RebelIO io;

    static {
        System.loadLibrary("godot_android");
    }

    /**
     * Invoked on the main thread to initialize Rebel Engine.
     */
    public static native void initialize(
        Activity activity,
        RebelFragment p_instance,
        Object p_asset_manager,
        boolean use_apk_expansion
    );

    /**
     * Invoked on the main thread to terminate the Rebel Engine.
     * @see androidx.fragment.app.Fragment#onDestroy()
     */
    public static native void ondestroy();

    /**
     * Invoked on the GL thread to setup Rebel Engine.
     * @param p_cmdline Command line arguments used to configure Rebel Engine.
     */
    public static native void setup(String[] p_cmdline);

    /**
     * Invoked on the GL thread when the underlying Android surface has changed
     * size.
     * @param width
     * @param height
     * @see android.opengl.GLSurfaceView.Renderer#onSurfaceChanged(GL10, int,
     *     int)
     */
    public static native void resize(int width, int height);

    /**
     * Invoked on the GL thread when the underlying Android surface is created
     * or recreated.
     * @param p_32_bits
     * @see android.opengl.GLSurfaceView.Renderer#onSurfaceCreated(GL10,
     *     EGLConfig)
     */
    public static native void newcontext(boolean p_32_bits);

    /**
     * Forward {@link Activity#onBackPressed()} event from the main thread to
     * the GL thread.
     */
    public static native void back();

    /**
     * Invoked on the GL thread to draw the current frame.
     * @see android.opengl.GLSurfaceView.Renderer#onDrawFrame(GL10)
     */
    public static native void step();

    /**
     * Forward touch events from the main thread to the GL thread.
     */
    public static native void touch(
        int inputDevice,
        int event,
        int pointer,
        int pointerCount,
        float[] positions
    );
    public static native void touch(
        int inputDevice,
        int event,
        int pointer,
        int pointerCount,
        float[] positions,
        int buttonsMask
    );
    public static native void touch(
        int inputDevice,
        int event,
        int pointer,
        int pointerCount,
        float[] positions,
        int buttonsMask,
        float verticalFactor,
        float horizontalFactor
    );

    /**
     * Forward hover events from the main thread to the GL thread.
     */
    public static native void hover(int type, float x, float y);

    /**
     * Forward double_tap events from the main thread to the GL thread.
     */
    public static native void doubleTap(int buttonMask, int x, int y);

    /**
     * Forward scroll events from the main thread to the GL thread.
     */
    public static native void scroll(int x, int y);

    /**
     * Forward accelerometer sensor events from the main thread to the GL
     * thread.
     * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
     */
    public static native void accelerometer(float x, float y, float z);

    /**
     * Forward gravity sensor events from the main thread to the GL thread.
     * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
     */
    public static native void gravity(float x, float y, float z);

    /**
     * Forward magnetometer sensor events from the main thread to the GL thread.
     * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
     */
    public static native void magnetometer(float x, float y, float z);

    /**
     * Forward gyroscope sensor events from the main thread to the GL thread.
     * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
     */
    public static native void gyroscope(float x, float y, float z);

    /**
     * Forward regular key events from the main thread to the GL thread.
     */
    public static native void key(
        int p_keycode,
        int p_scancode,
        int p_unicode_char,
        boolean p_pressed
    );

    /**
     * Forward game device's key events from the main thread to the GL thread.
     */
    public static native void joybutton(
        int p_device,
        int p_but,
        boolean p_pressed
    );

    /**
     * Forward joystick devices axis motion events from the main thread to the
     * GL thread.
     */
    public static native void joyaxis(int p_device, int p_axis, float p_value);

    /**
     * Forward joystick devices hat motion events from the main thread to the GL
     * thread.
     */
    public static native void joyhat(int p_device, int p_hat_x, int p_hat_y);

    /**
     * Fires when a joystick device is added or removed.
     */
    public static native void joyconnectionchanged(
        int p_device,
        boolean p_connected,
        String p_name
    );

    /**
     * Invoked when the Android app resumes.
     * @see androidx.fragment.app.Fragment#onResume()
     */
    public static native void focusin();

    /**
     * Invoked when the Android app pauses.
     * @see androidx.fragment.app.Fragment#onPause()
     */
    public static native void focusout();

    /**
     * Used to access global properties.
     * @param p_key Property key
     * @return String value of the property
     */
    public static native String getGlobal(String p_key);

    /**
     * Invoke method |p_method| on the Rebel object specified by |p_id|
     * @param p_id Id of the Rebel object to invoke the method on
     * @param p_method Name of the method to invoke
     * @param p_params Parameters to use for method invocation
     */
    public static native void callobject(
        long p_id,
        String p_method,
        Object[] p_params
    );

    /**
     * Invoke method |p_method| on the Rebel object specified by |p_id| during
     * idle time.
     * @param p_id Id of the Rebel object to invoke the method on
     * @param p_method Name of the method to invoke
     * @param p_params Parameters to use for method invocation
     */
    public static native void calldeferred(
        long p_id,
        String p_method,
        Object[] p_params
    );

    /**
     * Forward the results from a permission request.
     * @see Activity#onRequestPermissionsResult(int, String[], int[])
     * @param p_permission Request permission
     * @param p_result True if the permission was granted, false otherwise
     */
    public static native void requestPermissionResult(
        String p_permission,
        boolean p_result
    );

    /**
     * Invoked on the GL thread to configure the height of the virtual keyboard.
     */
    public static native void setVirtualKeyboardHeight(int p_height);

    /**
     * Invoked on the GL thread when the {@link RebelRenderer} has been resumed.
     * @see RebelRenderer#onActivityResumed()
     */
    public static native void onRendererResumed();

    /**
     * Invoked on the GL thread when the {@link RebelRenderer} has been paused.
     * @see RebelRenderer#onActivityPaused()
     */
    public static native void onRendererPaused();
}
