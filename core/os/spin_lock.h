// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H

#include "core/typedefs.h"

#include <atomic>

class SpinLock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
    _ALWAYS_INLINE_ void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) {
            ;
        }
    }

    _ALWAYS_INLINE_ void unlock() {
        locked.clear(std::memory_order_release);
    }
};
#endif // SPIN_LOCK_H
