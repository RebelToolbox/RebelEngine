// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIOFRAME_H
#define AUDIOFRAME_H

#include "core/math/vector2.h"
#include "core/typedefs.h"

static inline float undenormalise(volatile float f) {
    union {
        uint32_t i;
        float f;
    } v;

    v.f = f;

    // original: return (v.i & 0x7f800000) == 0 ? 0.0f : f;
    // version from Tim Blechmann:
    return (v.i & 0x7f800000) < 0x08000000 ? 0.0f : f;
}

static const float AUDIO_PEAK_OFFSET = 0.0000000001f;
static const float AUDIO_MIN_PEAK_DB = -200.0f; // linear2db(AUDIO_PEAK_OFFSET)

struct AudioFrame {
    // left and right samples
    float l, r;

    _ALWAYS_INLINE_ const float& operator[](int idx) const {
        return idx == 0 ? l : r;
    }

    _ALWAYS_INLINE_ float& operator[](int idx) {
        return idx == 0 ? l : r;
    }

    _ALWAYS_INLINE_ AudioFrame operator+(const AudioFrame& p_frame) const {
        return AudioFrame(l + p_frame.l, r + p_frame.r);
    }

    _ALWAYS_INLINE_ AudioFrame operator-(const AudioFrame& p_frame) const {
        return AudioFrame(l - p_frame.l, r - p_frame.r);
    }

    _ALWAYS_INLINE_ AudioFrame operator*(const AudioFrame& p_frame) const {
        return AudioFrame(l * p_frame.l, r * p_frame.r);
    }

    _ALWAYS_INLINE_ AudioFrame operator/(const AudioFrame& p_frame) const {
        return AudioFrame(l / p_frame.l, r / p_frame.r);
    }

    _ALWAYS_INLINE_ AudioFrame operator+(float p_sample) const {
        return AudioFrame(l + p_sample, r + p_sample);
    }

    _ALWAYS_INLINE_ AudioFrame operator-(float p_sample) const {
        return AudioFrame(l - p_sample, r - p_sample);
    }

    _ALWAYS_INLINE_ AudioFrame operator*(float p_sample) const {
        return AudioFrame(l * p_sample, r * p_sample);
    }

    _ALWAYS_INLINE_ AudioFrame operator/(float p_sample) const {
        return AudioFrame(l / p_sample, r / p_sample);
    }

    _ALWAYS_INLINE_ void operator+=(const AudioFrame& p_frame) {
        l += p_frame.l;
        r += p_frame.r;
    }

    _ALWAYS_INLINE_ void operator-=(const AudioFrame& p_frame) {
        l -= p_frame.l;
        r -= p_frame.r;
    }

    _ALWAYS_INLINE_ void operator*=(const AudioFrame& p_frame) {
        l *= p_frame.l;
        r *= p_frame.r;
    }

    _ALWAYS_INLINE_ void operator/=(const AudioFrame& p_frame) {
        l /= p_frame.l;
        r /= p_frame.r;
    }

    _ALWAYS_INLINE_ void operator+=(float p_sample) {
        l += p_sample;
        r += p_sample;
    }

    _ALWAYS_INLINE_ void operator-=(float p_sample) {
        l -= p_sample;
        r -= p_sample;
    }

    _ALWAYS_INLINE_ void operator*=(float p_sample) {
        l *= p_sample;
        r *= p_sample;
    }

    _ALWAYS_INLINE_ void operator/=(float p_sample) {
        l /= p_sample;
        r /= p_sample;
    }

    _ALWAYS_INLINE_ void undenormalise() {
        l = ::undenormalise(l);
        r = ::undenormalise(r);
    }

    _FORCE_INLINE_ AudioFrame
    linear_interpolate(const AudioFrame& p_b, float p_t) const {
        AudioFrame res = *this;

        res.l += (p_t * (p_b.l - l));
        res.r += (p_t * (p_b.r - r));

        return res;
    }

    _ALWAYS_INLINE_ AudioFrame(float p_l, float p_r) {
        l = p_l;
        r = p_r;
    }

    _ALWAYS_INLINE_ AudioFrame(const AudioFrame& p_frame) {
        l = p_frame.l;
        r = p_frame.r;
    }

    _ALWAYS_INLINE_ AudioFrame operator=(const AudioFrame& p_frame) {
        l = p_frame.l;
        r = p_frame.r;
        return *this;
    }

    _ALWAYS_INLINE_ operator Vector2() const {
        return Vector2(l, r);
    }

    _ALWAYS_INLINE_ AudioFrame(const Vector2& p_v2) {
        l = p_v2.x;
        r = p_v2.y;
    }

    _ALWAYS_INLINE_ AudioFrame() {}
};

#endif
