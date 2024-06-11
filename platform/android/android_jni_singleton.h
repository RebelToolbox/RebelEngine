// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_JNI_SINGLETON_H
#define ANDROID_JNI_SINGLETON_H

#include "android_jni_thread.h"
#include "android_jni_utils.h"
#include "core/engine.h"
#include "core/variant.h"

class JNISingleton : public Object {
    GDCLASS(JNISingleton, Object);

    struct MethodData {
        jmethodID method;
        Variant::Type ret_type;
        Vector<Variant::Type> argtypes;
    };

    jobject instance;
    Map<StringName, MethodData> method_map;

public:
    virtual Variant call(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    ) {
        Map<StringName, MethodData>::Element* E = method_map.find(p_method);

        // Check the method we're looking for is in the JNISingleton map and
        // that the arguments match.
        bool call_error = !E || E->get().argtypes.size() != p_argcount;
        if (!call_error) {
            for (int i = 0; i < p_argcount; i++) {
                if (!Variant::can_convert(
                        p_args[i]->get_type(),
                        E->get().argtypes[i]
                    )) {
                    call_error = true;
                    break;
                }
            }
        }

        if (call_error) {
            // The method is not in this map, defaulting to the regular instance
            // calls.
            return Object::call(p_method, p_args, p_argcount, r_error);
        }

        ERR_FAIL_COND_V(!instance, Variant());

        r_error.error = Variant::CallError::CALL_OK;

        jvalue* v = NULL;

        if (p_argcount) {
            v = (jvalue*)alloca(sizeof(jvalue) * p_argcount);
        }

        JNIEnv* env = get_jni_env();

        int res = env->PushLocalFrame(16);

        ERR_FAIL_COND_V(res != 0, Variant());

        List<jobject> to_erase;
        for (int i = 0; i < p_argcount; i++) {
            jvalret vr =
                _variant_to_jvalue(env, E->get().argtypes[i], p_args[i]);
            v[i] = vr.val;
            if (vr.obj) {
                to_erase.push_back(vr.obj);
            }
        }

        Variant ret;

        switch (E->get().ret_type) {
            case Variant::NIL: {
                env->CallVoidMethodA(instance, E->get().method, v);
            } break;
            case Variant::BOOL: {
                ret = env->CallBooleanMethodA(instance, E->get().method, v)
                   == JNI_TRUE;
            } break;
            case Variant::INT: {
                ret = env->CallIntMethodA(instance, E->get().method, v);
            } break;
            case Variant::REAL: {
                ret = env->CallFloatMethodA(instance, E->get().method, v);
            } break;
            case Variant::STRING: {
                jobject o =
                    env->CallObjectMethodA(instance, E->get().method, v);
                ret = string_from_jstring(env, (jstring)o);
                env->DeleteLocalRef(o);
            } break;
            case Variant::POOL_STRING_ARRAY: {
                jobjectArray arr = (jobjectArray
                )env->CallObjectMethodA(instance, E->get().method, v);

                ret = _jobject_to_variant(env, arr);

                env->DeleteLocalRef(arr);
            } break;
            case Variant::POOL_INT_ARRAY: {
                jintArray arr = (jintArray
                )env->CallObjectMethodA(instance, E->get().method, v);

                int fCount = env->GetArrayLength(arr);
                PoolVector<int> sarr;
                sarr.resize(fCount);

                PoolVector<int>::Write w = sarr.write();
                env->GetIntArrayRegion(arr, 0, fCount, w.ptr());
                w.release();
                ret = sarr;
                env->DeleteLocalRef(arr);
            } break;
            case Variant::POOL_REAL_ARRAY: {
                jfloatArray arr = (jfloatArray
                )env->CallObjectMethodA(instance, E->get().method, v);

                int fCount = env->GetArrayLength(arr);
                PoolVector<float> sarr;
                sarr.resize(fCount);

                PoolVector<float>::Write w = sarr.write();
                env->GetFloatArrayRegion(arr, 0, fCount, w.ptr());
                w.release();
                ret = sarr;
                env->DeleteLocalRef(arr);
            } break;

            case Variant::DICTIONARY: {
                jobject obj =
                    env->CallObjectMethodA(instance, E->get().method, v);
                ret = _jobject_to_variant(env, obj);
                env->DeleteLocalRef(obj);

            } break;
            default: {
                env->PopLocalFrame(NULL);
                ERR_FAIL_V(Variant());
            } break;
        }

        while (to_erase.size()) {
            env->DeleteLocalRef(to_erase.front()->get());
            to_erase.pop_front();
        }

        env->PopLocalFrame(NULL);

        return ret;
    }

    jobject get_instance() const {
        return instance;
    }

    void set_instance(jobject p_instance) {
        instance = p_instance;
    }

    void add_method(
        const StringName& p_name,
        jmethodID p_method,
        const Vector<Variant::Type>& p_args,
        Variant::Type p_ret_type
    ) {
        MethodData md;
        md.method          = p_method;
        md.argtypes        = p_args;
        md.ret_type        = p_ret_type;
        method_map[p_name] = md;
    }

    void add_signal(
        const StringName& p_name,
        const Vector<Variant::Type>& p_args
    ) {
        if (p_args.size() == 0) {
            ADD_SIGNAL(MethodInfo(p_name));
        } else if (p_args.size() == 1) {
            ADD_SIGNAL(MethodInfo(p_name, PropertyInfo(p_args[0], "arg1")));
        } else if (p_args.size() == 2) {
            ADD_SIGNAL(MethodInfo(
                p_name,
                PropertyInfo(p_args[0], "arg1"),
                PropertyInfo(p_args[1], "arg2")
            ));
        } else if (p_args.size() == 3) {
            ADD_SIGNAL(MethodInfo(
                p_name,
                PropertyInfo(p_args[0], "arg1"),
                PropertyInfo(p_args[1], "arg2"),
                PropertyInfo(p_args[2], "arg3")
            ));
        } else if (p_args.size() == 4) {
            ADD_SIGNAL(MethodInfo(
                p_name,
                PropertyInfo(p_args[0], "arg1"),
                PropertyInfo(p_args[1], "arg2"),
                PropertyInfo(p_args[2], "arg3"),
                PropertyInfo(p_args[3], "arg4")
            ));
        } else if (p_args.size() == 5) {
            ADD_SIGNAL(MethodInfo(
                p_name,
                PropertyInfo(p_args[0], "arg1"),
                PropertyInfo(p_args[1], "arg2"),
                PropertyInfo(p_args[2], "arg3"),
                PropertyInfo(p_args[3], "arg4"),
                PropertyInfo(p_args[4], "arg5")
            ));
        }
    }

    JNISingleton() {
        instance = NULL;
    }
};

#endif // ANDROID_JNI_SINGLETON_H
