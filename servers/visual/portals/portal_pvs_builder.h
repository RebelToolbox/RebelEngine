// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PORTAL_PVS_BUILDER_H
#define PORTAL_PVS_BUILDER_H

#include "core/bitfield_dynamic.h"
#include "core/local_vector.h"
#include "core/math/plane.h"

class PortalRenderer;
class PVS;

class PVSBuilder {
    struct Neighbours {
        LocalVector<int32_t, int32_t> room_ids;
    };

public:
    void calculate_pvs(
        PortalRenderer& p_portal_renderer,
        String p_filename,
        int p_depth_limit,
        bool p_use_simple_pvs,
        bool p_log_pvs_generation
    );

private:
    void find_neighbors(LocalVector<Neighbours>& r_neighbors);

    void logd(int p_depth, String p_string);
    void log(String p_string);

    void trace_rooms_recursive(
        int p_depth,
        int p_source_room_id,
        int p_room_id,
        int p_first_portal_id,
        bool p_first_portal_outgoing,
        int p_previous_portal_id,
        const LocalVector<Plane, int32_t>& p_planes,
        BitFieldDynamic& r_bitfield_rooms,
        int p_from_external_room_id = -1
    );
    void trace_rooms_recursive_simple(
        int p_depth,
        int p_source_room_id,
        int p_room_id,
        int p_first_portal_id,
        bool p_first_portal_outgoing,
        int p_previous_portal_id,
        const LocalVector<Plane, int32_t>& p_planes,
        BitFieldDynamic& r_bitfield_rooms
    );

    void create_secondary_pvs(
        int p_room_id,
        const LocalVector<Neighbours>& p_neighbors,
        BitFieldDynamic& r_bitfield_rooms
    );

    PortalRenderer* _portal_renderer = nullptr;
    PVS* _pvs                        = nullptr;
    int _depth_limit                 = 16;
    Vector3 _trace_start_point;

    static bool _log_active;
};

#endif
