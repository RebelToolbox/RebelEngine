// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "web_javascript.h"

#include "emscripten.h"

extern "C" {
extern void rebel_js_os_download_buffer(
    const uint8_t* p_buf,
    int p_buf_size,
    const char* p_name,
    const char* p_mime
);
}

#ifdef WEB_EVAL_ENABLED

extern "C" {
typedef union {
    int64_t i;
    double r;
    void* p;
} rebel_js_wrapper_ex;

typedef int (*RebelJSWrapperVariant2JSCallback)(
    const void** p_args,
    int p_pos,
    rebel_js_wrapper_ex* r_val,
    void** p_lock
);
typedef void (*RebelJSWrapperFreeLockCallback)(void** p_lock, int p_type);
extern int rebel_js_wrapper_interface_get(const char* p_name);
extern int rebel_js_wrapper_object_call(
    int p_id,
    const char* p_method,
    void** p_args,
    int p_argc,
    RebelJSWrapperVariant2JSCallback p_variant2js_callback,
    rebel_js_wrapper_ex* p_cb_rval,
    void** p_lock,
    RebelJSWrapperFreeLockCallback p_lock_callback
);
extern int rebel_js_wrapper_object_get(
    int p_id,
    rebel_js_wrapper_ex* p_val,
    const char* p_prop
);
extern int rebel_js_wrapper_object_getvar(
    int p_id,
    int p_type,
    rebel_js_wrapper_ex* p_val
);
extern int rebel_js_wrapper_object_setvar(
    int p_id,
    int p_key_type,
    rebel_js_wrapper_ex* p_key_ex,
    int p_val_type,
    rebel_js_wrapper_ex* p_val_ex
);
extern void rebel_js_wrapper_object_set(
    int p_id,
    const char* p_name,
    int p_type,
    rebel_js_wrapper_ex* p_val
);
extern void rebel_js_wrapper_object_unref(int p_id);
extern int rebel_js_wrapper_create_cb(
    void* p_ref,
    void (*p_callback)(void* p_ref, int p_arg_id, int p_argc)
);
extern int rebel_js_wrapper_create_object(
    const char* p_method,
    void** p_args,
    int p_argc,
    RebelJSWrapperVariant2JSCallback p_variant2js_callback,
    rebel_js_wrapper_ex* p_cb_rval,
    void** p_lock,
    RebelJSWrapperFreeLockCallback p_lock_callback
);
};

class JavaScriptObjectImpl : public JavaScriptObject {
private:
    friend class JavaScript;

    int _js_id   = 0;
    Object* _ref = nullptr;
    StringName _method;

    static int _variant2js(
        const void** p_args,
        int p_pos,
        rebel_js_wrapper_ex* r_val,
        void** p_lock
    );
    static void _free_lock(void** p_lock, int p_type);
    static Variant _js2variant(int p_type, rebel_js_wrapper_ex* p_val);
    static void* _alloc_variants(int p_size);
    static void _callback(void* p_ref, int p_arg_id, int p_argc);

protected:
    bool _set(const StringName& p_name, const Variant& p_value) override;
    bool _get(const StringName& p_name, Variant& r_ret) const override;
    void _get_property_list(List<PropertyInfo>* p_list) const override;

public:
    Variant getvar(const Variant& p_key, bool* r_valid = nullptr)
        const override;
    void setvar(
        const Variant& p_key,
        const Variant& p_value,
        bool* r_valid = nullptr
    ) override;
    Variant call(
        const StringName& p_method,
        const Variant** p_args,
        int p_argc,
        Variant::CallError& r_error
    ) override;

    JavaScriptObjectImpl() {}

    JavaScriptObjectImpl(int p_id) {
        _js_id = p_id;
    }

    ~JavaScriptObjectImpl() {
        if (_js_id) {
            rebel_js_wrapper_object_unref(_js_id);
        }
    }
};

bool JavaScriptObjectImpl::_set(
    const StringName& p_name,
    const Variant& p_value
) {
    ERR_FAIL_COND_V_MSG(!_js_id, false, "Invalid JS instance");
    const String name = p_name;
    rebel_js_wrapper_ex exchange;
    void* lock       = nullptr;
    const Variant* v = &p_value;
    int type         = _variant2js((const void**)&v, 0, &exchange, &lock);
    rebel_js_wrapper_object_set(
        _js_id,
        name.utf8().get_data(),
        type,
        &exchange
    );
    if (lock) {
        _free_lock(&lock, type);
    }
    return true;
}

bool JavaScriptObjectImpl::_get(const StringName& p_name, Variant& r_ret)
    const {
    ERR_FAIL_COND_V_MSG(!_js_id, false, "Invalid JS instance");
    const String name = p_name;
    rebel_js_wrapper_ex exchange;
    int type =
        rebel_js_wrapper_object_get(_js_id, &exchange, name.utf8().get_data());
    r_ret = _js2variant(type, &exchange);
    return true;
}

Variant JavaScriptObjectImpl::getvar(const Variant& p_key, bool* r_valid)
    const {
    if (r_valid) {
        *r_valid = false;
    }
    rebel_js_wrapper_ex exchange;
    void* lock       = nullptr;
    const Variant* v = &p_key;
    int prop_type    = _variant2js((const void**)&v, 0, &exchange, &lock);
    int type = rebel_js_wrapper_object_getvar(_js_id, prop_type, &exchange);
    if (lock) {
        _free_lock(&lock, prop_type);
    }
    if (type < 0) {
        return Variant();
    }
    if (r_valid) {
        *r_valid = true;
    }
    return _js2variant(type, &exchange);
}

void JavaScriptObjectImpl::setvar(
    const Variant& p_key,
    const Variant& p_value,
    bool* r_valid
) {
    if (r_valid) {
        *r_valid = false;
    }
    rebel_js_wrapper_ex kex, vex;
    void* klock       = nullptr;
    void* vlock       = nullptr;
    const Variant* kv = &p_key;
    const Variant* vv = &p_value;
    int ktype         = _variant2js((const void**)&kv, 0, &kex, &klock);
    int vtype         = _variant2js((const void**)&vv, 0, &vex, &vlock);
    int ret = rebel_js_wrapper_object_setvar(_js_id, ktype, &kex, vtype, &vex);
    if (klock) {
        _free_lock(&klock, ktype);
    }
    if (vlock) {
        _free_lock(&vlock, vtype);
    }
    if (ret == 0 && r_valid) {
        *r_valid = true;
    }
}

void JavaScriptObjectImpl::_get_property_list(List<PropertyInfo>* p_list
) const {}

void JavaScriptObjectImpl::_free_lock(void** p_lock, int p_type) {
    ERR_FAIL_COND_MSG(*p_lock == nullptr, "No lock to free!");
    const Variant::Type type = (Variant::Type)p_type;
    switch (type) {
        case Variant::STRING: {
            CharString* cs = (CharString*)(*p_lock);
            memdelete(cs);
            *p_lock = nullptr;
        } break;
        default:
            ERR_FAIL_MSG("Unknown lock type to free. Likely a bug.");
    }
}

Variant JavaScriptObjectImpl::_js2variant(
    int p_type,
    rebel_js_wrapper_ex* p_val
) {
    Variant::Type type = (Variant::Type)p_type;
    switch (type) {
        case Variant::BOOL:
            return Variant((bool)p_val->i);
        case Variant::INT:
            return p_val->i;
        case Variant::REAL:
            return p_val->r;
        case Variant::STRING: {
            String out = String::utf8((const char*)p_val->p);
            free(p_val->p);
            return out;
        }
        case Variant::OBJECT: {
            return memnew(JavaScriptObjectImpl(p_val->i));
        }
        default:
            return Variant();
    }
}

int JavaScriptObjectImpl::_variant2js(
    const void** p_args,
    int p_pos,
    rebel_js_wrapper_ex* r_val,
    void** p_lock
) {
    const Variant** args = (const Variant**)p_args;
    const Variant* v     = args[p_pos];
    Variant::Type type   = v->get_type();
    switch (type) {
        case Variant::BOOL:
            r_val->i = v->operator bool() ? 1 : 0;
            break;
        case Variant::INT: {
            const int64_t tmp = v->operator int64_t();
            if (tmp >= 1 << 31) {
                r_val->r = (double)tmp;
                return Variant::REAL;
            }
            r_val->i = v->operator int64_t();
        } break;
        case Variant::REAL:
            r_val->r = v->operator real_t();
            break;
        case Variant::STRING: {
            CharString* cs = memnew(CharString(v->operator String().utf8()));
            r_val->p       = (void*)cs->get_data();
            *p_lock        = (void*)cs;
        } break;
        case Variant::OBJECT: {
            JavaScriptObject* js_obj =
                Object::cast_to<JavaScriptObject>(v->operator Object*());
            r_val->i =
                js_obj != nullptr ? ((JavaScriptObjectImpl*)js_obj)->_js_id : 0;
        } break;
        default:
            break;
    }
    return type;
}

Variant JavaScriptObjectImpl::call(
    const StringName& p_method,
    const Variant** p_args,
    int p_argc,
    Variant::CallError& r_error
) {
    rebel_js_wrapper_ex exchange;
    const String method = p_method;
    void* lock          = nullptr;
    const int type      = rebel_js_wrapper_object_call(
        _js_id,
        method.utf8().get_data(),
        (void**)p_args,
        p_argc,
        &_variant2js,
        &exchange,
        &lock,
        &_free_lock
    );
    r_error.error = Variant::CallError::CALL_OK;
    if (type < 0) {
        r_error.error = Variant::CallError::CALL_ERROR_INSTANCE_IS_NULL;
        return Variant();
    }
    return _js2variant(type, &exchange);
}

void JavaScriptObjectImpl::_callback(void* p_ref, int p_args_id, int p_argc) {
    const JavaScriptObjectImpl* obj = (JavaScriptObjectImpl*)p_ref;
    ERR_FAIL_COND_MSG(obj->_ref == nullptr, "JavaScript callback failed.");
    Vector<const Variant*> argp;
    Array arg_arr;
    for (int i = 0; i < p_argc; i++) {
        rebel_js_wrapper_ex exchange;
        exchange.i = i;
        int type =
            rebel_js_wrapper_object_getvar(p_args_id, Variant::INT, &exchange);
        arg_arr.push_back(_js2variant(type, &exchange));
    }
    Variant arg            = arg_arr;
    const Variant* argv[1] = {&arg};
    Variant::CallError err;
    obj->_ref->call(obj->_method, argv, 1, err);
}

Ref<JavaScriptObject> JavaScript::create_callback(
    Object* p_ref,
    const StringName& p_method
) {
    Ref<JavaScriptObjectImpl> out = memnew(JavaScriptObjectImpl);
    out->_ref                     = p_ref;
    out->_method                  = p_method;
    out->_js_id =
        rebel_js_wrapper_create_cb(out.ptr(), JavaScriptObjectImpl::_callback);
    return out;
}

Ref<JavaScriptObject> JavaScript::get_interface(const String& p_interface) {
    int js_id = rebel_js_wrapper_interface_get(p_interface.utf8().get_data());
    ERR_FAIL_COND_V_MSG(
        !js_id,
        Ref<JavaScriptObject>(),
        "No interface '" + p_interface + "' registered."
    );
    return Ref<JavaScriptObject>(memnew(JavaScriptObjectImpl(js_id)));
}

Variant JavaScript::_create_object_bind(
    const Variant** p_args,
    int p_argcount,
    Variant::CallError& r_error
) {
    if (p_argcount < 1) {
        r_error.error    = Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
        r_error.argument = 0;
        return Ref<JavaScriptObject>();
    }
    if (p_args[0]->get_type() != Variant::STRING) {
        r_error.error    = Variant::CallError::CALL_ERROR_INVALID_ARGUMENT;
        r_error.argument = 0;
        r_error.expected = Variant::STRING;
        return Ref<JavaScriptObject>();
    }
    rebel_js_wrapper_ex exchange;
    const String object  = *p_args[0];
    void* lock           = nullptr;
    const Variant** args = p_argcount > 1 ? &p_args[1] : nullptr;
    const int type       = rebel_js_wrapper_create_object(
        object.utf8().get_data(),
        (void**)args,
        p_argcount - 1,
        &JavaScriptObjectImpl::_variant2js,
        &exchange,
        &lock,
        &JavaScriptObjectImpl::_free_lock
    );
    r_error.error = Variant::CallError::CALL_OK;
    if (type < 0) {
        r_error.error = Variant::CallError::CALL_ERROR_INSTANCE_IS_NULL;
        return Ref<JavaScriptObject>();
    }
    return JavaScriptObjectImpl::_js2variant(type, &exchange);
}

extern "C" {
union js_eval_ret {
    uint32_t b;
    double d;
    char* s;
};

extern int rebel_js_eval(
    const char* p_js,
    int p_use_global_ctx,
    union js_eval_ret* p_union_ptr,
    void* p_byte_arr,
    void* p_byte_arr_write,
    void* (*p_callback)(void* p_ptr, void* p_ptr2, int p_len)
);
}

void* resize_poolbytearray_and_open_write(
    void* p_arr,
    void* r_write,
    int p_len
) {
    PoolByteArray* arr          = (PoolByteArray*)p_arr;
    PoolByteArray::Write* write = (PoolByteArray::Write*)r_write;
    arr->resize(p_len);
    *write = arr->write();
    return write->ptr();
}

Variant JavaScript::eval(const String& p_code, bool p_use_global_exec_context) {
    PoolByteArray arr;
    PoolByteArray::Write arr_write;
    union js_eval_ret js_data;
    memset(&js_data, 0, sizeof(js_data));
    Variant::Type return_type = static_cast<Variant::Type>(rebel_js_eval(
        p_code.utf8().get_data(),
        p_use_global_exec_context,
        &js_data,
        &arr,
        &arr_write,
        resize_poolbytearray_and_open_write
    ));

    switch (return_type) {
        case Variant::BOOL:
            return js_data.b == 1;
        case Variant::REAL:
            return js_data.d;
        case Variant::STRING: {
            String str = String::utf8(js_data.s);
            free(js_data.s); // Must free the string allocated in JS.
            return str;
        }
        case Variant::POOL_BYTE_ARRAY:
            arr_write = PoolByteArray::Write();
            return arr;
        default:
            return Variant();
    }
}

#endif // WEB_EVAL_ENABLED

void JavaScript::download_buffer(
    Vector<uint8_t> p_arr,
    const String& p_name,
    const String& p_mime
) {
    rebel_js_os_download_buffer(
        p_arr.ptr(),
        p_arr.size(),
        p_name.utf8().get_data(),
        p_mime.utf8().get_data()
    );
}
