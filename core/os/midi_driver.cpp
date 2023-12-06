// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "midi_driver.h"

#include "core/os/os.h"
#include "main/input_default.h"

uint8_t MIDIDriver::last_received_message = 0x00;
MIDIDriver* MIDIDriver::singleton         = nullptr;

MIDIDriver* MIDIDriver::get_singleton() {
    return singleton;
}

void MIDIDriver::set_singleton() {
    singleton = this;
}

void MIDIDriver::receive_input_packet(
    uint64_t timestamp,
    uint8_t* data,
    uint32_t length
) {
    Ref<InputEventMIDI> event;
    event.instance();
    uint32_t param_position = 1;

    if (length >= 1) {
        if (data[0] >= 0xF0) {
            // channel does not apply to system common messages
            event->set_channel(0);
            event->set_message(data[0]);
            last_received_message = data[0];
        } else if ((data[0] & 0x80) == 0x00) {
            // running status
            event->set_channel(last_received_message & 0xF);
            event->set_message(last_received_message >> 4);
            param_position = 0;
        } else {
            event->set_channel(data[0] & 0xF);
            event->set_message(data[0] >> 4);
            param_position        = 1;
            last_received_message = data[0];
        }
    }

    switch (event->get_message()) {
        case MIDI_MESSAGE_AFTERTOUCH:
            if (length >= 2 + param_position) {
                event->set_pitch(data[param_position]);
                event->set_pressure(data[param_position + 1]);
            }
            break;

        case MIDI_MESSAGE_CONTROL_CHANGE:
            if (length >= 2 + param_position) {
                event->set_controller_number(data[param_position]);
                event->set_controller_value(data[param_position + 1]);
            }
            break;

        case MIDI_MESSAGE_NOTE_ON:
        case MIDI_MESSAGE_NOTE_OFF:
            if (length >= 2 + param_position) {
                event->set_pitch(data[param_position]);
                event->set_velocity(data[param_position + 1]);

                if (event->get_message() == MIDI_MESSAGE_NOTE_ON
                    && event->get_velocity() == 0) {
                    // https://www.midi.org/forum/228-writing-midi-software-send-note-off,-or-zero-velocity-note-on
                    event->set_message(MIDI_MESSAGE_NOTE_OFF);
                }
            }
            break;

        case MIDI_MESSAGE_PITCH_BEND:
            if (length >= 2 + param_position) {
                event->set_pitch(
                    (data[param_position + 1] << 7) | data[param_position]
                );
            }
            break;

        case MIDI_MESSAGE_PROGRAM_CHANGE:
            if (length >= 1 + param_position) {
                event->set_instrument(data[param_position]);
            }
            break;

        case MIDI_MESSAGE_CHANNEL_PRESSURE:
            if (length >= 1 + param_position) {
                event->set_pressure(data[param_position]);
            }
            break;
    }

    InputDefault* id = Object::cast_to<InputDefault>(Input::get_singleton());
    id->parse_input_event(event);
}

PoolStringArray MIDIDriver::get_connected_inputs() {
    PoolStringArray list;
    return list;
}

MIDIDriver::MIDIDriver() {
    set_singleton();
}
