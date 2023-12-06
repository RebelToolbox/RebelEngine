// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RID_H
#define RID_H

#include "core/list.h"
#include "core/os/memory.h"
#include "core/safe_refcount.h"
#include "core/set.h"
#include "core/typedefs.h"

class RID_OwnerBase;

class RID_Data {
    friend class RID_OwnerBase;

#ifndef DEBUG_ENABLED
    RID_OwnerBase* _owner;
#endif
    uint32_t _id;

public:
    _FORCE_INLINE_ uint32_t get_id() const {
        return _id;
    }

    virtual ~RID_Data();
};

class RID {
    friend class RID_OwnerBase;

    mutable RID_Data* _data;

public:
    _FORCE_INLINE_ RID_Data* get_data() const {
        return _data;
    }

    _FORCE_INLINE_ bool operator==(const RID& p_rid) const {
        return _data == p_rid._data;
    }

    _FORCE_INLINE_ bool operator<(const RID& p_rid) const {
        return _data < p_rid._data;
    }

    _FORCE_INLINE_ bool operator<=(const RID& p_rid) const {
        return _data <= p_rid._data;
    }

    _FORCE_INLINE_ bool operator>(const RID& p_rid) const {
        return _data > p_rid._data;
    }

    _FORCE_INLINE_ bool operator!=(const RID& p_rid) const {
        return _data != p_rid._data;
    }

    _FORCE_INLINE_ bool is_valid() const {
        return _data != nullptr;
    }

    _FORCE_INLINE_ uint32_t get_id() const {
        return _data ? _data->get_id() : 0;
    }

    _FORCE_INLINE_ RID() {
        _data = nullptr;
    }
};

class RID_OwnerBase {
protected:
    static SafeRefCount refcount;

    _FORCE_INLINE_ void _set_data(RID& p_rid, RID_Data* p_data) {
        p_rid._data = p_data;
        refcount.ref();
        p_data->_id = refcount.get();
#ifndef DEBUG_ENABLED
        p_data->_owner = this;
#endif
    }

#ifndef DEBUG_ENABLED

    _FORCE_INLINE_ bool _is_owner(const RID& p_rid) const {
        return this == p_rid._data->_owner;
    }

    _FORCE_INLINE_ void _remove_owner(RID& p_rid) {
        p_rid._data->_owner = NULL;
    }
#endif

public:
    virtual void get_owned_list(List<RID>* p_owned) = 0;

    static void init_rid();

    virtual ~RID_OwnerBase() {}
};

template <class T>
class RID_Owner : public RID_OwnerBase {
public:
#ifdef DEBUG_ENABLED
    mutable Set<RID_Data*> id_map;
#endif

public:
    _FORCE_INLINE_ RID make_rid(T* p_data) {
        RID rid;
        _set_data(rid, p_data);

#ifdef DEBUG_ENABLED
        id_map.insert(p_data);
#endif

        return rid;
    }

    _FORCE_INLINE_ T* get(const RID& p_rid) {
#ifdef DEBUG_ENABLED

        ERR_FAIL_COND_V(!p_rid.is_valid(), nullptr);
        ERR_FAIL_COND_V(!id_map.has(p_rid.get_data()), nullptr);
#endif
        return static_cast<T*>(p_rid.get_data());
    }

    _FORCE_INLINE_ T* getornull(const RID& p_rid) {
#ifdef DEBUG_ENABLED

        if (p_rid.get_data()) {
            ERR_FAIL_COND_V(!id_map.has(p_rid.get_data()), nullptr);
        }
#endif
        return static_cast<T*>(p_rid.get_data());
    }

    _FORCE_INLINE_ T* getptr(const RID& p_rid) {
        return static_cast<T*>(p_rid.get_data());
    }

    _FORCE_INLINE_ bool owns(const RID& p_rid) const {
        if (p_rid.get_data() == nullptr) {
            return false;
        }
#ifdef DEBUG_ENABLED
        return id_map.has(p_rid.get_data());
#else
        return _is_owner(p_rid);
#endif
    }

    void free(RID p_rid) {
#ifdef DEBUG_ENABLED
        id_map.erase(p_rid.get_data());
#else
        _remove_owner(p_rid);
#endif
    }

    void get_owned_list(List<RID>* p_owned) {
#ifdef DEBUG_ENABLED

        for (typename Set<RID_Data*>::Element* E = id_map.front(); E;
             E                                   = E->next()) {
            RID r;
            _set_data(r, static_cast<T*>(E->get()));
            p_owned->push_back(r);
        }
#endif
    }
};

#endif
