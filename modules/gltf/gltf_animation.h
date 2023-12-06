// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_ANIMATION_H
#define GLTF_ANIMATION_H

#include "core/resource.h"

class GLTFAnimation : public Resource {
    GDCLASS(GLTFAnimation, Resource);

protected:
    static void _bind_methods();

public:
    enum Interpolation {
        INTERP_LINEAR,
        INTERP_STEP,
        INTERP_CATMULLROMSPLINE,
        INTERP_CUBIC_SPLINE,
    };

    template <class T>
    struct Channel {
        Interpolation interpolation;
        Vector<float> times;
        Vector<T> values;
    };

    struct Track {
        Channel<Vector3> translation_track;
        Channel<Quat> rotation_track;
        Channel<Vector3> scale_track;
        Vector<Channel<float>> weight_tracks;
    };

public:
    bool get_loop() const;
    void set_loop(bool p_val);
    Map<int, GLTFAnimation::Track>& get_tracks();
    GLTFAnimation();

private:
    bool loop = false;
    Map<int, Track> tracks;
};
#endif // GLTF_ANIMATION_H
