// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "jni_utils.h"

jvalret _variant_to_jvalue(
    JNIEnv* env,
    Variant::Type p_type,
    const Variant* p_arg,
    bool force_jobject
) {
    jvalret v;

    switch (p_type) {
        case Variant::BOOL: {
            if (force_jobject) {
                jclass bclass  = env->FindClass("java/lang/Boolean");
                jmethodID ctor = env->GetMethodID(bclass, "<init>", "(Z)V");
                jvalue val;
                val.z       = (bool)(*p_arg);
                jobject obj = env->NewObjectA(bclass, ctor, &val);
                v.val.l     = obj;
                v.obj       = obj;
                env->DeleteLocalRef(bclass);
            } else {
                v.val.z = *p_arg;
            };
        } break;
        case Variant::INT: {
            if (force_jobject) {
                jclass bclass  = env->FindClass("java/lang/Integer");
                jmethodID ctor = env->GetMethodID(bclass, "<init>", "(I)V");
                jvalue val;
                val.i       = (int)(*p_arg);
                jobject obj = env->NewObjectA(bclass, ctor, &val);
                v.val.l     = obj;
                v.obj       = obj;
                env->DeleteLocalRef(bclass);

            } else {
                v.val.i = *p_arg;
            };
        } break;
        case Variant::REAL: {
            if (force_jobject) {
                jclass bclass  = env->FindClass("java/lang/Double");
                jmethodID ctor = env->GetMethodID(bclass, "<init>", "(D)V");
                jvalue val;
                val.d       = (double)(*p_arg);
                jobject obj = env->NewObjectA(bclass, ctor, &val);
                v.val.l     = obj;
                v.obj       = obj;
                env->DeleteLocalRef(bclass);

            } else {
                v.val.f = *p_arg;
            };
        } break;
        case Variant::STRING: {
            String s     = *p_arg;
            jstring jStr = env->NewStringUTF(s.utf8().get_data());
            v.val.l      = jStr;
            v.obj        = jStr;
        } break;
        case Variant::POOL_STRING_ARRAY: {
            PoolVector<String> sarray = *p_arg;
            jobjectArray arr          = env->NewObjectArray(
                sarray.size(),
                env->FindClass("java/lang/String"),
                env->NewStringUTF("")
            );

            for (int j = 0; j < sarray.size(); j++) {
                jstring str = env->NewStringUTF(sarray[j].utf8().get_data());
                env->SetObjectArrayElement(arr, j, str);
                env->DeleteLocalRef(str);
            }
            v.val.l = arr;
            v.obj   = arr;

        } break;

        case Variant::DICTIONARY: {
            Dictionary dict = *p_arg;
            jclass dclass =
                env->FindClass("com/rebeltoolbox/rebelengine/Dictionary");
            jmethodID ctor = env->GetMethodID(dclass, "<init>", "()V");
            jobject jdict  = env->NewObject(dclass, ctor);

            Array keys = dict.keys();

            jobjectArray jkeys = env->NewObjectArray(
                keys.size(),
                env->FindClass("java/lang/String"),
                env->NewStringUTF("")
            );
            for (int j = 0; j < keys.size(); j++) {
                jstring str =
                    env->NewStringUTF(String(keys[j]).utf8().get_data());
                env->SetObjectArrayElement(jkeys, j, str);
                env->DeleteLocalRef(str);
            };

            jmethodID set_keys =
                env->GetMethodID(dclass, "set_keys", "([Ljava/lang/String;)V");
            jvalue val;
            val.l = jkeys;
            env->CallVoidMethodA(jdict, set_keys, &val);
            env->DeleteLocalRef(jkeys);

            jobjectArray jvalues = env->NewObjectArray(
                keys.size(),
                env->FindClass("java/lang/Object"),
                NULL
            );

            for (int j = 0; j < keys.size(); j++) {
                Variant var = dict[keys[j]];
                jvalret v = _variant_to_jvalue(env, var.get_type(), &var, true);
                env->SetObjectArrayElement(jvalues, j, v.val.l);
                if (v.obj) {
                    env->DeleteLocalRef(v.obj);
                }
            };

            jmethodID set_values = env->GetMethodID(
                dclass,
                "set_values",
                "([Ljava/lang/Object;)V"
            );
            val.l = jvalues;
            env->CallVoidMethodA(jdict, set_values, &val);
            env->DeleteLocalRef(jvalues);
            env->DeleteLocalRef(dclass);

            v.val.l = jdict;
            v.obj   = jdict;
        } break;

        case Variant::POOL_INT_ARRAY: {
            PoolVector<int> array   = *p_arg;
            jintArray arr           = env->NewIntArray(array.size());
            PoolVector<int>::Read r = array.read();
            env->SetIntArrayRegion(arr, 0, array.size(), r.ptr());
            v.val.l = arr;
            v.obj   = arr;

        } break;
        case Variant::POOL_BYTE_ARRAY: {
            PoolVector<uint8_t> array   = *p_arg;
            jbyteArray arr              = env->NewByteArray(array.size());
            PoolVector<uint8_t>::Read r = array.read();
            env->SetByteArrayRegion(
                arr,
                0,
                array.size(),
                reinterpret_cast<const signed char*>(r.ptr())
            );
            v.val.l = arr;
            v.obj   = arr;

        } break;
        case Variant::POOL_REAL_ARRAY: {
            PoolVector<float> array   = *p_arg;
            jfloatArray arr           = env->NewFloatArray(array.size());
            PoolVector<float>::Read r = array.read();
            env->SetFloatArrayRegion(arr, 0, array.size(), r.ptr());
            v.val.l = arr;
            v.obj   = arr;

        } break;
        default: {
            v.val.i = 0;
        } break;
    }
    return v;
}

String _get_class_name(JNIEnv* env, jclass cls, bool* array) {
    jclass cclass = env->FindClass("java/lang/Class");
    jmethodID getName =
        env->GetMethodID(cclass, "getName", "()Ljava/lang/String;");
    jstring clsName = (jstring)env->CallObjectMethod(cls, getName);

    if (array) {
        jmethodID isArray = env->GetMethodID(cclass, "isArray", "()Z");
        jboolean isarr    = env->CallBooleanMethod(cls, isArray);
        (*array)          = isarr ? true : false;
    }
    String name = string_from_jstring(env, clsName);
    env->DeleteLocalRef(clsName);

    return name;
}

Variant _jobject_to_variant(JNIEnv* env, jobject obj) {
    if (obj == NULL) {
        return Variant();
    }

    jclass c = env->GetObjectClass(obj);
    bool array;
    String name = _get_class_name(env, c, &array);

    if (name == "java.lang.String") {
        return string_from_jstring(env, (jstring)obj);
    };

    if (name == "[Ljava.lang.String;") {
        jobjectArray arr = (jobjectArray)obj;
        int stringCount  = env->GetArrayLength(arr);
        PoolVector<String> sarr;

        for (int i = 0; i < stringCount; i++) {
            jstring string = (jstring)env->GetObjectArrayElement(arr, i);
            sarr.push_back(string_from_jstring(env, string));
            env->DeleteLocalRef(string);
        }

        return sarr;
    };

    if (name == "java.lang.Boolean") {
        jmethodID boolValue = env->GetMethodID(c, "booleanValue", "()Z");
        bool ret            = env->CallBooleanMethod(obj, boolValue);
        return ret;
    };

    if (name == "java.lang.Integer" || name == "java.lang.Long") {
        jclass nclass       = env->FindClass("java/lang/Number");
        jmethodID longValue = env->GetMethodID(nclass, "longValue", "()J");
        jlong ret           = env->CallLongMethod(obj, longValue);
        return ret;
    };

    if (name == "[I") {
        jintArray arr = (jintArray)obj;
        int fCount    = env->GetArrayLength(arr);
        PoolVector<int> sarr;
        sarr.resize(fCount);

        PoolVector<int>::Write w = sarr.write();
        env->GetIntArrayRegion(arr, 0, fCount, w.ptr());
        w.release();
        return sarr;
    };

    if (name == "[B") {
        jbyteArray arr = (jbyteArray)obj;
        int fCount     = env->GetArrayLength(arr);
        PoolVector<uint8_t> sarr;
        sarr.resize(fCount);

        PoolVector<uint8_t>::Write w = sarr.write();
        env->GetByteArrayRegion(
            arr,
            0,
            fCount,
            reinterpret_cast<signed char*>(w.ptr())
        );
        w.release();
        return sarr;
    };

    if (name == "java.lang.Float" || name == "java.lang.Double") {
        jclass nclass         = env->FindClass("java/lang/Number");
        jmethodID doubleValue = env->GetMethodID(nclass, "doubleValue", "()D");
        double ret            = env->CallDoubleMethod(obj, doubleValue);
        return ret;
    };

    if (name == "[D") {
        jdoubleArray arr = (jdoubleArray)obj;
        int fCount       = env->GetArrayLength(arr);
        PoolRealArray sarr;
        sarr.resize(fCount);

        PoolRealArray::Write w = sarr.write();

        for (int i = 0; i < fCount; i++) {
            double n;
            env->GetDoubleArrayRegion(arr, i, 1, &n);
            w.ptr()[i] = n;
        };
        return sarr;
    };

    if (name == "[F") {
        jfloatArray arr = (jfloatArray)obj;
        int fCount      = env->GetArrayLength(arr);
        PoolRealArray sarr;
        sarr.resize(fCount);

        PoolRealArray::Write w = sarr.write();

        for (int i = 0; i < fCount; i++) {
            float n;
            env->GetFloatArrayRegion(arr, i, 1, &n);
            w.ptr()[i] = n;
        };
        return sarr;
    };

    if (name == "[Ljava.lang.Object;") {
        jobjectArray arr = (jobjectArray)obj;
        int objCount     = env->GetArrayLength(arr);
        Array varr;

        for (int i = 0; i < objCount; i++) {
            jobject jobj = env->GetObjectArrayElement(arr, i);
            Variant v    = _jobject_to_variant(env, jobj);
            varr.push_back(v);
            env->DeleteLocalRef(jobj);
        }

        return varr;
    };

    if (name == "java.util.HashMap"
        || name == "com.rebeltoolbox.rebelengine.Dictionary") {
        Dictionary ret;
        jclass oclass = c;
        jmethodID get_keys =
            env->GetMethodID(oclass, "get_keys", "()[Ljava/lang/String;");
        jobjectArray arr = (jobjectArray)env->CallObjectMethod(obj, get_keys);

        PoolStringArray keys = _jobject_to_variant(env, arr);
        env->DeleteLocalRef(arr);

        jmethodID get_values =
            env->GetMethodID(oclass, "get_values", "()[Ljava/lang/Object;");
        arr = (jobjectArray)env->CallObjectMethod(obj, get_values);

        Array vals = _jobject_to_variant(env, arr);
        env->DeleteLocalRef(arr);

        for (int i = 0; i < keys.size(); i++) {
            ret[keys[i]] = vals[i];
        };

        return ret;
    };

    env->DeleteLocalRef(c);

    return Variant();
}

Variant::Type get_jni_type(const String& p_type) {
    static struct {
        const char* name;
        Variant::Type type;
    } _type_to_vtype[] = {
        {"void",                                    Variant::NIL              },
        {"boolean",                                 Variant::BOOL             },
        {"int",                                     Variant::INT              },
        {"float",                                   Variant::REAL             },
        {"double",                                  Variant::REAL             },
        {"java.lang.String",                        Variant::STRING           },
        {"[I",                                      Variant::POOL_INT_ARRAY   },
        {"[B",                                      Variant::POOL_BYTE_ARRAY  },
        {"[F",                                      Variant::POOL_REAL_ARRAY  },
        {"[Ljava.lang.String;",                     Variant::POOL_STRING_ARRAY},
        {"com.rebeltoolbox.rebelengine.Dictionary", Variant::DICTIONARY       },
        {NULL,                                      Variant::NIL              }
    };

    int idx = 0;

    while (_type_to_vtype[idx].name) {
        if (p_type == _type_to_vtype[idx].name) {
            return _type_to_vtype[idx].type;
        }

        idx++;
    }

    return Variant::NIL;
}

const char* get_jni_sig(const String& p_type) {
    static struct {
        const char* name;
        const char* sig;
    } _type_to_vtype[] = {
        {"void",                                    "V"                  },
        {"boolean",                                 "Z"                  },
        {"int",                                     "I"                  },
        {"float",                                   "F"                  },
        {"double",                                  "D"                  },
        {"java.lang.String",                        "Ljava/lang/String;" },
        {"com.rebeltoolbox.rebelengine.Dictionary",
         "Lcom/rebeltoolbox/rebelengine/Dictionary;"                     },
        {"[I",                                      "[I"                 },
        {"[B",                                      "[B"                 },
        {"[F",                                      "[F"                 },
        {"[Ljava.lang.String;",                     "[Ljava/lang/String;"},
        {NULL,                                      "V"                  }
    };

    int idx = 0;

    while (_type_to_vtype[idx].name) {
        if (p_type == _type_to_vtype[idx].name) {
            return _type_to_vtype[idx].sig;
        }

        idx++;
    }

    return "Ljava/lang/Object;";
}

String string_from_jstring(JNIEnv* env, jstring string) {
    String result;
    const char* utf8 = env->GetStringUTFChars(string, NULL);
    result.parse_utf8(utf8);
    env->ReleaseStringUTFChars(string, utf8);
    return result;
}
