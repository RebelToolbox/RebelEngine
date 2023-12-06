// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "visual_server_callbacks.h"

#include "core/object.h"

void VisualServerCallbacks::lock() {
    mutex.lock();
}

void VisualServerCallbacks::unlock() {
    mutex.unlock();
}

void VisualServerCallbacks::flush() {
    // should be ok without a lock ..
    // is the most common case and should be quicker
    if (!messages.size()) {
        return;
    }

    lock();
    for (int n = 0; n < messages.size(); n++) {
        const Message& mess = messages[n];

        Object* obj = ObjectDB::get_instance(mess.object_id);
        if (!obj) {
            continue;
        }

        obj->notification_callback(mess.type);
    }

    messages.clear();

    unlock();
}
