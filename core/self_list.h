// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SELF_LIST_H
#define SELF_LIST_H

#include "core/error_macros.h"
#include "core/typedefs.h"

// SelfList is an item that can be added to a SelfList::List.
// If added, it will remove itself from the list when it is destroyed.

template <class T>
class SelfList {
public:
    class List {
    public:
        constexpr List() = default;
        ~List();

        constexpr SelfList* get_first();
        constexpr const SelfList* get_first() const;

        constexpr void add(SelfList* item);
        constexpr void add_last(SelfList* item);
        constexpr void remove(SelfList* item);

    private:
        SelfList* first = nullptr;
        SelfList* last  = nullptr;
    };

    constexpr explicit SelfList(T* self);
    ~SelfList();

    constexpr T* get_self() const;
    constexpr SelfList* get_next();
    constexpr const SelfList* get_next() const;
    constexpr SelfList* get_previous();
    constexpr const SelfList* get_previous() const;
    [[nodiscard]]
    constexpr bool is_in_list() const;
    constexpr void remove_from_list();

private:
    T* self;
    List* list         = nullptr;
    SelfList* next     = nullptr;
    SelfList* previous = nullptr;
};

template <class T>
SelfList<T>::List::~List() {
    ERR_FAIL_COND(first != nullptr);
}

template <class T>
constexpr SelfList<T>* SelfList<T>::List::get_first() {
    return first;
}

template <class T>
constexpr const SelfList<T>* SelfList<T>::List::get_first() const {
    return first;
}

template <class T>
constexpr void SelfList<T>::List::add(SelfList* item) {
    ERR_FAIL_COND(item->list);
    item->list     = this;
    item->next     = first;
    item->previous = nullptr;
    if (first) {
        first->previous = item;
    } else {
        last = item;
    }
    first = item;
}

template <class T>
constexpr void SelfList<T>::List::add_last(SelfList* item) {
    ERR_FAIL_COND(item->list);
    item->list     = this;
    item->next     = nullptr;
    item->previous = last;
    if (last) {
        last->next = item;
    } else {
        first = item;
    }
    last = item;
}

template <class T>
constexpr void SelfList<T>::List::remove(SelfList* item) {
    ERR_FAIL_COND(item->list != this);
    if (item->next) {
        item->next->previous = item->previous;
    }
    if (item->previous) {
        item->previous->next = item->next;
    }
    if (first == item) {
        first = item->next;
    }
    if (last == item) {
        last = item->previous;
    }
    item->next     = nullptr;
    item->previous = nullptr;
    item->list     = nullptr;
}

template <class T>
constexpr SelfList<T>::SelfList(T* self) : self(self) {}

template <class T>
SelfList<T>::~SelfList() {
    if (list) {
        list->remove(this);
    }
}

template <class T>
constexpr T* SelfList<T>::get_self() const {
    return self;
}

template <class T>
constexpr SelfList<T>* SelfList<T>::get_next() {
    return next;
}

template <class T>
constexpr const SelfList<T>* SelfList<T>::get_next() const {
    return next;
}

template <class T>
constexpr SelfList<T>* SelfList<T>::get_previous() {
    return previous;
}

template <class T>
constexpr const SelfList<T>* SelfList<T>::get_previous() const {
    return previous;
}

template <class T>
constexpr bool SelfList<T>::is_in_list() const {
    return list;
}

template <class T>
constexpr void SelfList<T>::remove_from_list() {
    if (list) {
        list->remove(this);
    }
}

#endif // SELF_LIST_H
