// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VMAP_H
#define VMAP_H

#include "core/cowdata.h"

template <class Key, class Value>
class VMap {
public:
    struct Pair {
        Key key;
        Value value;

        Pair() = default;

        Pair(const Key& key, const Value& value) : key(key), value(value) {}
    };

    VMap() = default;
    VMap(const VMap& other);
    VMap& operator=(const VMap& other);

    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;

    // Returns -1 if not found.
    int get_position(const Key& key) const;
    // If not found, returns the position where this key would be inserted.
    int get_best_position(const Key& key) const;
    bool has(const Key& key) const;

    [[nodiscard]]
    bool empty() const;
    [[nodiscard]]
    int size() const;

    int insert(const Key& key, const Value& value);
    void erase(const Key& key);

    Key& get_key_at(int index);
    const Key& get_key_at(int index) const;

    Value& get_value_at(int index);
    const Value& get_value_at(int index) const;

    Pair* get_array();
    const Pair* get_array() const;

private:
    CowData<Pair> cow_data;

    int get_best_position(const Key& key, bool& found) const;
};

template <class Key, class Value>
VMap<Key, Value>::VMap(const VMap& other) {
    cow_data._ref(other.cow_data);
}

template <class Key, class Value>
VMap<Key, Value>& VMap<Key, Value>::operator=(const VMap& other) {
    cow_data._ref(other.cow_data);
    return *this;
}

template <class Key, class Value>
Value& VMap<Key, Value>::operator[](const Key& key) {
    int position = get_position(key);
    if (position < 0) {
        position = insert(key, Value());
    }
    return cow_data.get_m(position).value;
}

template <class Key, class Value>
const Value& VMap<Key, Value>::operator[](const Key& key) const {
    int position = get_position(key);
    CRASH_COND(position < 0);
    return cow_data.get(position).value;
}

template <class Key, class Value>
int VMap<Key, Value>::get_position(const Key& key) const {
    bool found;
    const int position = get_best_position(key, found);
    if (!found) {
        return -1;
    }
    return position;
}

template <class Key, class Value>
int VMap<Key, Value>::get_best_position(const Key& key) const {
    bool found;
    return get_best_position(key, found);
}

template <class Key, class Value>
bool VMap<Key, Value>::has(const Key& key) const {
    return get_position(key) != -1;
}

template <class Key, class Value>
bool VMap<Key, Value>::empty() const {
    return cow_data.empty();
}

template <class Key, class Value>
int VMap<Key, Value>::size() const {
    return cow_data.size();
}

template <class Key, class Value>
int VMap<Key, Value>::insert(const Key& key, const Value& value) {
    bool found;
    int position = get_best_position(key, found);
    if (found) {
        cow_data.get_m(position).value = value;
    } else {
        cow_data.insert(position, Pair(key, value));
    }
    return position;
}

template <class Key, class Value>
void VMap<Key, Value>::erase(const Key& key) {
    int position = get_position(key);
    if (position < 0) {
        return;
    }
    cow_data.remove(position);
}

template <class Key, class Value>
const Key& VMap<Key, Value>::get_key_at(int index) const {
    return cow_data.get(index).key;
}

template <class Key, class Value>
Key& VMap<Key, Value>::get_key_at(int index) {
    return cow_data.get_m(index).key;
}

template <class Key, class Value>
Value& VMap<Key, Value>::get_value_at(int index) {
    return cow_data.get_m(index).value;
}

template <class Key, class Value>
const Value& VMap<Key, Value>::get_value_at(int index) const {
    return cow_data.get(index).value;
}

template <class Key, class Value>
typename VMap<Key, Value>::Pair* VMap<Key, Value>::get_array() {
    return cow_data.ptrw();
}

template <class Key, class Value>
const typename VMap<Key, Value>::Pair* VMap<Key, Value>::get_array() const {
    return cow_data.ptr();
}

template <class Key, class Value>
int VMap<Key, Value>::get_best_position(const Key& key, bool& found) const {
    found = false;
    if (cow_data.empty()) {
        return 0;
    }

    int position      = 0;
    int begin         = 0;
    int end           = cow_data.size() - 1;
    const Pair* pairs = cow_data.ptr();
    while (begin <= end) {
        position = (begin + end) / 2;
        if (key < pairs[position].key) {
            // Search first half of the map.
            end = position - 1;
        } else if (pairs[position].key < key) {
            // Search second half of the map.
            begin = position + 1;
        } else {
            found = true;
            return position;
        }
    }

    if (pairs[position].key < key) {
        position++;
    }
    return position;
}

#endif // VMAP_H
