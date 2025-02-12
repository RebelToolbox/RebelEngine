// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MAIN_TIMER_SYNC_H
#define MAIN_TIMER_SYNC_H

#include "core/engine.h"

struct MainFrameTime {
    float idle_step;   // time to advance idles for (argument to process())
    int physics_steps; // number of times to iterate the physics engine
    float interpolation_fraction; // fraction through the current physics tick

    void clamp_idle(float min_idle_step, float max_idle_step);
};

class MainTimerSync {
    class DeltaSmoother {
    public:
        // pass the recorded delta, returns a smoothed delta
        int64_t smooth_delta(int64_t p_delta);

    private:
        void update_refresh_rate_estimator(int64_t p_delta);
        bool fps_allows_smoothing(int64_t p_delta);

        // estimated vsync delta (monitor refresh rate)
        int64_t _vsync_delta = 16666;

        // keep track of accumulated time so we know how many vsyncs to advance
        // by
        int64_t _leftover_time = 0;

        // keep a rough measurement of the FPS as we run.
        // If this drifts a long way below or above the refresh rate, the
        // machine is struggling to keep up, and we can switch off smoothing.
        // This also deals with the case that the user has overridden the vsync
        // in the GPU settings, in which case we don't want to try smoothing.
        static const int MEASURE_FPS_OVER_NUM_FRAMES = 64;

        int64_t _measurement_time          = 0;
        int64_t _measurement_frame_count   = 0;
        int64_t _measurement_end_frame     = MEASURE_FPS_OVER_NUM_FRAMES;
        int64_t _measurement_start_time    = 0;
        bool _measurement_allows_smoothing = true;

        // we can estimate the fps by growing it on condition
        // that a large proportion of frames are higher than the current
        // estimate.
        int32_t _estimated_fps            = 0;
        int32_t _hits_at_estimated        = 0;
        int32_t _hits_above_estimated     = 0;
        int32_t _hits_below_estimated     = 0;
        int32_t _hits_one_above_estimated = 0;
        int32_t _hits_one_below_estimated = 0;
        bool _estimate_complete           = false;
        bool _estimate_locked             = false;

        // data for averaging the delta over a second or so
        // to prevent spurious values
        int64_t _estimator_total_delta    = 0;
        int32_t _estimator_delta_readings = 0;

        void made_new_estimate() {
            _hits_above_estimated     = 0;
            _hits_at_estimated        = 0;
            _hits_below_estimated     = 0;
            _hits_one_above_estimated = 0;
            _hits_one_below_estimated = 0;

            _estimate_complete = false;
        }

    } _delta_smoother;

    // wall clock time measured on the main thread
    uint64_t last_cpu_ticks_usec;
    uint64_t current_cpu_ticks_usec;

    // logical game time since last physics timestep
    float time_accum;

    // current difference between wall clock time and reported sum of idle_steps
    float time_deficit;

    // number of frames back for keeping accumulated physics steps roughly
    // constant. value of 12 chosen because that is what is required to make 144
    // Hz monitors behave well with 60 Hz physics updates. The only worse
    // commonly available refresh would be 85, requiring CONTROL_STEPS = 17.
    static const int CONTROL_STEPS = 12;

    // sum of physics steps done over the last (i+1) frames
    int accumulated_physics_steps[CONTROL_STEPS];

    // typical value for accumulated_physics_steps[i] is either this or this
    // plus one
    int typical_physics_steps[CONTROL_STEPS];

    int fixed_fps;

protected:
    // returns the fraction of p_frame_slice required for the timer to overshoot
    // before advance_core considers changing the physics_steps return from
    // the typical values as defined by typical_physics_steps
    float get_physics_jitter_fix();

    // gets our best bet for the average number of physics steps per render
    // frame return value: number of frames back this data is consistent
    int get_average_physics_steps(float& p_min, float& p_max);

    // advance physics clock by p_idle_step, return appropriate number of steps
    // to simulate
    MainFrameTime advance_core(
        float p_frame_slice,
        int p_iterations_per_second,
        float p_idle_step
    );

    // calls advance_core, keeps track of deficit it adds to animaption_step,
    // make sure the deficit sum stays close to zero
    MainFrameTime advance_checked(
        float p_frame_slice,
        int p_iterations_per_second,
        float p_idle_step
    );

    // determine wall clock step since last iteration
    float get_cpu_idle_step();

public:
    MainTimerSync();

    // start the clock
    void init(uint64_t p_cpu_ticks_usec);
    // set measured wall clock time
    void set_cpu_ticks_usec(uint64_t p_cpu_ticks_usec);
    // set fixed fps
    void set_fixed_fps(int p_fixed_fps);

    // advance one frame, return timesteps to take
    MainFrameTime advance(float p_frame_slice, int p_iterations_per_second);
};

#endif // MAIN_TIMER_SYNC_H
