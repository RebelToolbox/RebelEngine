// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "room_group.h"

#include "room.h"
#include "room_manager.h"

void RoomGroup::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_roomgroup_priority", "p_priority"),
        &RoomGroup::set_roomgroup_priority
    );
    ClassDB::bind_method(
        D_METHOD("get_roomgroup_priority"),
        &RoomGroup::get_roomgroup_priority
    );

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "roomgroup_priority",
            PROPERTY_HINT_RANGE,
            "-16,16,1",
            PROPERTY_USAGE_DEFAULT
        ),
        "set_roomgroup_priority",
        "get_roomgroup_priority"
    );
}

RoomGroup::RoomGroup() {
    _room_group_rid = VisualServer::get_singleton()->roomgroup_create();
}

RoomGroup::~RoomGroup() {
    if (_room_group_rid != RID()) {
        VisualServer::get_singleton()->free(_room_group_rid);
    }
}

String RoomGroup::get_configuration_warning() const {
    String warning = Spatial::get_configuration_warning();

    if (Room::detect_nodes_of_type<RoomManager>(this)) {
        if (!warning.empty()) {
            warning += "\n\n";
        }
        warning +=
            TTR("The RoomManager should not be placed inside a RoomGroup.");
    }

    return warning;
}

void RoomGroup::clear() {
    _roomgroup_ID = -1;
}

void RoomGroup::add_room(Room* p_room) {
    VisualServer::get_singleton()->roomgroup_add_room(
        _room_group_rid,
        p_room->_room_rid
    );
}

// extra editor links to the room manager to allow unloading
// on change, or re-converting
void RoomGroup::_changed() {
#ifdef TOOLS_ENABLED
    RoomManager* rm = RoomManager::active_room_manager;
    if (!rm) {
        return;
    }

    rm->_rooms_changed("changed RoomGroup " + get_name());
#endif
}

void RoomGroup::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_WORLD: {
            ERR_FAIL_COND(get_world().is_null());
            VisualServer::get_singleton()->roomgroup_set_scenario(
                _room_group_rid,
                get_world()->get_scenario()
            );
        } break;
        case NOTIFICATION_EXIT_WORLD: {
            VisualServer::get_singleton()->roomgroup_set_scenario(
                _room_group_rid,
                RID()
            );
        } break;
    }
}
