// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_jni.h"

#include "android_file_access.h"
#include "android_input_handler.h"
#include "android_java_class.h"
#include "android_jni_dir_access.h"
#include "android_jni_io.h"
#include "android_jni_os.h"
#include "android_jni_singleton.h"
#include "android_jni_thread.h"
#include "android_jni_utils.h"
#include "android_net_socket.h"
#include "android_os.h"
#include "core/engine.h"
#include "core/project_settings.h"
#include "main/input_default.h"
#include "main/main.h"

#include <android/asset_manager_jni.h>
#include <android/input.h>
#include <unistd.h>

static JavaClassWrapper* java_class_wrapper = NULL;
static AndroidOS* android_os                = NULL;
static AndroidInputHandler* input_handler   = NULL;
static AndroidJNIOS* android_jni_os         = NULL;
static AndroidJNIIO* android_jni_io         = NULL;

static bool initialized = false;
static SafeNumeric<int> step; // Shared between UI and render threads

static Size2 new_size;
static Vector3 accelerometer;
static Vector3 gravity;
static Vector3 magnetometer;
static Vector3 gyroscope;

static void _initialize_java_modules() {
    if (!ProjectSettings::get_singleton()->has_setting("android/modules")) {
        return;
    }

    String modules = ProjectSettings::get_singleton()->get("android/modules");
    modules        = modules.strip_edges();
    if (modules == String()) {
        return;
    }
    Vector<String> mods = modules.split(",", false);

    if (mods.size()) {
        jobject cls = android_jni_os->get_class_loader();

        // TODO create wrapper for class loader

        JNIEnv* env         = get_jni_env();
        jclass classLoader  = env->FindClass("java/lang/ClassLoader");
        jmethodID findClass = env->GetMethodID(
            classLoader,
            "loadClass",
            "(Ljava/lang/String;)Ljava/lang/Class;"
        );

        for (int i = 0; i < mods.size(); i++) {
            String m = mods[i];

            print_line("Loading Android module: " + m);
            jstring strClassName = env->NewStringUTF(m.utf8().get_data());
            jclass singletonClass =
                (jclass)env->CallObjectMethod(cls, findClass, strClassName);
            ERR_CONTINUE_MSG(
                !singletonClass,
                "Couldn't find singleton for class: " + m + "."
            );

            jmethodID initialize = env->GetStaticMethodID(
                singletonClass,
                "initialize",
                "(Landroid/app/Activity;)Lcom/rebeltoolbox/rebelengine/"
                "RebelFragment$SingletonBase;"
            );
            ERR_CONTINUE_MSG(
                !initialize,
                "Couldn't find proper initialize function "
                "'public static RebelFragment.SingletonBase Class::initialize("
                "Activity p_activity)'"
                "initializer for singleton class: "
                    + m + "."
            );

            jobject obj = env->CallStaticObjectMethod(
                singletonClass,
                initialize,
                android_jni_os->get_activity()
            );
            env->NewGlobalRef(obj);
        }
    }
}

extern "C" {

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_setVirtualKeyboardHeight(
    JNIEnv* env,
    jclass clazz,
    jint p_height
) {
    if (android_jni_io) {
        android_jni_io->set_vk_height(p_height);
    }
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_initialize(
    JNIEnv* env,
    jclass clazz,
    jobject activity,
    jobject fragment,
    jobject p_asset_manager,
    jboolean p_use_apk_expansion
) {
    initialized = true;

    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    android_jni_os = new AndroidJNIOS(env, activity, fragment);
    android_jni_io = new AndroidJNIIO(
        env,
        android_jni_os->get_member_object(
            "io",
            "Lcom/rebeltoolbox/rebelengine/RebelIO;",
            env
        )
    );

    initialize_android_jni_thread(jvm, env);

    jobject amgr = env->NewGlobalRef(p_asset_manager);

    AndroidFileAccess::asset_manager = AAssetManager_fromJava(env, amgr);

    AndroidJNIDirAccess::setup(android_jni_io->get_instance());
    AndroidNetSocket::setup(android_jni_os->get_member_object(
        "wifiMulticastLock",
        "Lcom/rebeltoolbox/rebelengine/utils/WifiMulticastLock;",
        env
    ));

    android_os =
        new AndroidOS(android_jni_os, android_jni_io, p_use_apk_expansion);

    char wd[500];
    getcwd(wd, 500);

    android_jni_os->on_video_init(env);
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_ondestroy(
    JNIEnv* env,
    jclass clazz
) {
    if (android_jni_io) {
        delete android_jni_io;
    }
    if (android_jni_os) {
        delete android_jni_os;
    }
    if (input_handler) {
        delete input_handler;
    }
    if (android_os) {
        delete android_os;
    }
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_setup(
    JNIEnv* env,
    jclass clazz,
    jobjectArray p_cmdline
) {
    setup_android_thread();

    const char** cmdline = NULL;
    jstring* j_cmdline   = NULL;
    int cmdlen           = 0;
    if (p_cmdline) {
        cmdlen = env->GetArrayLength(p_cmdline);
        if (cmdlen) {
            cmdline =
                (const char**)memalloc((cmdlen + 1) * sizeof(const char*));
            ERR_FAIL_NULL_MSG(cmdline, "Out of memory.");
            cmdline[cmdlen] = NULL;
            j_cmdline       = (jstring*)memalloc(cmdlen * sizeof(jstring));
            ERR_FAIL_NULL_MSG(j_cmdline, "Out of memory.");

            for (int i = 0; i < cmdlen; i++) {
                jstring string =
                    (jstring)env->GetObjectArrayElement(p_cmdline, i);
                const char* rawString = env->GetStringUTFChars(string, 0);

                cmdline[i]   = rawString;
                j_cmdline[i] = string;
            }
        }
    }

    Error err = Main::setup("apk", cmdlen, (char**)cmdline, false);
    if (cmdline) {
        if (j_cmdline) {
            for (int i = 0; i < cmdlen; ++i) {
                env->ReleaseStringUTFChars(j_cmdline[i], cmdline[i]);
            }
            memfree(j_cmdline);
        }
        memfree(cmdline);
    }

    if (err != OK) {
        return; // should exit instead and print the error
    }

    java_class_wrapper =
        memnew(JavaClassWrapper(android_jni_os->get_activity()));
    ClassDB::register_class<JNISingleton>();
    _initialize_java_modules();
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_resize(
    JNIEnv* env,
    jclass clazz,
    jint width,
    jint height
) {
    if (android_os) {
        android_os->set_display_size(Size2(width, height));
    }
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_newcontext(
    JNIEnv* env,
    jclass clazz,
    jboolean p_32_bits
) {
    if (android_os) {
        if (step.get() == 0) {
            // During startup
            android_os->set_context_is_16_bits(!p_32_bits);
        } else {
            // GL context recreated because it was lost; restart app to let it
            // reload everything
            step.set(-1); // Ensure no further steps are attempted and no
                          // further events are sent
            android_os->main_loop_end();
            android_jni_os->restart(env);
        }
    }
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_back(JNIEnv* env, jclass clazz) {
    if (step.get() == 0) {
        return;
    }

    if (android_os->get_main_loop()) {
        android_os->get_main_loop()->notification(
            MainLoop::NOTIFICATION_WM_GO_BACK_REQUEST
        );
    }
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_step(JNIEnv* env, jclass clazz) {
    if (step.get() == -1) {
        return;
    }

    if (step.get() == 0) {
        // Rebel is initialized on Android's UI thread, so
        // _main_thread_id was set to that thread's id, but
        // Rebel Engine's main thread is the one running the game loop.
        Main::setup2(Thread::get_caller_id());
        input_handler = new AndroidInputHandler();
        step.increment();
        return;
    }

    if (step.get() == 1) {
        if (!Main::start()) {
            return; // should exit instead and print the error
        }

        android_jni_os->on_setup_completed(env);
        android_os->main_loop_begin();
        android_jni_os->on_main_loop_started(env);
        step.increment();
    }

    android_os->process_accelerometer(accelerometer);
    android_os->process_gravity(gravity);
    android_os->process_magnetometer(magnetometer);
    android_os->process_gyroscope(gyroscope);

    if (android_os->main_loop_iterate()) {
        android_jni_os->force_quit(env);
    }
}

void touch_preprocessing(
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
) {
    if (step.get() <= 0) {
        return;
    }

    Vector<AndroidInputHandler::Touch> touches;
    for (int i = 0; i < pointer_count; i++) {
        jfloat p[3];
        env->GetFloatArrayRegion(positions, i * 3, 3, p);
        AndroidInputHandler::Touch touch;
        touch.pos = Point2(p[1], p[2]);
        touch.id  = (int)p[0];
        touches.push_back(touch);
    }

    if ((input_device & AINPUT_SOURCE_MOUSE) == AINPUT_SOURCE_MOUSE) {
        input_handler->process_mouse_event(
            ev,
            buttons_mask,
            touches[0].pos,
            vertical_factor,
            horizontal_factor
        );
    } else {
        input_handler->process_touch(ev, pointer, touches);
    }
}

// Called on the UI thread
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_touch__IIII_3F(
    JNIEnv* env,
    jclass clazz,
    jint input_device,
    jint ev,
    jint pointer,
    jint pointer_count,
    jfloatArray position
) {
    touch_preprocessing(
        env,
        clazz,
        input_device,
        ev,
        pointer,
        pointer_count,
        position
    );
}

// Called on the UI thread
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_touch__IIII_3FI(
    JNIEnv* env,
    jclass clazz,
    jint input_device,
    jint ev,
    jint pointer,
    jint pointer_count,
    jfloatArray position,
    jint buttons_mask
) {
    touch_preprocessing(
        env,
        clazz,
        input_device,
        ev,
        pointer,
        pointer_count,
        position,
        buttons_mask
    );
}

// Called on the UI thread
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_touch__IIII_3FIFF(
    JNIEnv* env,
    jclass clazz,
    jint input_device,
    jint ev,
    jint pointer,
    jint pointer_count,
    jfloatArray position,
    jint buttons_mask,
    jfloat vertical_factor,
    jfloat horizontal_factor
) {
    touch_preprocessing(
        env,
        clazz,
        input_device,
        ev,
        pointer,
        pointer_count,
        position,
        buttons_mask,
        vertical_factor,
        horizontal_factor
    );
}

// Called on the UI thread
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_hover(
    JNIEnv* env,
    jclass clazz,
    jint p_type,
    jfloat p_x,
    jfloat p_y
) {
    if (step.get() <= 0) {
        return;
    }

    input_handler->process_hover(p_type, Point2(p_x, p_y));
}

// Called on the UI thread
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_doubleTap(
    JNIEnv* env,
    jclass clazz,
    jint p_button_mask,
    jint p_x,
    jint p_y
) {
    if (step.get() <= 0) {
        return;
    }

    input_handler->process_double_tap(p_button_mask, Point2(p_x, p_y));
}

// Called on the UI thread
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_scroll(
    JNIEnv* env,
    jclass clazz,
    jint p_x,
    jint p_y
) {
    if (step.get() <= 0) {
        return;
    }

    input_handler->process_scroll(Point2(p_x, p_y));
}

// Called on the UI thread
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_joybutton(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jint p_button,
    jboolean p_pressed
) {
    if (step.get() <= 0) {
        return;
    }

    AndroidInputHandler::JoypadEvent jevent;
    jevent.device  = p_device;
    jevent.type    = AndroidInputHandler::JOY_EVENT_BUTTON;
    jevent.index   = p_button;
    jevent.pressed = p_pressed;

    input_handler->process_joy_event(jevent);
}

// Called on the UI thread
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_joyaxis(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jint p_axis,
    jfloat p_value
) {
    if (step.get() <= 0) {
        return;
    }

    AndroidInputHandler::JoypadEvent jevent;
    jevent.device = p_device;
    jevent.type   = AndroidInputHandler::JOY_EVENT_AXIS;
    jevent.index  = p_axis;
    jevent.value  = p_value;

    input_handler->process_joy_event(jevent);
}

// Called on the UI thread
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_joyhat(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jint p_hat_x,
    jint p_hat_y
) {
    if (step.get() <= 0) {
        return;
    }

    AndroidInputHandler::JoypadEvent jevent;
    jevent.device = p_device;
    jevent.type   = AndroidInputHandler::JOY_EVENT_HAT;
    int hat       = 0;
    if (p_hat_x != 0) {
        if (p_hat_x < 0) {
            hat |= InputDefault::HAT_MASK_LEFT;
        } else {
            hat |= InputDefault::HAT_MASK_RIGHT;
        }
    }
    if (p_hat_y != 0) {
        if (p_hat_y < 0) {
            hat |= InputDefault::HAT_MASK_UP;
        } else {
            hat |= InputDefault::HAT_MASK_DOWN;
        }
    }
    jevent.hat = hat;

    input_handler->process_joy_event(jevent);
}

// Called on the UI thread
JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_joyconnectionchanged(
    JNIEnv* env,
    jclass clazz,
    jint p_device,
    jboolean p_connected,
    jstring p_name
) {
    if (step.get() <= 0) {
        return;
    }
    String name = string_from_jstring(env, p_name);
    input_handler->joy_connection_changed(p_device, p_connected, name);
}

// Called on the UI thread
JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_key(
    JNIEnv* env,
    jclass clazz,
    jint p_keycode,
    jint p_scancode,
    jint p_unicode_char,
    jboolean p_pressed
) {
    if (step.get() <= 0) {
        return;
    }

    input_handler
        ->process_key_event(p_keycode, p_scancode, p_unicode_char, p_pressed);
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_accelerometer(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
) {
    accelerometer = Vector3(x, y, z);
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_gravity(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
) {
    gravity = Vector3(x, y, z);
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_magnetometer(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
) {
    magnetometer = Vector3(x, y, z);
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_gyroscope(
    JNIEnv* env,
    jclass clazz,
    jfloat x,
    jfloat y,
    jfloat z
) {
    gyroscope = Vector3(x, y, z);
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_focusin(
    JNIEnv* env,
    jclass clazz
) {
    if (step.get() <= 0) {
        return;
    }

    android_os->main_loop_focusin();
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_focusout(
    JNIEnv* env,
    jclass clazz
) {
    if (step.get() <= 0) {
        return;
    }

    android_os->main_loop_focusout();
}

JNIEXPORT jstring JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_getGlobal(
    JNIEnv* env,
    jclass clazz,
    jstring path
) {
    String js = string_from_jstring(env, path);
    return env->NewStringUTF(ProjectSettings::get_singleton()
                                 ->get(js)
                                 .
                                 operator String()
                                 .utf8()
                                 .get_data());
}

JNIEXPORT void JNICALL Java_com_rebeltoolbox_rebelengine_RebelEngine_callobject(
    JNIEnv* env,
    jclass clazz,
    jlong ID,
    jstring method,
    jobjectArray params
) {
    Object* obj = ObjectDB::get_instance(ID);
    ERR_FAIL_COND(!obj);

    int res = env->PushLocalFrame(16);
    ERR_FAIL_COND(res != 0);

    String str_method = string_from_jstring(env, method);
    int count         = env->GetArrayLength(params);
    Variant* vlist    = (Variant*)alloca(sizeof(Variant) * count);
    Variant** vptr    = (Variant**)alloca(sizeof(Variant*) * count);
    for (int i = 0; i < count; i++) {
        jobject obj = env->GetObjectArrayElement(params, i);
        Variant v;
        if (obj) {
            v = _jobject_to_variant(env, obj);
        }
        memnew_placement(&vlist[i], Variant);
        vlist[i] = v;
        vptr[i]  = &vlist[i];
        env->DeleteLocalRef(obj);
    };

    Variant::CallError err;
    obj->call(str_method, (const Variant**)vptr, count, err);
    // something

    env->PopLocalFrame(NULL);
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_calldeferred(
    JNIEnv* env,
    jclass clazz,
    jlong ID,
    jstring method,
    jobjectArray params
) {
    Object* obj = ObjectDB::get_instance(ID);
    ERR_FAIL_COND(!obj);

    int res = env->PushLocalFrame(16);
    ERR_FAIL_COND(res != 0);

    String str_method = string_from_jstring(env, method);
    int count         = env->GetArrayLength(params);
    Variant args[VARIANT_ARG_MAX];

    for (int i = 0; i < MIN(count, VARIANT_ARG_MAX); i++) {
        jobject obj = env->GetObjectArrayElement(params, i);
        if (obj) {
            args[i] = _jobject_to_variant(env, obj);
        }
        env->DeleteLocalRef(obj);
    };

    obj->call_deferred(str_method, args[0], args[1], args[2], args[3], args[4]);
    // something
    env->PopLocalFrame(NULL);
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_requestPermissionResult(
    JNIEnv* env,
    jclass clazz,
    jstring p_permission,
    jboolean p_result
) {
    String permission = string_from_jstring(env, p_permission);
    if (permission == "android.permission.RECORD_AUDIO" && p_result) {
        AudioDriver::get_singleton()->capture_start();
    }

    if (android_os->get_main_loop()) {
        android_os->get_main_loop()->emit_signal(
            "on_request_permissions_result",
            permission,
            p_result == JNI_TRUE
        );
    }
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_onRendererResumed(
    JNIEnv* env,
    jclass clazz
) {
    if (step.get() <= 0) {
        return;
    }

    if (android_os->get_main_loop()) {
        android_os->get_main_loop()->notification(
            MainLoop::NOTIFICATION_APP_RESUMED
        );
    }
}

JNIEXPORT void JNICALL
Java_com_rebeltoolbox_rebelengine_RebelEngine_onRendererPaused(
    JNIEnv* env,
    jclass clazz
) {
    if (step.get() <= 0) {
        return;
    }

    if (android_os->get_main_loop()) {
        android_os->get_main_loop()->notification(
            MainLoop::NOTIFICATION_APP_PAUSED
        );
    }
}
}
