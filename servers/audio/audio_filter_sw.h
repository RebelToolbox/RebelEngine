// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_FILTER_SW_H
#define AUDIO_FILTER_SW_H

#include "core/math/math_funcs.h"

class AudioFilterSW {
public:
    struct Coeffs {
        float a1, a2;
        float b0, b1, b2;

        // bool operator==(const Coeffs &p_rv) { return (FLOATS_EQ(a1,p_rv.a1)
        // && FLOATS_EQ(a2,p_rv.a2) && FLOATS_EQ(b1,p_rv.b1) &&
        // FLOATS_EQ(b2,p_rv.b2) && FLOATS_EQ(b0,p_rv.b0) ); }
        Coeffs() {
            a1 = a2 = b0 = b1 = b2 = 0.0;
        }
    };

    enum Mode {
        BANDPASS,
        HIGHPASS,
        LOWPASS,
        NOTCH,
        PEAK,
        BANDLIMIT,
        LOWSHELF,
        HIGHSHELF
    };

    class Processor { // simple filter processor

        AudioFilterSW* filter;
        Coeffs coeffs;
        float ha1, ha2, hb1, hb2; // history
        Coeffs incr_coeffs;

    public:
        void set_filter(AudioFilterSW* p_filter, bool p_clear_history = true);
        void process(
            float* p_samples,
            int p_amount,
            int p_stride       = 1,
            bool p_interpolate = false
        );
        void update_coeffs(int p_interp_buffer_len = 0);
        _ALWAYS_INLINE_ void process_one(float& p_sample);
        _ALWAYS_INLINE_ void process_one_interp(float& p_sample);

        Processor();
    };

private:
    float cutoff;
    float resonance;
    float gain;
    float sampling_rate;
    int stages;
    Mode mode;

public:
    float get_response(float p_freq, Coeffs* p_coeffs);

    void set_mode(Mode p_mode);
    void set_cutoff(float p_cutoff);
    void set_resonance(float p_resonance);
    void set_gain(float p_gain);
    void set_sampling_rate(float p_srate);
    void set_stages(int p_stages); // adjust for multiple stages

    void prepare_coefficients(Coeffs* p_coeffs);

    AudioFilterSW();
};

/* inline methods */

void AudioFilterSW::Processor::process_one(float& p_sample) {
    float pre = p_sample;
    p_sample =
        (p_sample * coeffs.b0 + hb1 * coeffs.b1 + hb2 * coeffs.b2
         + ha1 * coeffs.a1 + ha2 * coeffs.a2);
    ha2 = ha1;
    hb2 = hb1;
    hb1 = pre;
    ha1 = p_sample;
}

void AudioFilterSW::Processor::process_one_interp(float& p_sample) {
    float pre = p_sample;
    p_sample =
        (p_sample * coeffs.b0 + hb1 * coeffs.b1 + hb2 * coeffs.b2
         + ha1 * coeffs.a1 + ha2 * coeffs.a2);
    ha2 = ha1;
    hb2 = hb1;
    hb1 = pre;
    ha1 = p_sample;

    coeffs.b0 += incr_coeffs.b0;
    coeffs.b1 += incr_coeffs.b1;
    coeffs.b2 += incr_coeffs.b2;
    coeffs.a1 += incr_coeffs.a1;
    coeffs.a2 += incr_coeffs.a2;
}

#endif // AUDIO_FILTER_SW_H
