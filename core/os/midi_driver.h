// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MIDI_DRIVER_H
#define MIDI_DRIVER_H

#include "core/typedefs.h"
#include "core/variant.h"

/**
 * Multi-Platform abstraction for accessing to MIDI.
 */

class MIDIDriver {
    static MIDIDriver* singleton;
    static uint8_t last_received_message;

public:
    static MIDIDriver* get_singleton();
    void set_singleton();

    virtual Error open() = 0;
    virtual void close() = 0;

    virtual PoolStringArray get_connected_inputs();

    static void receive_input_packet(
        uint64_t timestamp,
        uint8_t* data,
        uint32_t length
    );

    MIDIDriver();

    virtual ~MIDIDriver() {}
};

#endif
