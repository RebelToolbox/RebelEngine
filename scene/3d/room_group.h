// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ROOM_GROUP_H
#define ROOM_GROUP_H

#include "core/rid.h"
#include "spatial.h"

class Room;

class RoomGroup : public Spatial {
    GDCLASS(RoomGroup, Spatial);

    friend class RoomManager;

    RID _room_group_rid;

public:
    RoomGroup();
    ~RoomGroup();

    void add_room(Room* p_room);

    void set_roomgroup_priority(int p_priority) {
        _settings_priority = p_priority;
        _changed();
    }

    int get_roomgroup_priority() const {
        return _settings_priority;
    }

    String get_configuration_warning() const;

private:
    void clear();
    void _changed();

    // roomgroup ID during conversion
    int _roomgroup_ID;

    // the roomgroup can be used to set a number of rooms to a different
    // priority to allow a group of rooms WITHIN another room / rooms. This is
    // for e.g. buildings on landscape.
    int _settings_priority = 0;

    // makes sure lrooms are not converted more than once per
    // call to rooms_convert
    int _conversion_tick = -1;

protected:
    static void _bind_methods();
    void _notification(int p_what);
};

#endif
