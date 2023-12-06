// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/class_db.h"
#include "jsonrpc.h"

void register_jsonrpc_types() {
    ClassDB::register_class<JSONRPC>();
}

void unregister_jsonrpc_types() {}
