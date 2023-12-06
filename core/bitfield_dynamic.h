// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BITFIELD_DYNAMIC_H
#define BITFIELD_DYNAMIC_H

#include "core/error_macros.h"

class BitFieldDynamic {
public:
    ~BitFieldDynamic() {
        destroy();
    }

private:
    // prevent copying (see effective C++ scott meyers)
    // there is no implementation for copy constructor, hence compiler will
    // complain if you try to copy feel free to add one if needed...
    BitFieldDynamic& operator=(const BitFieldDynamic&);

public:
    // create automatically blanks
    void create(uint32_t p_num_bits, bool p_blank = true);
    void destroy();

    // public funcs
    uint32_t get_num_bits() const {
        return _num_bits;
    }

    uint32_t get_bit(uint32_t p_bit) const;
    void set_bit(uint32_t p_bit, uint32_t p_set);
    bool check_and_set(uint32_t p_bit);
    void blank(bool p_set_or_zero = false);
    void invert();
    void copy_from(const BitFieldDynamic& p_source);

    // loading / saving
    uint8_t* get_data() {
        return _data;
    }

    const uint8_t* get_data() const {
        return _data;
    }

    uint32_t get_num_bytes() const {
        return _num_bytes;
    }

protected:
    // member vars
    uint8_t* _data      = nullptr;
    uint32_t _num_bytes = 0;
    uint32_t _num_bits  = 0;
};

inline uint32_t BitFieldDynamic::get_bit(uint32_t p_bit) const {
    DEV_ASSERT(_data);
    uint32_t byte_number = p_bit >> 3; // divide by 8
    DEV_ASSERT(byte_number < _num_bytes);
    uint8_t uc       = _data[byte_number];
    uint32_t bit_set = uc & (1 << (p_bit & 7));
    return bit_set;
}

inline bool BitFieldDynamic::check_and_set(uint32_t p_bit) {
    DEV_ASSERT(_data);
    uint32_t byte_number = p_bit >> 3; // divide by 8
    DEV_ASSERT(byte_number < _num_bytes);
    uint8_t& uc      = _data[byte_number];
    uint32_t mask    = (1 << (p_bit & 7));
    uint32_t bit_set = uc & mask;
    if (bit_set) {
        return false;
    }

    // set
    uc = uc | mask;
    return true;
}

inline void BitFieldDynamic::set_bit(uint32_t p_bit, uint32_t p_set) {
    DEV_ASSERT(_data);
    uint32_t byte_number = p_bit >> 3; // divide by 8
    DEV_ASSERT(byte_number < _num_bytes);
    uint8_t uc    = _data[byte_number];
    uint32_t mask = 1 << (p_bit & 7);
    if (p_set) {
        uc = uc | mask;
    } else {
        uc &= ~mask;
    }
    _data[byte_number] = uc;
}

#endif
