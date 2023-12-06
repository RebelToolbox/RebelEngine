// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RWLOCK_H
#define RWLOCK_H

#include "core/error_list.h"

#if !defined(NO_THREADS)

#include <shared_mutex>

class RWLock {
    mutable std::shared_timed_mutex mutex;

public:
    // Lock the rwlock, block if locked by someone else
    void read_lock() const {
        mutex.lock_shared();
    }

    // Unlock the rwlock, let other threads continue
    void read_unlock() const {
        mutex.unlock_shared();
    }

    // Attempt to lock the rwlock, OK on success, ERR_BUSY means it can't lock.
    Error read_try_lock() const {
        return mutex.try_lock_shared() ? OK : ERR_BUSY;
    }

    // Lock the rwlock, block if locked by someone else
    void write_lock() {
        mutex.lock();
    }

    // Unlock the rwlock, let other thwrites continue
    void write_unlock() {
        mutex.unlock();
    }

    // Attempt to lock the rwlock, OK on success, ERR_BUSY means it can't lock.
    Error write_try_lock() {
        return mutex.try_lock() ? OK : ERR_BUSY;
    }
};

#else

class RWLock {
public:
    void read_lock() const {}

    void read_unlock() const {}

    Error read_try_lock() const {
        return OK;
    }

    void write_lock() {}

    void write_unlock() {}

    Error write_try_lock() {
        return OK;
    }
};

#endif

class RWLockRead {
    const RWLock& lock;

public:
    RWLockRead(const RWLock& p_lock) : lock(p_lock) {
        lock.read_lock();
    }

    ~RWLockRead() {
        lock.read_unlock();
    }
};

class RWLockWrite {
    RWLock& lock;

public:
    RWLockWrite(RWLock& p_lock) : lock(p_lock) {
        lock.write_lock();
    }

    ~RWLockWrite() {
        lock.write_unlock();
    }
};

#endif // RWLOCK_H
