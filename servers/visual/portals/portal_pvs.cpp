// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "portal_pvs.h"

void PVS::clear() {
    _room_pvs.clear();
    _room_secondary_pvs.clear();
    _loaded = false;
}
