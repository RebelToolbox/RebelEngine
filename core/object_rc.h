// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef OBJECTRC_H
#define OBJECTRC_H

#include "core/os/memory.h"
#include "core/typedefs.h"

#include <atomic>

class Object;

// Used to track Variants pointing to a non-Reference Object
class ObjectRC {
    std::atomic<Object*> _ptr;
    std::atomic<uint32_t> _users;

public:
    // This is for allowing debug builds to check for instance ID validity,
    // so warnings are shown in debug builds when a stray Variant (one pointing
    // to a released Object) would have happened.
    const ObjectID instance_id;

    _FORCE_INLINE_ void increment() {
        _users.fetch_add(1, std::memory_order_relaxed);
    }

    _FORCE_INLINE_ bool decrement() {
        return _users.fetch_sub(1, std::memory_order_relaxed) == 1;
    }

    _FORCE_INLINE_ bool invalidate() {
        _ptr.store(nullptr, std::memory_order_release);
        return decrement();
    }

    _FORCE_INLINE_ Object* get_ptr() {
        return _ptr.load(std::memory_order_acquire);
    }

    _FORCE_INLINE_ ObjectRC(Object* p_object) :
        instance_id(p_object->get_instance_id()) {
        // 1 (the Object) + 1 (the first user)
        _users.store(2, std::memory_order_relaxed);
        _ptr.store(p_object, std::memory_order_release);
    }
};

#endif
