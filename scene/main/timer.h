// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TIMER_H
#define TIMER_H

#include "scene/main/node.h"

class Timer : public Node {
    GDCLASS(Timer, Node);

    float wait_time;
    bool one_shot;
    bool autostart;
    bool processing;
    bool paused;

    double time_left;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    enum TimerProcessMode {
        TIMER_PROCESS_PHYSICS,
        TIMER_PROCESS_IDLE,
    };

    void set_wait_time(float p_time);
    float get_wait_time() const;

    void set_one_shot(bool p_one_shot);
    bool is_one_shot() const;

    void set_autostart(bool p_start);
    bool has_autostart() const;

    void start(float p_time = -1);
    void stop();

    void set_paused(bool p_paused);
    bool is_paused() const;

    bool is_stopped() const;

    float get_time_left() const;

    String get_configuration_warning() const;

    void set_timer_process_mode(TimerProcessMode p_mode);
    TimerProcessMode get_timer_process_mode() const;
    Timer();

private:
    TimerProcessMode timer_process_mode;
    void _set_process(bool p_process, bool p_force = false);
};

VARIANT_ENUM_CAST(Timer::TimerProcessMode);

#endif // TIMER_H
