// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

// author: Andreas Haas <hondres,  liugam3@gmail.com>
#ifndef LINUX_JOYPAD_H
#define LINUX_JOYPAD_H

#ifdef JOYDEV_ENABLED
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "main/input_default.h"

struct input_absinfo;

class LinuxJoypad {
public:
    LinuxJoypad(InputDefault* in);
    ~LinuxJoypad();
    void process_joypads();

private:
    enum {
        JOYPADS_MAX = 16,
        MAX_ABS     = 63,
        MAX_KEY = 767, // Hack because <linux/input.h> can't be included here
    };

    struct Joypad {
        InputDefault::JoyAxis curr_axis[MAX_ABS];
        int key_map[MAX_KEY];
        int abs_map[MAX_ABS];
        int dpad;
        int fd;

        String devpath;
        input_absinfo* abs_info[MAX_ABS];

        bool force_feedback;
        int ff_effect_id;
        uint64_t ff_effect_timestamp;

        Joypad();
        ~Joypad();
        void reset();
    };

#ifdef UDEV_ENABLED
    bool use_udev;
#endif
    SafeFlag exit_monitor;
    Mutex joy_mutex;
    Thread joy_thread;
    InputDefault* input;
    Joypad joypads[JOYPADS_MAX];
    Vector<String> attached_devices;

    static void joy_thread_func(void* p_user);

    int get_joy_from_path(String p_path) const;

    void setup_joypad_properties(int p_id);
    void close_joypad(int p_id = -1);
#ifdef UDEV_ENABLED
    void enumerate_joypads(struct udev* p_udev);
    void monitor_joypads(struct udev* p_udev);
#endif
    void monitor_joypads();
    void run_joypad_thread();
    void open_joypad(const char* p_path);

    void joypad_vibration_start(
        int p_id,
        float p_weak_magnitude,
        float p_strong_magnitude,
        float p_duration,
        uint64_t p_timestamp
    );
    void joypad_vibration_stop(int p_id, uint64_t p_timestamp);

    InputDefault::JoyAxis axis_correct(const input_absinfo* p_abs, int p_value)
        const;
};

#endif
#endif // LINUX_JOYPAD_H
