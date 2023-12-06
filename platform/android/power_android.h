// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef POWER_ANDROID_H
#define POWER_ANDROID_H

#include "core/os/os.h"

#include <android/native_window_jni.h>

class PowerAndroid {
    struct LocalReferenceHolder {
        JNIEnv* m_env;
        const char* m_func;
    };

private:
    static struct LocalReferenceHolder refs;
    static JNIEnv* env;
    static jmethodID mid;
    static jobject context;
    static jstring action;
    static jclass cls;
    static jobject filter;
    static jobject intent;
    static jstring iname;
    static jmethodID imid;
    static jstring bname;
    static jmethodID bmid;

    int nsecs_left;
    int percent_left;
    OS::PowerState power_state;

    bool GetPowerInfo_Android();
    bool UpdatePowerInfo();

public:
    static int s_active;

    PowerAndroid();
    virtual ~PowerAndroid();
    static bool LocalReferenceHolder_Init(
        struct LocalReferenceHolder* refholder,
        JNIEnv* env
    );
    static struct LocalReferenceHolder LocalReferenceHolder_Setup(
        const char* func
    );
    static void LocalReferenceHolder_Cleanup(
        struct LocalReferenceHolder* refholder
    );

    OS::PowerState get_power_state();
    int get_power_seconds_left();
    int get_power_percent_left();
};

#endif // POWER_ANDROID_H
