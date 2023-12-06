// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PORTAL_PVS_H
#define PORTAL_PVS_H

#include "core/local_vector.h"

class PVS {
public:
    void clear();

    void add_to_pvs(int p_room_id) {
        _room_pvs.push_back(p_room_id);
    }

    int32_t get_pvs_size() const {
        return _room_pvs.size();
    }

    int32_t get_pvs_room_id(int32_t p_entry) const {
        return _room_pvs[p_entry];
    }

    void add_to_secondary_pvs(int p_room_id) {
        _room_secondary_pvs.push_back(p_room_id);
    }

    int32_t get_secondary_pvs_size() const {
        return _room_secondary_pvs.size();
    }

    int32_t get_secondary_pvs_room_id(int32_t p_entry) const {
        return _room_secondary_pvs[p_entry];
    }

    void set_loaded(bool p_loaded) {
        _loaded = p_loaded;
    }

    bool is_loaded() const {
        return _loaded;
    }

private:
    // pvs
    LocalVector<uint16_t, int32_t> _room_pvs;
    // secondary pvs is primary plus the immediate neighbors of the primary pvs
    LocalVector<uint16_t, int32_t> _room_secondary_pvs;
    bool _loaded = false;
};

#endif
