// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SIMPLE_TYPE_H
#define SIMPLE_TYPE_H

/* Batch of specializations to obtain the actual simple type */

template <class T>
struct GetSimpleTypeT {
    typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T&> {
    typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T const> {
    typedef T type_t;
};

#endif
