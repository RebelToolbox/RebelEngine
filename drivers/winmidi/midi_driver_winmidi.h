// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef WINMIDI_ENABLED

#ifndef MIDI_DRIVER_WINMIDI_H
#define MIDI_DRIVER_WINMIDI_H

#include "core/os/midi_driver.h"
#include "core/vector.h"

#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef WIN32_LEAN_AND_MEAN
// Manually include <mmsystem.h> after <windows.h> if using WIN32_LEAN_AND_MEAN
#include <mmsystem.h>
#endif // WIN32_LEAN_AND_MEAN

class MIDIDriverWinMidi : public MIDIDriver {
    Vector<HMIDIIN> connected_sources;

    static void CALLBACK read(
        HMIDIIN hMidiIn,
        UINT wMsg,
        DWORD_PTR dwInstance,
        DWORD_PTR dwParam1,
        DWORD_PTR dwParam2
    );

public:
    virtual Error open();
    virtual void close();

    virtual PoolStringArray get_connected_inputs();

    MIDIDriverWinMidi();
    virtual ~MIDIDriverWinMidi();
};

#endif // MIDI_DRIVER_WINMIDI_H
#endif // WINMIDI_ENABLED
