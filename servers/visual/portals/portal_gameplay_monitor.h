// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PORTAL_GAMEPLAY_MONITOR_H
#define PORTAL_GAMEPLAY_MONITOR_H

#include "core/local_vector.h"
#include "servers/visual_server_callbacks.h"

#include <stdint.h>

class PortalRenderer;
struct VSRoom;

class PortalGameplayMonitor {
public:
    PortalGameplayMonitor();

    void unload(PortalRenderer& p_portal_renderer);

    // entering and exiting gameplay notifications (requires PVS)
    void update_gameplay(
        PortalRenderer& p_portal_renderer,
        const int* p_source_room_ids,
        int p_num_source_rooms
    );
    void set_params(bool p_use_secondary_pvs, bool p_use_signals);

private:
    void _update_gameplay_room(
        PortalRenderer& p_portal_renderer,
        int p_room_id,
        bool p_source_rooms_changed
    );
    bool _source_rooms_changed(
        const int* p_source_room_ids,
        int p_num_source_rooms
    );
    void _swap(bool p_source_rooms_changed);

    // gameplay ticks happen every physics tick
    uint32_t _gameplay_tick = 1;

    // Room ticks only happen when the rooms the cameras are within change.
    // This is an optimization. This tick needs to be maintained separately from
    // _gameplay_tick because testing against the previous tick is used to
    // determine whether to send enter or exit gameplay notifications, and this
    // must be synchronized differently for rooms, roomgroups and static ghosts.
    uint32_t _room_tick = 1;

    // we need two version, current and previous
    LocalVector<uint32_t, int32_t> _active_moving_pool_ids[2];
    LocalVector<uint32_t, int32_t>* _active_moving_pool_ids_curr;
    LocalVector<uint32_t, int32_t>* _active_moving_pool_ids_prev;

    LocalVector<uint32_t, int32_t> _active_rghost_pool_ids[2];
    LocalVector<uint32_t, int32_t>* _active_rghost_pool_ids_curr;
    LocalVector<uint32_t, int32_t>* _active_rghost_pool_ids_prev;

    LocalVector<uint32_t, int32_t> _active_room_ids[2];
    LocalVector<uint32_t, int32_t>* _active_room_ids_curr;
    LocalVector<uint32_t, int32_t>* _active_room_ids_prev;

    LocalVector<uint32_t, int32_t> _active_roomgroup_ids[2];
    LocalVector<uint32_t, int32_t>* _active_roomgroup_ids_curr;
    LocalVector<uint32_t, int32_t>* _active_roomgroup_ids_prev;

    LocalVector<uint32_t, int32_t> _active_sghost_ids[2];
    LocalVector<uint32_t, int32_t>* _active_sghost_ids_curr;
    LocalVector<uint32_t, int32_t>* _active_sghost_ids_prev;

    LocalVector<uint32_t, int32_t> _source_rooms_prev;

    VisualServerCallbacks::CallbackType _enter_callback_type =
        VisualServerCallbacks::CALLBACK_NOTIFICATION_ENTER_GAMEPLAY;
    VisualServerCallbacks::CallbackType _exit_callback_type =
        VisualServerCallbacks::CALLBACK_NOTIFICATION_EXIT_GAMEPLAY;

    bool _use_secondary_pvs = false;
    bool _use_signals       = false;
};

#endif
