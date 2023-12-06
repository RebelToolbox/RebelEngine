// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "core/error_list.h"
#include "core/typedefs.h"

#if !defined(NO_THREADS)

#include <condition_variable>
#include <mutex>

class Semaphore {
private:
    mutable std::mutex mutex_;
    mutable std::condition_variable condition_;
    mutable unsigned long count_ = 0; // Initialized as locked.

public:
    _ALWAYS_INLINE_ void post() const {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }

    _ALWAYS_INLINE_ void wait() const {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        while (!count_) { // Handle spurious wake-ups.
            condition_.wait(lock);
        }
        --count_;
    }

    _ALWAYS_INLINE_ bool try_wait() const {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        if (count_) {
            --count_;
            return true;
        }
        return false;
    }

    _ALWAYS_INLINE_ int get() const {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        return count_;
    }
};

#else

class Semaphore {
public:
    _ALWAYS_INLINE_ void post() const {}

    _ALWAYS_INLINE_ void wait() const {}

    _ALWAYS_INLINE_ bool try_wait() const {
        return true;
    }

    _ALWAYS_INLINE_ int get() const {
        return 1;
    }
};

#endif

#endif // SEMAPHORE_H
