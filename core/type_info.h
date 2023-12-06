// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GET_TYPE_INFO_H
#define GET_TYPE_INFO_H

enum PropertyHint {
    PROPERTY_HINT_NONE,       ///< no hint provided.
    PROPERTY_HINT_RANGE,      ///< hint_text = "min,max,step,slider; //slider is
                              ///< optional"
    PROPERTY_HINT_EXP_RANGE,  ///< hint_text = "min,max,step", exponential edit
    PROPERTY_HINT_ENUM,       ///< hint_text= "val1,val2,val3,etc"
    PROPERTY_HINT_EXP_EASING, /// exponential easing function (Math::ease) use
                              /// "attenuation" hint string to revert (flip h),
                              /// "full" to also include in/out. (ie:
                              /// "attenuation,inout")
    PROPERTY_HINT_LENGTH,     ///< hint_text= "length" (as integer)
    PROPERTY_HINT_SPRITE_FRAME, // FIXME: Obsolete: drop whenever we can break
                                // compat. Keeping now for GDNative compat.
    PROPERTY_HINT_KEY_ACCEL,    ///< hint_text= "length" (as integer)
    PROPERTY_HINT_FLAGS,        ///< hint_text= "flag1,flag2,etc" (as bit flags)
    PROPERTY_HINT_LAYERS_2D_RENDER,
    PROPERTY_HINT_LAYERS_2D_PHYSICS,
    PROPERTY_HINT_LAYERS_3D_RENDER,
    PROPERTY_HINT_LAYERS_3D_PHYSICS,
    PROPERTY_HINT_FILE, ///< a file path must be passed, hint_text (optionally)
                        ///< is a filter "*.png,*.wav,*.doc,"
    PROPERTY_HINT_DIR,  ///< a directory path must be passed
    PROPERTY_HINT_GLOBAL_FILE,      ///< a file path must be passed, hint_text
                                    ///< (optionally) is a filter
                                    ///< "*.png,*.wav,*.doc,"
    PROPERTY_HINT_GLOBAL_DIR,       ///< a directory path must be passed
    PROPERTY_HINT_RESOURCE_TYPE,    ///< a resource object type
    PROPERTY_HINT_MULTILINE_TEXT,   ///< used for string properties that can
                                    ///< contain multiple lines
    PROPERTY_HINT_PLACEHOLDER_TEXT, ///< used to set a placeholder text for
                                    ///< string properties
    PROPERTY_HINT_COLOR_NO_ALPHA,   ///< used for ignoring alpha component when
                                    ///< editing a color
    PROPERTY_HINT_IMAGE_COMPRESS_LOSSY,
    PROPERTY_HINT_IMAGE_COMPRESS_LOSSLESS,
    PROPERTY_HINT_OBJECT_ID,
    PROPERTY_HINT_TYPE_STRING, ///< a type string, the hint is the base type to
                               ///< choose
    PROPERTY_HINT_NODE_PATH_TO_EDITED_NODE, ///< so something else can provide
                                            ///< this (used in scripts)
    PROPERTY_HINT_METHOD_OF_VARIANT_TYPE,   ///< a method of a type
    PROPERTY_HINT_METHOD_OF_BASE_TYPE,      ///< a method of a base type
    PROPERTY_HINT_METHOD_OF_INSTANCE,       ///< a method of an instance
    PROPERTY_HINT_METHOD_OF_SCRIPT,         ///< a method of a script & base
    PROPERTY_HINT_PROPERTY_OF_VARIANT_TYPE, ///< a property of a type
    PROPERTY_HINT_PROPERTY_OF_BASE_TYPE,    ///< a property of a base type
    PROPERTY_HINT_PROPERTY_OF_INSTANCE,     ///< a property of an instance
    PROPERTY_HINT_PROPERTY_OF_SCRIPT,       ///< a property of a script & base
    PROPERTY_HINT_OBJECT_TOO_BIG,           ///< object is too big to send
    PROPERTY_HINT_NODE_PATH_VALID_TYPES,
    PROPERTY_HINT_SAVE_FILE, ///< a file path must be passed, hint_text
                             ///< (optionally) is a filter
                             ///< "*.png,*.wav,*.doc,". This opens a save
                             ///< dialog
    PROPERTY_HINT_MAX,
    // When updating PropertyHint, also sync the hardcoded list in
    // VisualScriptEditorVariableEdit
};

enum PropertyUsageFlags {
    PROPERTY_USAGE_STORAGE           = 1,
    PROPERTY_USAGE_EDITOR            = 2,
    PROPERTY_USAGE_NETWORK           = 4,
    PROPERTY_USAGE_EDITOR_HELPER     = 8,
    PROPERTY_USAGE_CHECKABLE         = 16, // used for editing global variables
    PROPERTY_USAGE_CHECKED           = 32, // used for editing global variables
    PROPERTY_USAGE_INTERNATIONALIZED = 64, // hint for internationalized
                                           // strings
    PROPERTY_USAGE_GROUP    = 128, // used for grouping props in the editor
    PROPERTY_USAGE_CATEGORY = 256,
    // FIXME: Drop in 4.0, possibly reorder other flags?
    // Those below are deprecated thanks to ClassDB's now class value cache
    // PROPERTY_USAGE_STORE_IF_NONZERO = 512, //only store if nonzero
    // PROPERTY_USAGE_STORE_IF_NONONE = 1024, //only store if false
    PROPERTY_USAGE_NO_INSTANCE_STATE      = 2048,
    PROPERTY_USAGE_RESTART_IF_CHANGED     = 4096,
    PROPERTY_USAGE_SCRIPT_VARIABLE        = 8192,
    PROPERTY_USAGE_STORE_IF_NULL          = 16384,
    PROPERTY_USAGE_ANIMATE_AS_TRIGGER     = 32768,
    PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED = 65536,
    PROPERTY_USAGE_SCRIPT_DEFAULT_VALUE   = 1 << 17,
    PROPERTY_USAGE_CLASS_IS_ENUM          = 1 << 18,
    PROPERTY_USAGE_NIL_IS_VARIANT         = 1 << 19,
    PROPERTY_USAGE_INTERNAL               = 1 << 20,
    PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE =
        1 << 21, // If the object is duplicated also this property will be
                 // duplicated
    PROPERTY_USAGE_HIGH_END_GFX              = 1 << 22,
    PROPERTY_USAGE_NODE_PATH_FROM_SCENE_ROOT = 1 << 23,
    PROPERTY_USAGE_RESOURCE_NOT_PERSISTENT   = 1 << 24,
    PROPERTY_USAGE_KEYING_INCREMENTS =
        1 << 25, // Used in inspector to increment property when keyed in
                 // animation player

    PROPERTY_USAGE_DEFAULT =
        PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_NETWORK,
    PROPERTY_USAGE_DEFAULT_INTL = PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_EDITOR
                                | PROPERTY_USAGE_NETWORK
                                | PROPERTY_USAGE_INTERNATIONALIZED,
    PROPERTY_USAGE_NOEDITOR = PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_NETWORK,
};

struct PropertyInfo {
    Variant::Type type;
    String name;
    StringName class_name; // for classes
    PropertyHint hint;
    String hint_string;
    uint32_t usage;

    _FORCE_INLINE_ PropertyInfo added_usage(int p_fl) const {
        PropertyInfo pi  = *this;
        pi.usage        |= p_fl;
        return pi;
    }

    operator Dictionary() const;

    static PropertyInfo from_dict(const Dictionary& p_dict);

    PropertyInfo() :
        type(Variant::NIL),
        hint(PROPERTY_HINT_NONE),
        usage(PROPERTY_USAGE_DEFAULT) {}

    PropertyInfo(
        Variant::Type p_type,
        const String p_name,
        PropertyHint p_hint            = PROPERTY_HINT_NONE,
        const String& p_hint_string    = "",
        uint32_t p_usage               = PROPERTY_USAGE_DEFAULT,
        const StringName& p_class_name = StringName()
    ) :
        type(p_type),
        name(p_name),
        hint(p_hint),
        hint_string(p_hint_string),
        usage(p_usage) {
        if (hint == PROPERTY_HINT_RESOURCE_TYPE) {
            class_name = hint_string;
        } else {
            class_name = p_class_name;
        }
    }

    PropertyInfo(const StringName& p_class_name) :
        type(Variant::OBJECT),
        class_name(p_class_name),
        hint(PROPERTY_HINT_NONE),
        usage(PROPERTY_USAGE_DEFAULT) {}

    bool operator==(const PropertyInfo& p_info) const {
        return (
            (type == p_info.type) && (name == p_info.name)
            && (class_name == p_info.class_name) && (hint == p_info.hint)
            && (hint_string == p_info.hint_string) && (usage == p_info.usage)
        );
    }

    bool operator<(const PropertyInfo& p_info) const {
        return name < p_info.name;
    }
};

struct MethodInfo {
    String name;
    PropertyInfo return_val;
    uint32_t flags;
    int id;
    List<PropertyInfo> arguments;
    Vector<Variant> default_arguments;

    inline bool operator==(const MethodInfo& p_method) const {
        return id == p_method.id;
    }

    inline bool operator<(const MethodInfo& p_method) const {
        return id == p_method.id ? (name < p_method.name) : (id < p_method.id);
    }

    operator Dictionary() const;

    static MethodInfo from_dict(const Dictionary& p_dict);
    MethodInfo();
    MethodInfo(const String& p_name);
    MethodInfo(const String& p_name, const PropertyInfo& p_param1);
    MethodInfo(
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2
    );
    MethodInfo(
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3
    );
    MethodInfo(
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3,
        const PropertyInfo& p_param4
    );
    MethodInfo(
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3,
        const PropertyInfo& p_param4,
        const PropertyInfo& p_param5
    );
    MethodInfo(Variant::Type ret);
    MethodInfo(Variant::Type ret, const String& p_name);
    MethodInfo(
        Variant::Type ret,
        const String& p_name,
        const PropertyInfo& p_param1
    );
    MethodInfo(
        Variant::Type ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2
    );
    MethodInfo(
        Variant::Type ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3
    );
    MethodInfo(
        Variant::Type ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3,
        const PropertyInfo& p_param4
    );
    MethodInfo(
        Variant::Type ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3,
        const PropertyInfo& p_param4,
        const PropertyInfo& p_param5
    );
    MethodInfo(const PropertyInfo& p_ret, const String& p_name);
    MethodInfo(
        const PropertyInfo& p_ret,
        const String& p_name,
        const PropertyInfo& p_param1
    );
    MethodInfo(
        const PropertyInfo& p_ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2
    );
    MethodInfo(
        const PropertyInfo& p_ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3
    );
    MethodInfo(
        const PropertyInfo& p_ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3,
        const PropertyInfo& p_param4
    );
    MethodInfo(
        const PropertyInfo& p_ret,
        const String& p_name,
        const PropertyInfo& p_param1,
        const PropertyInfo& p_param2,
        const PropertyInfo& p_param3,
        const PropertyInfo& p_param4,
        const PropertyInfo& p_param5
    );
};

#ifdef DEBUG_METHODS_ENABLED

template <bool C, typename T = void>
struct EnableIf {
    typedef T type;
};

template <typename T>
struct EnableIf<false, T> {};

template <typename, typename>
struct TypesAreSame {
    static bool const value = false;
};

template <typename A>
struct TypesAreSame<A, A> {
    static bool const value = true;
};

template <typename B, typename D>
struct TypeInherits {
    static D* get_d();

    static char (&test(B*))[1];
    static char (&test(...))[2];

    static bool const value =
        sizeof(test(get_d())) == sizeof(char)
        && !TypesAreSame<B volatile const, void volatile const>::value;
};

namespace GodotTypeInfo
{
enum Metadata {
    METADATA_NONE,
    METADATA_INT_IS_INT8,
    METADATA_INT_IS_INT16,
    METADATA_INT_IS_INT32,
    METADATA_INT_IS_INT64,
    METADATA_INT_IS_UINT8,
    METADATA_INT_IS_UINT16,
    METADATA_INT_IS_UINT32,
    METADATA_INT_IS_UINT64,
    METADATA_REAL_IS_FLOAT,
    METADATA_REAL_IS_DOUBLE
};
} // namespace GodotTypeInfo

// If the compiler fails because it's trying to instantiate the primary
// 'GetTypeInfo' template instead of one of the specializations, it's most
// likely because the type 'T' is not supported. If 'T' is a class that inherits
// 'Object', make sure it can see the actual class declaration instead of a
// forward declaration. You can always forward declare 'T' in a header file, and
// then include the actual declaration of 'T' in the source file where
// 'GetTypeInfo<T>' is instantiated.
template <class T, typename = void>
struct GetTypeInfo;

#define MAKE_TYPE_INFO(m_type, m_var_type)                                     \
    template <>                                                                \
    struct GetTypeInfo<m_type> {                                               \
        static const Variant::Type VARIANT_TYPE = m_var_type;                  \
        static const GodotTypeInfo::Metadata METADATA =                        \
            GodotTypeInfo::METADATA_NONE;                                      \
        static inline PropertyInfo get_class_info() {                          \
            return PropertyInfo(VARIANT_TYPE, String());                       \
        }                                                                      \
    };                                                                         \
    template <>                                                                \
    struct GetTypeInfo<const m_type&> {                                        \
        static const Variant::Type VARIANT_TYPE = m_var_type;                  \
        static const GodotTypeInfo::Metadata METADATA =                        \
            GodotTypeInfo::METADATA_NONE;                                      \
        static inline PropertyInfo get_class_info() {                          \
            return PropertyInfo(VARIANT_TYPE, String());                       \
        }                                                                      \
    };

#define MAKE_TYPE_INFO_WITH_META(m_type, m_var_type, m_metadata)               \
    template <>                                                                \
    struct GetTypeInfo<m_type> {                                               \
        static const Variant::Type VARIANT_TYPE       = m_var_type;            \
        static const GodotTypeInfo::Metadata METADATA = m_metadata;            \
        static inline PropertyInfo get_class_info() {                          \
            return PropertyInfo(VARIANT_TYPE, String());                       \
        }                                                                      \
    };                                                                         \
    template <>                                                                \
    struct GetTypeInfo<const m_type&> {                                        \
        static const Variant::Type VARIANT_TYPE       = m_var_type;            \
        static const GodotTypeInfo::Metadata METADATA = m_metadata;            \
        static inline PropertyInfo get_class_info() {                          \
            return PropertyInfo(VARIANT_TYPE, String());                       \
        }                                                                      \
    };

MAKE_TYPE_INFO(bool, Variant::BOOL)
MAKE_TYPE_INFO_WITH_META(
    uint8_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_UINT8
)
MAKE_TYPE_INFO_WITH_META(
    int8_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_INT8
)
MAKE_TYPE_INFO_WITH_META(
    uint16_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_UINT16
)
MAKE_TYPE_INFO_WITH_META(
    int16_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_INT16
)
MAKE_TYPE_INFO_WITH_META(
    uint32_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_UINT32
)
MAKE_TYPE_INFO_WITH_META(
    int32_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_INT32
)
MAKE_TYPE_INFO_WITH_META(
    uint64_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_UINT64
)
MAKE_TYPE_INFO_WITH_META(
    int64_t,
    Variant::INT,
    GodotTypeInfo::METADATA_INT_IS_INT64
)
MAKE_TYPE_INFO(wchar_t, Variant::INT)
MAKE_TYPE_INFO_WITH_META(
    float,
    Variant::REAL,
    GodotTypeInfo::METADATA_REAL_IS_FLOAT
)
MAKE_TYPE_INFO_WITH_META(
    double,
    Variant::REAL,
    GodotTypeInfo::METADATA_REAL_IS_DOUBLE
)

MAKE_TYPE_INFO(String, Variant::STRING)
MAKE_TYPE_INFO(Vector2, Variant::VECTOR2)
MAKE_TYPE_INFO(Rect2, Variant::RECT2)
MAKE_TYPE_INFO(Vector3, Variant::VECTOR3)
MAKE_TYPE_INFO(Transform2D, Variant::TRANSFORM2D)
MAKE_TYPE_INFO(Plane, Variant::PLANE)
MAKE_TYPE_INFO(Quat, Variant::QUAT)
MAKE_TYPE_INFO(AABB, Variant::AABB)
MAKE_TYPE_INFO(Basis, Variant::BASIS)
MAKE_TYPE_INFO(Transform, Variant::TRANSFORM)
MAKE_TYPE_INFO(Color, Variant::COLOR)
MAKE_TYPE_INFO(NodePath, Variant::NODE_PATH)
MAKE_TYPE_INFO(RID, Variant::_RID)
MAKE_TYPE_INFO(Dictionary, Variant::DICTIONARY)
MAKE_TYPE_INFO(Array, Variant::ARRAY)
MAKE_TYPE_INFO(PoolByteArray, Variant::POOL_BYTE_ARRAY)
MAKE_TYPE_INFO(PoolIntArray, Variant::POOL_INT_ARRAY)
MAKE_TYPE_INFO(PoolRealArray, Variant::POOL_REAL_ARRAY)
MAKE_TYPE_INFO(PoolStringArray, Variant::POOL_STRING_ARRAY)
MAKE_TYPE_INFO(PoolVector2Array, Variant::POOL_VECTOR2_ARRAY)
MAKE_TYPE_INFO(PoolVector3Array, Variant::POOL_VECTOR3_ARRAY)
MAKE_TYPE_INFO(PoolColorArray, Variant::POOL_COLOR_ARRAY)

MAKE_TYPE_INFO(StringName, Variant::STRING)
MAKE_TYPE_INFO(IP_Address, Variant::STRING)

class BSP_Tree;
MAKE_TYPE_INFO(BSP_Tree, Variant::DICTIONARY)

// for RefPtr
template <>
struct GetTypeInfo<RefPtr> {
    static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
    static const GodotTypeInfo::Metadata METADATA =
        GodotTypeInfo::METADATA_NONE;

    static inline PropertyInfo get_class_info() {
        return PropertyInfo(
            Variant::OBJECT,
            String(),
            PROPERTY_HINT_RESOURCE_TYPE,
            "Reference"
        );
    }
};

template <>
struct GetTypeInfo<const RefPtr&> {
    static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
    static const GodotTypeInfo::Metadata METADATA =
        GodotTypeInfo::METADATA_NONE;

    static inline PropertyInfo get_class_info() {
        return PropertyInfo(
            Variant::OBJECT,
            String(),
            PROPERTY_HINT_RESOURCE_TYPE,
            "Reference"
        );
    }
};

// for variant
template <>
struct GetTypeInfo<Variant> {
    static const Variant::Type VARIANT_TYPE = Variant::NIL;
    static const GodotTypeInfo::Metadata METADATA =
        GodotTypeInfo::METADATA_NONE;

    static inline PropertyInfo get_class_info() {
        return PropertyInfo(
            Variant::NIL,
            String(),
            PROPERTY_HINT_NONE,
            String(),
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_NIL_IS_VARIANT
        );
    }
};

template <>
struct GetTypeInfo<const Variant&> {
    static const Variant::Type VARIANT_TYPE = Variant::NIL;
    static const GodotTypeInfo::Metadata METADATA =
        GodotTypeInfo::METADATA_NONE;

    static inline PropertyInfo get_class_info() {
        return PropertyInfo(
            Variant::NIL,
            String(),
            PROPERTY_HINT_NONE,
            String(),
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_NIL_IS_VARIANT
        );
    }
};

#define MAKE_TEMPLATE_TYPE_INFO(m_template, m_type, m_var_type)                \
    template <>                                                                \
    struct GetTypeInfo<m_template<m_type>> {                                   \
        static const Variant::Type VARIANT_TYPE = m_var_type;                  \
        static const GodotTypeInfo::Metadata METADATA =                        \
            GodotTypeInfo::METADATA_NONE;                                      \
        static inline PropertyInfo get_class_info() {                          \
            return PropertyInfo(VARIANT_TYPE, String());                       \
        }                                                                      \
    };                                                                         \
    template <>                                                                \
    struct GetTypeInfo<const m_template<m_type>&> {                            \
        static const Variant::Type VARIANT_TYPE = m_var_type;                  \
        static const GodotTypeInfo::Metadata METADATA =                        \
            GodotTypeInfo::METADATA_NONE;                                      \
        static inline PropertyInfo get_class_info() {                          \
            return PropertyInfo(VARIANT_TYPE, String());                       \
        }                                                                      \
    };

MAKE_TEMPLATE_TYPE_INFO(Vector, uint8_t, Variant::POOL_BYTE_ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, int, Variant::POOL_INT_ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, float, Variant::POOL_REAL_ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, String, Variant::POOL_STRING_ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, Vector2, Variant::POOL_VECTOR2_ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, Vector3, Variant::POOL_VECTOR3_ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, Color, Variant::POOL_COLOR_ARRAY)

MAKE_TEMPLATE_TYPE_INFO(Vector, Variant, Variant::ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, RID, Variant::ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, Plane, Variant::ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, StringName, Variant::POOL_STRING_ARRAY)

MAKE_TEMPLATE_TYPE_INFO(PoolVector, Plane, Variant::ARRAY)
MAKE_TEMPLATE_TYPE_INFO(PoolVector, Face3, Variant::POOL_VECTOR3_ARRAY)

template <typename T>
struct GetTypeInfo<
    T*,
    typename EnableIf<TypeInherits<Object, T>::value>::type> {
    static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
    static const GodotTypeInfo::Metadata METADATA =
        GodotTypeInfo::METADATA_NONE;

    static inline PropertyInfo get_class_info() {
        return PropertyInfo(StringName(T::get_class_static()));
    }
};

template <typename T>
struct GetTypeInfo<
    const T*,
    typename EnableIf<TypeInherits<Object, T>::value>::type> {
    static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
    static const GodotTypeInfo::Metadata METADATA =
        GodotTypeInfo::METADATA_NONE;

    static inline PropertyInfo get_class_info() {
        return PropertyInfo(StringName(T::get_class_static()));
    }
};

#define TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_impl)                              \
    template <>                                                                \
    struct GetTypeInfo<m_impl> {                                               \
        static const Variant::Type VARIANT_TYPE = Variant::INT;                \
        static const GodotTypeInfo::Metadata METADATA =                        \
            GodotTypeInfo::METADATA_NONE;                                      \
        static inline PropertyInfo get_class_info() {                          \
            return PropertyInfo(                                               \
                Variant::INT,                                                  \
                String(),                                                      \
                PROPERTY_HINT_NONE,                                            \
                String(),                                                      \
                PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,         \
                String(#m_enum).replace("::", ".")                             \
            );                                                                 \
        }                                                                      \
    };

#define MAKE_ENUM_TYPE_INFO(m_enum)                                            \
    TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_enum)                                  \
    TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_enum const)                            \
    TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_enum&)                                 \
    TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, const m_enum&)

template <typename T>
inline StringName __constant_get_enum_name(T param, const String& p_constant) {
    if (GetTypeInfo<T>::VARIANT_TYPE == Variant::NIL) {
        ERR_PRINT(
            "Missing VARIANT_ENUM_CAST for constant's enum: " + p_constant
        );
    }
    return GetTypeInfo<T>::get_class_info().class_name;
}

#define CLASS_INFO(m_type) (GetTypeInfo<m_type*>::get_class_info())

#else

#define MAKE_ENUM_TYPE_INFO(m_enum)
#define CLASS_INFO(m_type)

#endif // DEBUG_METHODS_ENABLED

#endif // GET_TYPE_INFO_H
