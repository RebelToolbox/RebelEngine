// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "gdnative/variant.h"

#include "core/reference.h"
#include "core/variant.h"

#ifdef __cplusplus
extern "C" {
#endif

static_assert(
    sizeof(rebel_variant) == sizeof(Variant),
    "Variant size mismatch"
);

// Workaround GCC ICE on armv7hl which was affected GCC 6.0 up to 8.0
// (GH-16100). It was fixed upstream in 8.1, and a fix was backported to 7.4.
// This can be removed once no supported distro ships with versions older
// than 7.4.
#if defined(__arm__) && defined(__GNUC__) && !defined(__clang__)               \
    && (__GNUC__ == 6 || (__GNUC__ == 7 && __GNUC_MINOR__ < 4)                 \
        || (__GNUC__ == 8 && __GNUC_MINOR__ < 1))
#pragma GCC push_options
#pragma GCC optimize("-O0")
#endif

#define memnew_placement_custom(m_placement, m_class, m_constr)                \
    _post_initialize(new (m_placement, sizeof(m_class), "") m_constr)

#if defined(__arm__) && defined(__GNUC__) && !defined(__clang__)               \
    && (__GNUC__ == 6 || (__GNUC__ == 7 && __GNUC_MINOR__ < 4)                 \
        || (__GNUC__ == 8 && __GNUC_MINOR__ < 1))
#pragma GCC pop_options
#endif

// Constructors

rebel_variant_type GDAPI rebel_variant_get_type(const rebel_variant* p_self) {
    const Variant* self = (const Variant*)p_self;
    return (rebel_variant_type)self->get_type();
}

void GDAPI
rebel_variant_new_copy(rebel_variant* p_dest, const rebel_variant* p_src) {
    Variant* dest = (Variant*)p_dest;
    Variant* src  = (Variant*)p_src;
    memnew_placement(dest, Variant(*src));
}

void GDAPI rebel_variant_new_nil(rebel_variant* r_dest) {
    Variant* dest = (Variant*)r_dest;
    memnew_placement(dest, Variant);
}

void GDAPI rebel_variant_new_bool(rebel_variant* r_dest, const rebel_bool p_b) {
    Variant* dest = (Variant*)r_dest;
    memnew_placement_custom(dest, Variant, Variant(p_b));
}

void GDAPI rebel_variant_new_uint(rebel_variant* r_dest, const uint64_t p_i) {
    Variant* dest = (Variant*)r_dest;
    memnew_placement_custom(dest, Variant, Variant(p_i));
}

void GDAPI rebel_variant_new_int(rebel_variant* r_dest, const int64_t p_i) {
    Variant* dest = (Variant*)r_dest;
    memnew_placement_custom(dest, Variant, Variant(p_i));
}

void GDAPI rebel_variant_new_real(rebel_variant* r_dest, const double p_r) {
    Variant* dest = (Variant*)r_dest;
    memnew_placement_custom(dest, Variant, Variant(p_r));
}

void GDAPI
rebel_variant_new_string(rebel_variant* r_dest, const rebel_string* p_s) {
    Variant* dest = (Variant*)r_dest;
    String* s     = (String*)p_s;
    memnew_placement_custom(dest, Variant, Variant(*s));
}

void GDAPI
rebel_variant_new_vector2(rebel_variant* r_dest, const rebel_vector2* p_v2) {
    Variant* dest = (Variant*)r_dest;
    Vector2* v2   = (Vector2*)p_v2;
    memnew_placement_custom(dest, Variant, Variant(*v2));
}

void GDAPI
rebel_variant_new_rect2(rebel_variant* r_dest, const rebel_rect2* p_rect2) {
    Variant* dest = (Variant*)r_dest;
    Rect2* rect2  = (Rect2*)p_rect2;
    memnew_placement_custom(dest, Variant, Variant(*rect2));
}

void GDAPI
rebel_variant_new_vector3(rebel_variant* r_dest, const rebel_vector3* p_v3) {
    Variant* dest = (Variant*)r_dest;
    Vector3* v3   = (Vector3*)p_v3;
    memnew_placement_custom(dest, Variant, Variant(*v3));
}

void GDAPI rebel_variant_new_transform2d(
    rebel_variant* r_dest,
    const rebel_transform2d* p_t2d
) {
    Variant* dest    = (Variant*)r_dest;
    Transform2D* t2d = (Transform2D*)p_t2d;
    memnew_placement_custom(dest, Variant, Variant(*t2d));
}

void GDAPI
rebel_variant_new_plane(rebel_variant* r_dest, const rebel_plane* p_plane) {
    Variant* dest = (Variant*)r_dest;
    Plane* plane  = (Plane*)p_plane;
    memnew_placement_custom(dest, Variant, Variant(*plane));
}

void GDAPI
rebel_variant_new_quat(rebel_variant* r_dest, const rebel_quat* p_quat) {
    Variant* dest = (Variant*)r_dest;
    Quat* quat    = (Quat*)p_quat;
    memnew_placement_custom(dest, Variant, Variant(*quat));
}

void GDAPI
rebel_variant_new_aabb(rebel_variant* r_dest, const rebel_aabb* p_aabb) {
    Variant* dest = (Variant*)r_dest;
    AABB* aabb    = (AABB*)p_aabb;
    memnew_placement_custom(dest, Variant, Variant(*aabb));
}

void GDAPI
rebel_variant_new_basis(rebel_variant* r_dest, const rebel_basis* p_basis) {
    Variant* dest = (Variant*)r_dest;
    Basis* basis  = (Basis*)p_basis;
    memnew_placement_custom(dest, Variant, Variant(*basis));
}

void GDAPI rebel_variant_new_transform(
    rebel_variant* r_dest,
    const rebel_transform* p_trans
) {
    Variant* dest    = (Variant*)r_dest;
    Transform* trans = (Transform*)p_trans;
    memnew_placement_custom(dest, Variant, Variant(*trans));
}

void GDAPI
rebel_variant_new_color(rebel_variant* r_dest, const rebel_color* p_color) {
    Variant* dest = (Variant*)r_dest;
    Color* color  = (Color*)p_color;
    memnew_placement_custom(dest, Variant, Variant(*color));
}

void GDAPI rebel_variant_new_node_path(
    rebel_variant* r_dest,
    const rebel_node_path* p_np
) {
    Variant* dest = (Variant*)r_dest;
    NodePath* np  = (NodePath*)p_np;
    memnew_placement_custom(dest, Variant, Variant(*np));
}

void GDAPI
rebel_variant_new_rid(rebel_variant* r_dest, const rebel_rid* p_rid) {
    Variant* dest = (Variant*)r_dest;
    RID* rid      = (RID*)p_rid;
    memnew_placement_custom(dest, Variant, Variant(*rid));
}

void GDAPI
rebel_variant_new_object(rebel_variant* r_dest, const rebel_object* p_obj) {
    Variant* dest        = (Variant*)r_dest;
    Object* obj          = (Object*)p_obj;
    Reference* reference = Object::cast_to<Reference>(obj);
    REF ref;
    if (reference) {
        ref = REF(reference);
    }
    if (!ref.is_null()) {
        memnew_placement_custom(dest, Variant, Variant(ref.get_ref_ptr()));
    } else {
#if defined(DEBUG_METHODS_ENABLED)
        if (reference) {
            ERR_PRINT(
                "Reference object has 0 refcount in rebel_variant_new_object - "
                "you lost it somewhere."
            );
        }
#endif
        memnew_placement_custom(dest, Variant, Variant(obj));
    }
}

void GDAPI rebel_variant_new_dictionary(
    rebel_variant* r_dest,
    const rebel_dictionary* p_dict
) {
    Variant* dest    = (Variant*)r_dest;
    Dictionary* dict = (Dictionary*)p_dict;
    memnew_placement_custom(dest, Variant, Variant(*dict));
}

void GDAPI
rebel_variant_new_array(rebel_variant* r_dest, const rebel_array* p_arr) {
    Variant* dest = (Variant*)r_dest;
    Array* arr    = (Array*)p_arr;
    memnew_placement_custom(dest, Variant, Variant(*arr));
}

void GDAPI rebel_variant_new_pool_byte_array(
    rebel_variant* r_dest,
    const rebel_pool_byte_array* p_pba
) {
    Variant* dest      = (Variant*)r_dest;
    PoolByteArray* pba = (PoolByteArray*)p_pba;
    memnew_placement_custom(dest, Variant, Variant(*pba));
}

void GDAPI rebel_variant_new_pool_int_array(
    rebel_variant* r_dest,
    const rebel_pool_int_array* p_pia
) {
    Variant* dest     = (Variant*)r_dest;
    PoolIntArray* pia = (PoolIntArray*)p_pia;
    memnew_placement_custom(dest, Variant, Variant(*pia));
}

void GDAPI rebel_variant_new_pool_real_array(
    rebel_variant* r_dest,
    const rebel_pool_real_array* p_pra
) {
    Variant* dest      = (Variant*)r_dest;
    PoolRealArray* pra = (PoolRealArray*)p_pra;
    memnew_placement_custom(dest, Variant, Variant(*pra));
}

void GDAPI rebel_variant_new_pool_string_array(
    rebel_variant* r_dest,
    const rebel_pool_string_array* p_psa
) {
    Variant* dest        = (Variant*)r_dest;
    PoolStringArray* psa = (PoolStringArray*)p_psa;
    memnew_placement_custom(dest, Variant, Variant(*psa));
}

void GDAPI rebel_variant_new_pool_vector2_array(
    rebel_variant* r_dest,
    const rebel_pool_vector2_array* p_pv2a
) {
    Variant* dest          = (Variant*)r_dest;
    PoolVector2Array* pv2a = (PoolVector2Array*)p_pv2a;
    memnew_placement_custom(dest, Variant, Variant(*pv2a));
}

void GDAPI rebel_variant_new_pool_vector3_array(
    rebel_variant* r_dest,
    const rebel_pool_vector3_array* p_pv3a
) {
    Variant* dest          = (Variant*)r_dest;
    PoolVector3Array* pv3a = (PoolVector3Array*)p_pv3a;
    memnew_placement_custom(dest, Variant, Variant(*pv3a));
}

void GDAPI rebel_variant_new_pool_color_array(
    rebel_variant* r_dest,
    const rebel_pool_color_array* p_pca
) {
    Variant* dest       = (Variant*)r_dest;
    PoolColorArray* pca = (PoolColorArray*)p_pca;
    memnew_placement_custom(dest, Variant, Variant(*pca));
}

rebel_bool GDAPI rebel_variant_as_bool(const rebel_variant* p_self) {
    const Variant* self = (const Variant*)p_self;
    return self->operator bool();
}

uint64_t GDAPI rebel_variant_as_uint(const rebel_variant* p_self) {
    const Variant* self = (const Variant*)p_self;
    return self->operator uint64_t();
}

int64_t GDAPI rebel_variant_as_int(const rebel_variant* p_self) {
    const Variant* self = (const Variant*)p_self;
    return self->operator int64_t();
}

double GDAPI rebel_variant_as_real(const rebel_variant* p_self) {
    const Variant* self = (const Variant*)p_self;
    return self->operator double();
}

rebel_string GDAPI rebel_variant_as_string(const rebel_variant* p_self) {
    rebel_string raw_dest;
    const Variant* self = (const Variant*)p_self;
    String* dest        = (String*)&raw_dest;
    memnew_placement(
        dest,
        String(self->operator String())
    ); // operator = is overloaded by String
    return raw_dest;
}

rebel_vector2 GDAPI rebel_variant_as_vector2(const rebel_variant* p_self) {
    rebel_vector2 raw_dest;
    const Variant* self = (const Variant*)p_self;
    Vector2* dest       = (Vector2*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_rect2 GDAPI rebel_variant_as_rect2(const rebel_variant* p_self) {
    rebel_rect2 raw_dest;
    const Variant* self = (const Variant*)p_self;
    Rect2* dest         = (Rect2*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_vector3 GDAPI rebel_variant_as_vector3(const rebel_variant* p_self) {
    rebel_vector3 raw_dest;
    const Variant* self = (const Variant*)p_self;
    Vector3* dest       = (Vector3*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_transform2d GDAPI rebel_variant_as_transform2d(const rebel_variant* p_self
) {
    rebel_transform2d raw_dest;
    const Variant* self = (const Variant*)p_self;
    Transform2D* dest   = (Transform2D*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_plane GDAPI rebel_variant_as_plane(const rebel_variant* p_self) {
    rebel_plane raw_dest;
    const Variant* self = (const Variant*)p_self;
    Plane* dest         = (Plane*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_quat GDAPI rebel_variant_as_quat(const rebel_variant* p_self) {
    rebel_quat raw_dest;
    const Variant* self = (const Variant*)p_self;
    Quat* dest          = (Quat*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_aabb GDAPI rebel_variant_as_aabb(const rebel_variant* p_self) {
    rebel_aabb raw_dest;
    const Variant* self = (const Variant*)p_self;
    AABB* dest          = (AABB*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_basis GDAPI rebel_variant_as_basis(const rebel_variant* p_self) {
    rebel_basis raw_dest;
    const Variant* self = (const Variant*)p_self;
    Basis* dest         = (Basis*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_transform GDAPI rebel_variant_as_transform(const rebel_variant* p_self) {
    rebel_transform raw_dest;
    const Variant* self = (const Variant*)p_self;
    Transform* dest     = (Transform*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_color GDAPI rebel_variant_as_color(const rebel_variant* p_self) {
    rebel_color raw_dest;
    const Variant* self = (const Variant*)p_self;
    Color* dest         = (Color*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_node_path GDAPI rebel_variant_as_node_path(const rebel_variant* p_self) {
    rebel_node_path raw_dest;
    const Variant* self = (const Variant*)p_self;
    NodePath* dest      = (NodePath*)&raw_dest;
    memnew_placement(
        dest,
        NodePath(self->operator NodePath())
    ); // operator = is overloaded by NodePath
    return raw_dest;
}

rebel_rid GDAPI rebel_variant_as_rid(const rebel_variant* p_self) {
    rebel_rid raw_dest;
    const Variant* self = (const Variant*)p_self;
    RID* dest           = (RID*)&raw_dest;
    *dest               = *self;
    return raw_dest;
}

rebel_object GDAPI* rebel_variant_as_object(const rebel_variant* p_self) {
    const Variant* self = (const Variant*)p_self;
    Object* dest;
    dest = *self;
    return (rebel_object*)dest;
}

rebel_dictionary GDAPI rebel_variant_as_dictionary(const rebel_variant* p_self
) {
    rebel_dictionary raw_dest;
    const Variant* self = (const Variant*)p_self;
    Dictionary* dest    = (Dictionary*)&raw_dest;
    memnew_placement(
        dest,
        Dictionary(self->operator Dictionary())
    ); // operator = is overloaded by Dictionary
    return raw_dest;
}

rebel_array GDAPI rebel_variant_as_array(const rebel_variant* p_self) {
    rebel_array raw_dest;
    const Variant* self = (const Variant*)p_self;
    Array* dest         = (Array*)&raw_dest;
    memnew_placement(
        dest,
        Array(self->operator Array())
    ); // operator = is overloaded by Array
    return raw_dest;
}

rebel_pool_byte_array GDAPI
rebel_variant_as_pool_byte_array(const rebel_variant* p_self) {
    rebel_pool_byte_array raw_dest;
    const Variant* self = (const Variant*)p_self;
    PoolByteArray* dest = (PoolByteArray*)&raw_dest;
    memnew_placement(
        dest,
        PoolByteArray(self->operator PoolByteArray())
    ); // operator = is overloaded by PoolByteArray
    *dest = *self;
    return raw_dest;
}

rebel_pool_int_array GDAPI
rebel_variant_as_pool_int_array(const rebel_variant* p_self) {
    rebel_pool_int_array raw_dest;
    const Variant* self = (const Variant*)p_self;
    PoolIntArray* dest  = (PoolIntArray*)&raw_dest;
    memnew_placement(
        dest,
        PoolIntArray(self->operator PoolIntArray())
    ); // operator = is overloaded by PoolIntArray
    *dest = *self;
    return raw_dest;
}

rebel_pool_real_array GDAPI
rebel_variant_as_pool_real_array(const rebel_variant* p_self) {
    rebel_pool_real_array raw_dest;
    const Variant* self = (const Variant*)p_self;
    PoolRealArray* dest = (PoolRealArray*)&raw_dest;
    memnew_placement(
        dest,
        PoolRealArray(self->operator PoolRealArray())
    ); // operator = is overloaded by PoolRealArray
    *dest = *self;
    return raw_dest;
}

rebel_pool_string_array GDAPI
rebel_variant_as_pool_string_array(const rebel_variant* p_self) {
    rebel_pool_string_array raw_dest;
    const Variant* self   = (const Variant*)p_self;
    PoolStringArray* dest = (PoolStringArray*)&raw_dest;
    memnew_placement(
        dest,
        PoolStringArray(self->operator PoolStringArray())
    ); // operator = is overloaded by PoolStringArray
    *dest = *self;
    return raw_dest;
}

rebel_pool_vector2_array GDAPI
rebel_variant_as_pool_vector2_array(const rebel_variant* p_self) {
    rebel_pool_vector2_array raw_dest;
    const Variant* self    = (const Variant*)p_self;
    PoolVector2Array* dest = (PoolVector2Array*)&raw_dest;
    memnew_placement(
        dest,
        PoolVector2Array(self->operator PoolVector2Array())
    ); // operator = is overloaded by PoolVector2Array
    *dest = *self;
    return raw_dest;
}

rebel_pool_vector3_array GDAPI
rebel_variant_as_pool_vector3_array(const rebel_variant* p_self) {
    rebel_pool_vector3_array raw_dest;
    const Variant* self    = (const Variant*)p_self;
    PoolVector3Array* dest = (PoolVector3Array*)&raw_dest;
    memnew_placement(
        dest,
        PoolVector3Array(self->operator PoolVector3Array())
    ); // operator = is overloaded by PoolVector3Array
    *dest = *self;
    return raw_dest;
}

rebel_pool_color_array GDAPI
rebel_variant_as_pool_color_array(const rebel_variant* p_self) {
    rebel_pool_color_array raw_dest;
    const Variant* self  = (const Variant*)p_self;
    PoolColorArray* dest = (PoolColorArray*)&raw_dest;
    memnew_placement(
        dest,
        PoolColorArray(self->operator PoolColorArray())
    ); // operator = is overloaded by PoolColorArray
    *dest = *self;
    return raw_dest;
}

rebel_variant GDAPI rebel_variant_call(
    rebel_variant* p_self,
    const rebel_string* p_method,
    const rebel_variant** p_args,
    const rebel_int p_argcount,
    rebel_variant_call_error* r_error
) {
    Variant* self        = (Variant*)p_self;
    String* method       = (String*)p_method;
    const Variant** args = (const Variant**)p_args;
    rebel_variant raw_dest;
    Variant* dest = (Variant*)&raw_dest;
    Variant::CallError error;
    memnew_placement_custom(
        dest,
        Variant,
        Variant(self->call(*method, args, p_argcount, error))
    );
    if (r_error) {
        r_error->error    = (rebel_variant_call_error_error)error.error;
        r_error->argument = error.argument;
        r_error->expected = (rebel_variant_type)error.expected;
    }
    return raw_dest;
}

rebel_bool GDAPI rebel_variant_has_method(
    const rebel_variant* p_self,
    const rebel_string* p_method
) {
    const Variant* self  = (const Variant*)p_self;
    const String* method = (const String*)p_method;
    return self->has_method(*method);
}

rebel_bool GDAPI rebel_variant_operator_equal(
    const rebel_variant* p_self,
    const rebel_variant* p_other
) {
    const Variant* self  = (const Variant*)p_self;
    const Variant* other = (const Variant*)p_other;
    return self->operator==(*other);
}

rebel_bool GDAPI rebel_variant_operator_less(
    const rebel_variant* p_self,
    const rebel_variant* p_other
) {
    const Variant* self  = (const Variant*)p_self;
    const Variant* other = (const Variant*)p_other;
    return self->operator<(*other);
}

rebel_bool GDAPI rebel_variant_hash_compare(
    const rebel_variant* p_self,
    const rebel_variant* p_other
) {
    const Variant* self  = (const Variant*)p_self;
    const Variant* other = (const Variant*)p_other;
    return self->hash_compare(*other);
}

rebel_bool GDAPI rebel_variant_booleanize(const rebel_variant* p_self) {
    const Variant* self = (const Variant*)p_self;
    return self->booleanize();
}

void GDAPI rebel_variant_destroy(rebel_variant* p_self) {
    Variant* self = (Variant*)p_self;
    self->~Variant();
}

// GDNative core 1.1

rebel_string GDAPI rebel_variant_get_operator_name(rebel_variant_operator p_op
) {
    Variant::Operator op = (Variant::Operator)p_op;
    rebel_string raw_dest;
    String* dest = (String*)&raw_dest;
    memnew_placement(
        dest,
        String(Variant::get_operator_name(op))
    ); // operator = is overloaded by String
    return raw_dest;
}

void GDAPI rebel_variant_evaluate(
    rebel_variant_operator p_op,
    const rebel_variant* p_a,
    const rebel_variant* p_b,
    rebel_variant* r_ret,
    rebel_bool* r_valid
) {
    Variant::Operator op = (Variant::Operator)p_op;
    const Variant* a     = (const Variant*)p_a;
    const Variant* b     = (const Variant*)p_b;
    Variant* ret         = (Variant*)r_ret;
    Variant::evaluate(op, *a, *b, *ret, *r_valid);
}

#ifdef __cplusplus
}
#endif
