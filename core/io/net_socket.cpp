// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "net_socket.h"

NetSocket* (*NetSocket::_create)() = nullptr;

NetSocket* NetSocket::create() {
    if (_create) {
        return _create();
    }

    ERR_PRINT("Unable to create network socket, platform not supported");
    return nullptr;
}
