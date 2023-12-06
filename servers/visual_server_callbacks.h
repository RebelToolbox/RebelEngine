// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VISUAL_SERVER_CALLBACKS_H
#define VISUAL_SERVER_CALLBACKS_H

#include "core/local_vector.h"
#include "core/object_id.h"
#include "core/os/mutex.h"

class VisualServerCallbacks {
public:
    enum CallbackType {
        CALLBACK_NOTIFICATION_ENTER_GAMEPLAY,
        CALLBACK_NOTIFICATION_EXIT_GAMEPLAY,
        CALLBACK_SIGNAL_ENTER_GAMEPLAY,
        CALLBACK_SIGNAL_EXIT_GAMEPLAY,
    };

    struct Message {
        CallbackType type;
        ObjectID object_id;
    };

    void lock();
    void unlock();
    void flush();

    void push_message(const Message& p_message) {
        messages.push_back(p_message);
    }

    int32_t get_num_messages() const {
        return messages.size();
    }

    const Message& get_message(int p_index) const {
        return messages[p_index];
    }

    void clear() {
        messages.clear();
    }

private:
    LocalVector<Message, int32_t> messages;
    Mutex mutex;
};

#endif // VISUAL_SERVER_CALLBACKS_H
