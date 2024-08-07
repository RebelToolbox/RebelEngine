// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef OBJECT_H
#define OBJECT_H

#include "core/class_db.h"
#include "core/hash_map.h"
#include "core/list.h"
#include "core/map.h"
#include "core/object_id.h"
#include "core/os/rw_lock.h"
#include "core/safe_refcount.h"
#include "core/set.h"
#include "core/variant.h"
#include "core/vmap.h"

#include <atomic>

#define VARIANT_ARG_LIST                                                       \
    const Variant &p_arg1 = Variant(), const Variant &p_arg2 = Variant(),      \
                  const Variant &p_arg3 = Variant(),                           \
                  const Variant &p_arg4 = Variant(),                           \
                  const Variant &p_arg5 = Variant()
#define VARIANT_ARG_PASS p_arg1, p_arg2, p_arg3, p_arg4, p_arg5
#define VARIANT_ARG_DECLARE                                                    \
    const Variant &p_arg1, const Variant &p_arg2, const Variant &p_arg3,       \
        const Variant &p_arg4, const Variant &p_arg5
#define VARIANT_ARG_MAX 5
#define VARIANT_ARGPTRS                                                        \
    const Variant* argptr[5] = {&p_arg1, &p_arg2, &p_arg3, &p_arg4, &p_arg5};
#define VARIANT_ARGPTRS_PASS                                                   \
    *argptr[0], *argptr[1], *argptr[2], *argptr[3], *argptr[4]
#define VARIANT_ARGS_FROM_ARRAY(m_arr)                                         \
    m_arr[0], m_arr[1], m_arr[2], m_arr[3], m_arr[4]

/**
@author Juan Linietsky <reduzio@gmail.com>
*/

#define ADD_SIGNAL(m_signal) ClassDB::add_signal(get_class_static(), m_signal)
#define ADD_PROPERTY(m_property, m_setter, m_getter)                           \
    ClassDB::add_property(                                                     \
        get_class_static(),                                                    \
        m_property,                                                            \
        _scs_create(m_setter),                                                 \
        _scs_create(m_getter)                                                  \
    )
#define ADD_PROPERTYI(m_property, m_setter, m_getter, m_index)                 \
    ClassDB::add_property(                                                     \
        get_class_static(),                                                    \
        m_property,                                                            \
        _scs_create(m_setter),                                                 \
        _scs_create(m_getter),                                                 \
        m_index                                                                \
    )
#define ADD_PROPERTY_DEFAULT(m_property, m_default)                            \
    ClassDB::set_property_default_value(                                       \
        get_class_static(),                                                    \
        m_property,                                                            \
        m_default                                                              \
    )
#define ADD_GROUP(m_name, m_prefix)                                            \
    ClassDB::add_property_group(get_class_static(), m_name, m_prefix)

Array convert_property_list(const List<PropertyInfo>* p_list);

/*
   the following is an incomprehensible blob of hacks and workarounds to
   compensate for many of the fallencies in C++. As a plus, this macro pretty
   much alone defines the object model.
*/

#define REVERSE_GET_PROPERTY_LIST                                              \
                                                                               \
public:                                                                        \
    _FORCE_INLINE_ bool _is_gpl_reversed() const {                             \
        return true;                                                           \
    };                                                                         \
                                                                               \
private:

#define UNREVERSE_GET_PROPERTY_LIST                                            \
                                                                               \
public:                                                                        \
    _FORCE_INLINE_ bool _is_gpl_reversed() const {                             \
        return false;                                                          \
    };                                                                         \
                                                                               \
private:

#define GDCLASS(m_class, m_inherits)                                           \
                                                                               \
private:                                                                       \
    void operator=(const m_class& p_rval) {}                                   \
    mutable StringName _class_name;                                            \
    friend class ClassDB;                                                      \
                                                                               \
public:                                                                        \
    virtual String get_class() const {                                         \
        return String(#m_class);                                               \
    }                                                                          \
    virtual const StringName* _get_class_namev() const {                       \
        if (!_class_name) _class_name = get_class_static();                    \
        return &_class_name;                                                   \
    }                                                                          \
    static _FORCE_INLINE_ void* get_class_ptr_static() {                       \
        static int ptr;                                                        \
        return &ptr;                                                           \
    }                                                                          \
    static _FORCE_INLINE_ String get_class_static() {                          \
        return String(#m_class);                                               \
    }                                                                          \
    static _FORCE_INLINE_ String get_parent_class_static() {                   \
        return m_inherits::get_class_static();                                 \
    }                                                                          \
    static void get_inheritance_list_static(List<String>* p_inheritance_list   \
    ) {                                                                        \
        m_inherits::get_inheritance_list_static(p_inheritance_list);           \
        p_inheritance_list->push_back(String(#m_class));                       \
    }                                                                          \
    static String get_category_static() {                                      \
        String category = m_inherits::get_category_static();                   \
        if (_get_category != m_inherits::_get_category) {                      \
            if (category != "") category += "/";                               \
            category += _get_category();                                       \
        }                                                                      \
        return category;                                                       \
    }                                                                          \
    static String inherits_static() {                                          \
        return String(#m_inherits);                                            \
    }                                                                          \
    virtual bool is_class(const String& p_class) const {                       \
        return (p_class == (#m_class)) ? true : m_inherits::is_class(p_class); \
    }                                                                          \
    virtual bool is_class_ptr(void* p_ptr) const {                             \
        return (p_ptr == get_class_ptr_static())                               \
                 ? true                                                        \
                 : m_inherits::is_class_ptr(p_ptr);                            \
    }                                                                          \
                                                                               \
    static void get_valid_parents_static(List<String>* p_parents) {            \
        if (m_class::_get_valid_parents_static                                 \
            != m_inherits::_get_valid_parents_static) {                        \
            m_class::_get_valid_parents_static(p_parents);                     \
        }                                                                      \
                                                                               \
        m_inherits::get_valid_parents_static(p_parents);                       \
    }                                                                          \
                                                                               \
protected:                                                                     \
    _FORCE_INLINE_ static void (*_get_bind_methods())() {                      \
        return &m_class::_bind_methods;                                        \
    }                                                                          \
                                                                               \
public:                                                                        \
    static void initialize_class() {                                           \
        static bool initialized = false;                                       \
        if (initialized) return;                                               \
        m_inherits::initialize_class();                                        \
        ClassDB::_add_class<m_class>();                                        \
        if (m_class::_get_bind_methods() != m_inherits::_get_bind_methods())   \
            _bind_methods();                                                   \
        initialized = true;                                                    \
    }                                                                          \
                                                                               \
protected:                                                                     \
    virtual void _initialize_classv() {                                        \
        initialize_class();                                                    \
    }                                                                          \
    _FORCE_INLINE_ bool (Object::*_get_get(                                    \
    ) const)(const StringName& p_name, Variant&) const {                       \
        return (bool(Object::*)(const StringName&, Variant&) const)            \
             & m_class::_get;                                                  \
    }                                                                          \
    virtual bool _getv(const StringName& p_name, Variant& r_ret) const {       \
        if (m_class::_get_get() != m_inherits::_get_get()) {                   \
            if (_get(p_name, r_ret)) return true;                              \
        }                                                                      \
        return m_inherits::_getv(p_name, r_ret);                               \
    }                                                                          \
    _FORCE_INLINE_ bool (Object::*_get_set(                                    \
    ) const)(const StringName& p_name, const Variant& p_property) {            \
        return (bool(Object::*)(const StringName&, const Variant&))            \
             & m_class::_set;                                                  \
    }                                                                          \
    virtual bool _setv(const StringName& p_name, const Variant& p_property) {  \
        if (m_inherits::_setv(p_name, p_property)) return true;                \
        if (m_class::_get_set() != m_inherits::_get_set()) {                   \
            return _set(p_name, p_property);                                   \
        }                                                                      \
        return false;                                                          \
    }                                                                          \
    _FORCE_INLINE_ void (Object::*_get_get_property_list()                     \
                             const)(List<PropertyInfo> * p_list) const {       \
        return (void(Object::*)(List<PropertyInfo>*) const)                    \
             & m_class::_get_property_list;                                    \
    }                                                                          \
    virtual void _get_property_listv(                                          \
        List<PropertyInfo>* p_list,                                            \
        bool p_reversed                                                        \
    ) const {                                                                  \
        if (!p_reversed) {                                                     \
            m_inherits::_get_property_listv(p_list, p_reversed);               \
        }                                                                      \
        p_list->push_back(PropertyInfo(                                        \
            Variant::NIL,                                                      \
            get_class_static(),                                                \
            PROPERTY_HINT_NONE,                                                \
            String(),                                                          \
            PROPERTY_USAGE_CATEGORY                                            \
        ));                                                                    \
        if (!_is_gpl_reversed())                                               \
            ClassDB::get_property_list(#m_class, p_list, true, this);          \
        if (m_class::_get_get_property_list()                                  \
            != m_inherits::_get_get_property_list()) {                         \
            _get_property_list(p_list);                                        \
        }                                                                      \
        if (_is_gpl_reversed())                                                \
            ClassDB::get_property_list(#m_class, p_list, true, this);          \
        if (p_reversed) {                                                      \
            m_inherits::_get_property_listv(p_list, p_reversed);               \
        }                                                                      \
    }                                                                          \
    _FORCE_INLINE_ void (Object::*_get_notification() const)(int) {            \
        return (void(Object::*)(int)) & m_class::_notification;                \
    }                                                                          \
    virtual void _notificationv(int p_notification, bool p_reversed) {         \
        if (!p_reversed)                                                       \
            m_inherits::_notificationv(p_notification, p_reversed);            \
        if (m_class::_get_notification() != m_inherits::_get_notification()) { \
            _notification(p_notification);                                     \
        }                                                                      \
        if (p_reversed)                                                        \
            m_inherits::_notificationv(p_notification, p_reversed);            \
    }                                                                          \
                                                                               \
private:

#define OBJ_CATEGORY(m_category)                                               \
                                                                               \
protected:                                                                     \
    _FORCE_INLINE_ static String _get_category() {                             \
        return m_category;                                                     \
    }                                                                          \
                                                                               \
private:

#define OBJ_SAVE_TYPE(m_class)                                                 \
                                                                               \
public:                                                                        \
    virtual String get_save_class() const {                                    \
        return #m_class;                                                       \
    }                                                                          \
                                                                               \
private:

class ScriptInstance;
class ObjectRC;

class Object {
public:
    enum ConnectFlags {
        CONNECT_DEFERRED          = 1,
        CONNECT_PERSIST           = 2, // hint for scene to save this connection
        CONNECT_ONESHOT           = 4,
        CONNECT_REFERENCE_COUNTED = 8,
    };

    struct Connection {
        Object* source;
        StringName signal;
        Object* target;
        StringName method;
        uint32_t flags;
        Vector<Variant> binds;
        bool operator<(const Connection& p_conn) const;

        operator Variant() const;

        Connection() {
            source = nullptr;
            target = nullptr;
            flags  = 0;
        }

        Connection(const Variant& p_variant);
    };

private:
    enum {
        MAX_SCRIPT_INSTANCE_BINDINGS = 8
    };

#ifdef DEBUG_ENABLED
    friend struct _ObjectDebugLock;
#endif
    friend bool predelete_handler(Object*);
    friend void postinitialize_handler(Object*);

    struct Signal {
        struct Target {
            ObjectID _id;
            StringName method;

            _FORCE_INLINE_ bool operator<(const Target& p_target) const {
                return (_id == p_target._id) ? (method < p_target.method)
                                             : (_id < p_target._id);
            }

            Target(const ObjectID& p_id, const StringName& p_method) :
                _id(p_id),
                method(p_method) {}

            Target() {
                _id = 0;
            }
        };

        struct Slot {
            int reference_count;
            Connection conn;
            List<Connection>::Element* cE;

            Slot() {
                reference_count = 0;
            }
        };

        MethodInfo user;
        VMap<Target, Slot> slot_map;

        Signal() {}
    };

    HashMap<StringName, Signal> signal_map;
    List<Connection> connections;
#ifdef DEBUG_ENABLED
    SafeRefCount _lock_index;
#endif
    bool _block_signals;
    int _predelete_ok;
    Set<Object*> change_receptors;
    ObjectID _instance_id;
    std::atomic<ObjectRC*> _rc;
    bool _predelete();
    void _postinitialize();
    bool _can_translate;
    bool _emitting;
#ifdef TOOLS_ENABLED
    bool _edited;
    uint32_t _edited_version;
    Set<String> editor_section_folding;
#endif
    ScriptInstance* script_instance;
    RefPtr script;
    Dictionary metadata;
    mutable StringName _class_name;
    mutable const StringName* _class_ptr;

    void _add_user_signal(const String& p_name, const Array& p_args = Array());
    bool _has_user_signal(const StringName& p_name) const;
    Variant _emit_signal(
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );
    Array _get_signal_list() const;
    Array _get_signal_connection_list(const String& p_signal) const;
    Array _get_incoming_connections() const;
    void _set_bind(const String& p_set, const Variant& p_value);
    Variant _get_bind(const String& p_name) const;
    void _set_indexed_bind(const NodePath& p_name, const Variant& p_value);
    Variant _get_indexed_bind(const NodePath& p_name) const;

    friend class Reference;
    SafeNumeric<uint32_t> instance_binding_count;
    void* _script_instance_bindings[MAX_SCRIPT_INSTANCE_BINDINGS];

protected:
    virtual void _initialize_classv() {
        initialize_class();
    }

    virtual bool _setv(const StringName& p_name, const Variant& p_property) {
        return false;
    };

    virtual bool _getv(const StringName& p_name, Variant& r_property) const {
        return false;
    };

    virtual void _get_property_listv(
        List<PropertyInfo>* p_list,
        bool p_reversed
    ) const {};
    virtual void _notificationv(int p_notification, bool p_reversed){};

    static String _get_category() {
        return "";
    }

    static void _bind_methods();

    bool _set(const StringName& p_name, const Variant& p_property) {
        return false;
    };

    bool _get(const StringName& p_name, Variant& r_property) const {
        return false;
    };

    void _get_property_list(List<PropertyInfo>* p_list) const {};
    void _notification(int p_notification){};

    _FORCE_INLINE_ static void (*_get_bind_methods())() {
        return &Object::_bind_methods;
    }

    _FORCE_INLINE_ bool (Object::*_get_get(
    ) const)(const StringName& p_name, Variant& r_ret) const {
        return &Object::_get;
    }

    _FORCE_INLINE_ bool (Object::*_get_set(
    ) const)(const StringName& p_name, const Variant& p_property) {
        return &Object::_set;
    }

    _FORCE_INLINE_ void (Object::*_get_get_property_list()
                             const)(List<PropertyInfo>* p_list) const {
        return &Object::_get_property_list;
    }

    _FORCE_INLINE_ void (Object::*_get_notification() const)(int) {
        return &Object::_notification;
    }

    static void get_valid_parents_static(List<String>* p_parents);
    static void _get_valid_parents_static(List<String>* p_parents);

    void cancel_delete();

    void property_list_changed_notify();
    virtual void _changed_callback(Object* p_changed, const char* p_prop);

    // Variant _call_bind(const StringName& p_name, const Variant& p_arg1 =
    // Variant(), const Variant& p_arg2 = Variant(), const Variant& p_arg3 =
    // Variant(), const Variant& p_arg4 = Variant()); void
    // _call_deferred_bind(const StringName& p_name, const Variant& p_arg1 =
    // Variant(), const Variant& p_arg2 = Variant(), const Variant& p_arg3 =
    // Variant(), const Variant& p_arg4 = Variant());

    Variant _call_bind(
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );
    Variant _call_deferred_bind(
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );

    virtual const StringName* _get_class_namev() const {
        if (!_class_name) {
            _class_name = get_class_static();
        }
        return &_class_name;
    }

    PoolVector<String> _get_meta_list_bind() const;
    Array _get_property_list_bind() const;
    Array _get_method_list_bind() const;

    void _clear_internal_resource_paths(const Variant& p_var);

    friend class ClassDB;
    virtual void _validate_property(PropertyInfo& property) const;

    void _disconnect(
        const StringName& p_signal,
        Object* p_to_object,
        const StringName& p_to_method,
        bool p_force = false
    );

public: // should be protected, but bug in clang++
    static void initialize_class();
    _FORCE_INLINE_ static void register_custom_data_to_otdb(){};

public:
#ifdef TOOLS_ENABLED
    _FORCE_INLINE_ void _change_notify(const char* p_property = "") {
        _edited = true;
        for (Set<Object*>::Element* E = change_receptors.front(); E;
             E                        = E->next()) {
            ((Object*)(E->get()))->_changed_callback(this, p_property);
        }
    }
#else
    _FORCE_INLINE_ void _change_notify(const char* p_what = "") {}
#endif
    static void* get_class_ptr_static() {
        static int ptr;
        return &ptr;
    }

    ObjectRC* _use_rc();

    bool _is_gpl_reversed() const {
        return false;
    }

    _FORCE_INLINE_ ObjectID get_instance_id() const {
        return _instance_id;
    }

    // this is used for editors

    void add_change_receptor(Object* p_receptor);
    void remove_change_receptor(Object* p_receptor);

    template <class T>
    static T* cast_to(Object* p_object) {
#ifndef NO_SAFE_CAST
        return dynamic_cast<T*>(p_object);
#else
        if (!p_object) {
            return NULL;
        }
        if (p_object->is_class_ptr(T::get_class_ptr_static())) {
            return static_cast<T*>(p_object);
        } else {
            return NULL;
        }
#endif
    }

    template <class T>
    static const T* cast_to(const Object* p_object) {
#ifndef NO_SAFE_CAST
        return dynamic_cast<const T*>(p_object);
#else
        if (!p_object) {
            return NULL;
        }
        if (p_object->is_class_ptr(T::get_class_ptr_static())) {
            return static_cast<const T*>(p_object);
        } else {
            return NULL;
        }
#endif
    }

    enum {
        NOTIFICATION_POSTINITIALIZE = 0,
        NOTIFICATION_PREDELETE      = 1
    };

    /* TYPE API */
    static void get_inheritance_list_static(List<String>* p_inheritance_list) {
        p_inheritance_list->push_back("Object");
    }

    static String get_class_static() {
        return "Object";
    }

    static String get_parent_class_static() {
        return String();
    }

    static String get_category_static() {
        return String();
    }

    virtual String get_class() const {
        return "Object";
    }

    virtual String get_save_class() const {
        return get_class();
    } // class stored when saving

    virtual bool is_class(const String& p_class) const {
        return (p_class == "Object");
    }

    virtual bool is_class_ptr(void* p_ptr) const {
        return get_class_ptr_static() == p_ptr;
    }

    _FORCE_INLINE_ const StringName& get_class_name() const {
        if (!_class_ptr) {
            return *_get_class_namev();
        } else {
            return *_class_ptr;
        }
    }

    /* IAPI */
    // void set(const String& p_name, const Variant& p_value);
    // Variant get(const String& p_name) const;

    void set(
        const StringName& p_name,
        const Variant& p_value,
        bool* r_valid = nullptr
    );
    Variant get(const StringName& p_name, bool* r_valid = nullptr) const;
    void set_indexed(
        const Vector<StringName>& p_names,
        const Variant& p_value,
        bool* r_valid = nullptr
    );
    Variant get_indexed(
        const Vector<StringName>& p_names,
        bool* r_valid = nullptr
    ) const;

    void get_property_list(List<PropertyInfo>* p_list, bool p_reversed = false)
        const;

    bool has_method(const StringName& p_method) const;
    void get_method_list(List<MethodInfo>* p_list) const;
    Variant callv(const StringName& p_method, const Array& p_args);
    virtual Variant call(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );
    virtual void call_multilevel(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount
    );
    virtual void call_multilevel_reversed(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount
    );
    Variant call(const StringName& p_name, VARIANT_ARG_LIST); // C++ helper
    void call_multilevel(
        const StringName& p_name,
        VARIANT_ARG_LIST
    ); // C++ helper

    void notification(int p_notification, bool p_reversed = false);

    virtual void notification_callback(int p_message_type) {}

    virtual String to_string();

    // used mainly by script, get and set all INCLUDING string
    virtual Variant getvar(const Variant& p_key, bool* r_valid = nullptr) const;
    virtual void setvar(
        const Variant& p_key,
        const Variant& p_value,
        bool* r_valid = nullptr
    );

    /* SCRIPT */

    void set_script(const RefPtr& p_script);
    RefPtr get_script() const;

    /* SCRIPT */

    bool has_meta(const String& p_name) const;
    void set_meta(const String& p_name, const Variant& p_value);
    void remove_meta(const String& p_name);
    Variant get_meta(const String& p_name) const;
    void get_meta_list(List<String>* p_list) const;

#ifdef TOOLS_ENABLED
    void set_edited(bool p_edited);
    bool is_edited() const;
    uint32_t get_edited_version(
    ) const; // this function is used to check when something changed beyond a
             // point, it's used mainly for generating previews
#endif

    void set_script_instance(ScriptInstance* p_instance);

    _FORCE_INLINE_ ScriptInstance* get_script_instance() const {
        return script_instance;
    }

    void set_script_and_instance(
        const RefPtr& p_script,
        ScriptInstance* p_instance
    ); // some script languages can't control instance creation, so this
       // function eases the process

    void add_user_signal(const MethodInfo& p_signal);
    Error emit_signal(const StringName& p_name, VARIANT_ARG_LIST);
    Error emit_signal(
        const StringName& p_name,
        const Variant** p_args,
        int p_argcount
    );
    bool has_signal(const StringName& p_name) const;
    void get_signal_list(List<MethodInfo>* p_signals) const;
    void get_signal_connection_list(
        const StringName& p_signal,
        List<Connection>* p_connections
    ) const;
    void get_all_signal_connections(List<Connection>* p_connections) const;
    int get_persistent_signal_connection_count() const;
    void get_signals_connected_to_this(List<Connection>* p_connections) const;

    Error connect(
        const StringName& p_signal,
        Object* p_to_object,
        const StringName& p_to_method,
        const Vector<Variant>& p_binds = Vector<Variant>(),
        uint32_t p_flags               = 0
    );
    void disconnect(
        const StringName& p_signal,
        Object* p_to_object,
        const StringName& p_to_method
    );
    bool is_connected(
        const StringName& p_signal,
        Object* p_to_object,
        const StringName& p_to_method
    ) const;

    void call_deferred(const StringName& p_method, VARIANT_ARG_LIST);
    void set_deferred(const StringName& p_property, const Variant& p_value);

    void set_block_signals(bool p_block);
    bool is_blocking_signals() const;

    Variant::Type get_static_property_type(
        const StringName& p_property,
        bool* r_valid = nullptr
    ) const;
    Variant::Type get_static_property_type_indexed(
        const Vector<StringName>& p_path,
        bool* r_valid = nullptr
    ) const;

    virtual void get_translatable_strings(List<String>* p_strings) const;

    virtual void get_argument_options(
        const StringName& p_function,
        int p_idx,
        List<String>* r_options
    ) const;

    StringName tr(const StringName& p_message
    ) const; // translate message (internationalization)

    bool _is_queued_for_deletion; // set to true by SceneTree::queue_delete()
    bool is_queued_for_deletion() const;

    _FORCE_INLINE_ void set_message_translation(bool p_enable) {
        _can_translate = p_enable;
    }

    _FORCE_INLINE_ bool can_translate_messages() const {
        return _can_translate;
    }

#ifdef TOOLS_ENABLED
    void editor_set_section_unfold(const String& p_section, bool p_unfolded);
    bool editor_is_section_unfolded(const String& p_section);

    const Set<String>& editor_get_section_folding() const {
        return editor_section_folding;
    }

    void editor_clear_section_folding() {
        editor_section_folding.clear();
    }

#endif

    // used by script languages to store binding data
    void* get_script_instance_binding(int p_script_language_index);
    bool has_script_instance_binding(int p_script_language_index);
    void set_script_instance_binding(int p_script_language_index, void* p_data);

    void clear_internal_resource_paths();

    Object();
    virtual ~Object();
};

bool predelete_handler(Object* p_object);
void postinitialize_handler(Object* p_object);

class ObjectDB {
    struct ObjectPtrHash {
        static _FORCE_INLINE_ uint32_t hash(const Object* p_obj) {
            union {
                const Object* p;
                unsigned long i;
            } u;

            u.p = p_obj;
            return HashMapHasherDefault::hash((uint64_t)u.i);
        }
    };

    static HashMap<ObjectID, Object*> instances;
    static HashMap<Object*, ObjectID, ObjectPtrHash> instance_checks;

    static ObjectID instance_counter;
    friend class Object;
    friend void unregister_core_types();

    static RWLock rw_lock;
    static void cleanup();
    static ObjectID add_instance(Object* p_object);
    static void remove_instance(Object* p_object);
    friend void register_core_types();

public:
    typedef void (*DebugFunc)(Object* p_obj);

    static Object* get_instance(ObjectID p_instance_id);
    static void debug_objects(DebugFunc p_func);
    static int get_object_count();

    // This one may give false positives because a new object may be allocated
    // at the same memory of a previously freed one
    _FORCE_INLINE_ static bool instance_validate(Object* p_ptr) {
        rw_lock.read_lock();

        bool exists = instance_checks.has(p_ptr);

        rw_lock.read_unlock();

        return exists;
    }
};

#endif
