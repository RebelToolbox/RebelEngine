// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_JNI_H
#define ANDROID_JNI_H

#include <jni.h>

// The Rebel Engine Android Java Native Interface.
// For the Android declrations of these methods, see
// java/lib/src/com/rebeltoolbox/rebelengine/RebelEngine.java

extern "C" {
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_initialize(
    JNIEnv* env,
    jclass clazz,
    jobject activity,
    jobject fragment,
    jobject p_asset_manager,
    jboolean p_use_apk_expansion
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_ondestroy(
    JNIEnv* env,
    jclass clazz
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_setup(
    JNIEnv* env,
    jclass clazz,
    jobjectArray p_cmdline
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_resize(
    JNIEnv* env,
    jclass clazz,
    jint width,
    jint height
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_newcontext(
    JNIEnv* env,
    jclass clazz,
    jboolean p_32_bits
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_step(JNIEnv* env, jclass clazz);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_back(JNIEnv* env, jclass clazz);
void touch_preprocessing(
    JNIEnv* env,
    jclass clazz,
    jint input_device,
    jint ev,
    jint pointer,
    jint pointer_count,
    jfloatArray positions,
    jint buttons_mask        = 0,
    jfloat vertical_factor   = 0,
    jfloat horizontal_factor = 0
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_touch__IIII_3F(
    JNIEnv* env,
    jclass clazz,
    jint input_device,
    jint ev,
    jint pointer,
    jint pointer_count,
    jfloatArray positions
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_touch__IIII_3FI(
    JNIEnv* env,
    jclass clazz,
    jint input_device,
    jint ev,
    jint pointer,
    jint pointer_count,
    jfloatArray positions,
    jint buttons_mask
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_touch__IIII_3FIFF(
    JNIEnv* env,
    jclass clazz,
    jint input_device,
    jint ev,
    jint pointer,
    jint pointer_count,
    jfloatArray positions,
    jint buttons_mask,
    jfloat vertical_factor,
    jfloat horizontal_factor
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_hover(
    JNIEnv* env,
    jclass clazz,
    jint p_type,
    jfloat p_x,
    jfloat p_y
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_doubleTap(
    JNIEnv* env,
    jclass clazz,
    jint p_button_mask,
    jint p_x,
    jint p_y
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_scroll(
    JNIEnv* env,
    jclass clazz,
    jint p_x,
    jint p_y
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_key(
    JNIEnv* env,
    jclass clazz,
    jint p_keycode,
    jint p_scancode,
    jint p_unicode_char,
    jboolean p_pressed
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_joybutton(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jint p_button,
    jboolean p_pressed
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_joyaxis(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jint p_axis,
    jfloat p_value
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_joyhat(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jint p_hat_x,
    jint p_hat_y
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_joyconnectionchanged(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jboolean p_connected,
    jstring p_name
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_accelerometer(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_gravity(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_magnetometer(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_gyroscope(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_focusin(
    JNIEnv* env,
    jclass clazz
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_focusout(
    JNIEnv* env,
    jclass clazz
);
JNIEXPORT jstring JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_getGlobal(
    JNIEnv* env,
    jclass clazz,
    jstring path
);
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_callobject(
    JNIEnv* env,
    jclass clazz,
    jlong ID,
    jstring method,
    jobjectArray params
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_calldeferred(
    JNIEnv* env,
    jclass clazz,
    jlong ID,
    jstring method,
    jobjectArray params
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_setVirtualKeyboardHeight(
    JNIEnv* env,
    jclass clazz,
    jint p_height
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_requestPermissionResult(
    JNIEnv* env,
    jclass clazz,
    jstring p_permission,
    jboolean p_result
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_onRendererResumed(
    JNIEnv* env,
    jclass clazz
);
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_onRendererPaused(
    JNIEnv* env,
    jclass clazz
);
}

#endif // ANDROID_JNI_H
