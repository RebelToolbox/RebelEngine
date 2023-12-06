// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ARGUMENTS_VECTOR_H
#define ARGUMENTS_VECTOR_H

#include "core/os/memory.h"

template <typename T, int POOL_SIZE = 5>
struct ArgumentsVector {
private:
    T pool[POOL_SIZE];
    T* _ptr;
    int size;

    ArgumentsVector();
    ArgumentsVector(const ArgumentsVector&);

public:
    T* ptr() {
        return _ptr;
    }

    T& get(int p_idx) {
        return _ptr[p_idx];
    }

    void set(int p_idx, const T& p_value) {
        _ptr[p_idx] = p_value;
    }

    explicit ArgumentsVector(int p_size) : size(p_size) {
        if (p_size <= POOL_SIZE) {
            _ptr = pool;
        } else {
            _ptr = memnew_arr(T, p_size);
        }
    }

    ~ArgumentsVector() {
        if (size > POOL_SIZE) {
            memdelete_arr(_ptr);
        }
    }
};

#endif // ARGUMENTS_VECTOR_H
