// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef COREMIDI_ENABLED

#ifndef MIDI_DRIVER_COREMIDI_H
#define MIDI_DRIVER_COREMIDI_H

#include "core/os/midi_driver.h"
#include "core/vector.h"

#include <CoreMIDI/CoreMIDI.h>
#include <stdio.h>

class MIDIDriverCoreMidi : public MIDIDriver {
    MIDIClientRef client;
    MIDIPortRef port_in;

    Vector<MIDIEndpointRef> connected_sources;

    static void read(
        const MIDIPacketList* packet_list,
        void* read_proc_ref_con,
        void* src_conn_ref_con
    );

public:
    virtual Error open();
    virtual void close();

    PoolStringArray get_connected_inputs();

    MIDIDriverCoreMidi();
    virtual ~MIDIDriverCoreMidi();
};

#endif // MIDI_DRIVER_COREMIDI_H
#endif // COREMIDI_ENABLED
