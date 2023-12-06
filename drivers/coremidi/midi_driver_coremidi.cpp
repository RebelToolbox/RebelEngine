// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef COREMIDI_ENABLED

#include "midi_driver_coremidi.h"

#include "core/print_string.h"

#include <CoreAudio/HostTime.h>
#include <CoreServices/CoreServices.h>

void MIDIDriverCoreMidi::read(
    const MIDIPacketList* packet_list,
    void* read_proc_ref_con,
    void* src_conn_ref_con
) {
    MIDIPacket* packet = const_cast<MIDIPacket*>(packet_list->packet);
    for (UInt32 i = 0; i < packet_list->numPackets; i++) {
        receive_input_packet(packet->timeStamp, packet->data, packet->length);
        packet = MIDIPacketNext(packet);
    }
}

Error MIDIDriverCoreMidi::open() {
    CFStringRef name =
        CFStringCreateWithCString(NULL, "Godot", kCFStringEncodingASCII);
    OSStatus result = MIDIClientCreate(name, NULL, NULL, &client);
    CFRelease(name);
    if (result != noErr) {
        ERR_PRINT("MIDIClientCreate failed, code: " + itos(result));
        return ERR_CANT_OPEN;
    }

    result = MIDIInputPortCreate(
        client,
        CFSTR("Godot Input"),
        MIDIDriverCoreMidi::read,
        (void*)this,
        &port_in
    );
    if (result != noErr) {
        ERR_PRINT("MIDIInputPortCreate failed, code: " + itos(result));
        return ERR_CANT_OPEN;
    }

    int sources = MIDIGetNumberOfSources();
    for (int i = 0; i < sources; i++) {
        MIDIEndpointRef source = MIDIGetSource(i);
        if (source) {
            MIDIPortConnectSource(port_in, source, (void*)this);
            connected_sources.insert(i, source);
        }
    }

    return OK;
}

void MIDIDriverCoreMidi::close() {
    for (int i = 0; i < connected_sources.size(); i++) {
        MIDIEndpointRef source = connected_sources[i];
        MIDIPortDisconnectSource(port_in, source);
    }
    connected_sources.clear();

    if (port_in != 0) {
        MIDIPortDispose(port_in);
        port_in = 0;
    }

    if (client != 0) {
        MIDIClientDispose(client);
        client = 0;
    }
}

PoolStringArray MIDIDriverCoreMidi::get_connected_inputs() {
    PoolStringArray list;

    for (int i = 0; i < connected_sources.size(); i++) {
        MIDIEndpointRef source = connected_sources[i];
        CFStringRef ref        = NULL;
        char name[256];

        MIDIObjectGetStringProperty(source, kMIDIPropertyDisplayName, &ref);
        CFStringGetCString(ref, name, sizeof(name), kCFStringEncodingUTF8);
        CFRelease(ref);

        list.push_back(name);
    }

    return list;
}

MIDIDriverCoreMidi::MIDIDriverCoreMidi() : client(0) {}

MIDIDriverCoreMidi::~MIDIDriverCoreMidi() {
    close();
}

#endif // COREMIDI_ENABLED
