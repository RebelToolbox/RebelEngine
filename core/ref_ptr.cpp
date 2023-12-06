// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "ref_ptr.h"

#include "core/reference.h"
#include "core/resource.h"

void RefPtr::operator=(const RefPtr& p_other) {
    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    Ref<Reference>* ref_other =
        reinterpret_cast<Ref<Reference>*>(const_cast<char*>(&p_other.data[0]));

    *ref = *ref_other;
}

bool RefPtr::operator==(const RefPtr& p_other) const {
    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    Ref<Reference>* ref_other =
        reinterpret_cast<Ref<Reference>*>(const_cast<char*>(&p_other.data[0]));

    return *ref == *ref_other;
}

bool RefPtr::operator!=(const RefPtr& p_other) const {
    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    Ref<Reference>* ref_other =
        reinterpret_cast<Ref<Reference>*>(const_cast<char*>(&p_other.data[0]));

    return *ref != *ref_other;
}

RefPtr::RefPtr(const RefPtr& p_other) {
    memnew_placement(&data[0], Ref<Reference>);

    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    Ref<Reference>* ref_other =
        reinterpret_cast<Ref<Reference>*>(const_cast<char*>(&p_other.data[0]));

    *ref = *ref_other;
}

bool RefPtr::is_null() const {
    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    return ref->is_null();
}

RID RefPtr::get_rid() const {
    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    if (ref->is_null()) {
        return RID();
    }
    Resource* res = Object::cast_to<Resource>(ref->ptr());
    if (res) {
        return res->get_rid();
    }
    return RID();
}

void RefPtr::unref() {
    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    ref->unref();
}

RefPtr::RefPtr() {
    ERR_FAIL_COND(sizeof(Ref<Reference>) > DATASIZE);
    memnew_placement(&data[0], Ref<Reference>);
}

RefPtr::~RefPtr() {
    Ref<Reference>* ref = reinterpret_cast<Ref<Reference>*>(&data[0]);
    ref->~Ref<Reference>();
}
