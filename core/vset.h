// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VSET_H
#define VSET_H

#include "core/typedefs.h"
#include "core/vector.h"

template <class T>
class VSet {
    Vector<T> _data;

    _FORCE_INLINE_ int _find(const T& p_val, bool& r_exact) const {
        r_exact = false;
        if (_data.empty()) {
            return 0;
        }

        int low    = 0;
        int high   = _data.size() - 1;
        const T* a = &_data[0];
        int middle = 0;

#ifdef DEBUG_ENABLED
        if (low > high) {
            ERR_PRINT("low > high, this may be a bug");
        }
#endif

        while (low <= high) {
            middle = (low + high) / 2;

            if (p_val < a[middle]) {
                high = middle - 1; // search low end of array
            } else if (a[middle] < p_val) {
                low = middle + 1; // search high end of array
            } else {
                r_exact = true;
                return middle;
            }
        }

        // return the position where this would be inserted
        if (a[middle] < p_val) {
            middle++;
        }
        return middle;
    }

    _FORCE_INLINE_ int _find_exact(const T& p_val) const {
        if (_data.empty()) {
            return -1;
        }

        int low  = 0;
        int high = _data.size() - 1;
        int middle;
        const T* a = &_data[0];

        while (low <= high) {
            middle = (low + high) / 2;

            if (p_val < a[middle]) {
                high = middle - 1; // search low end of array
            } else if (a[middle] < p_val) {
                low = middle + 1; // search high end of array
            } else {
                return middle;
            }
        }

        return -1;
    }

public:
    void insert(const T& p_val) {
        bool exact;
        int pos = _find(p_val, exact);
        if (exact) {
            return;
        }
        _data.insert(pos, p_val);
    }

    bool has(const T& p_val) const {
        return _find_exact(p_val) != -1;
    }

    void erase(const T& p_val) {
        int pos = _find_exact(p_val);
        if (pos < 0) {
            return;
        }
        _data.remove(pos);
    }

    int find(const T& p_val) const {
        return _find_exact(p_val);
    }

    _FORCE_INLINE_ bool empty() const {
        return _data.empty();
    }

    _FORCE_INLINE_ int size() const {
        return _data.size();
    }

    inline T& operator[](int p_index) {
        return _data.write[p_index];
    }

    inline const T& operator[](int p_index) const {
        return _data[p_index];
    }
};

#endif // VSET_H
