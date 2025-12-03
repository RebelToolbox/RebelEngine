// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_LIGHT_H
#define GLTF_LIGHT_H

#include "core/engine.h"
#include "core/resource.h"

class GLTFLight : public Resource {
    GDCLASS(GLTFLight, Resource)
    friend class GLTFDocument;

protected:
    static void _bind_methods();

private:
    Color color     = Color(1.0f, 1.0f, 1.0f);
    float intensity = 1.0f;
    String type;
    float range            = INFINITY;
    float inner_cone_angle = 0.0f;
    float outer_cone_angle = static_cast<float>(Math_TAU) / 8.0f;

public:
    Color get_color();
    void set_color(Color p_color);

    float get_intensity();
    void set_intensity(float p_intensity);

    String get_type();
    void set_type(String p_type);

    float get_range();
    void set_range(float p_range);

    float get_inner_cone_angle();
    void set_inner_cone_angle(float p_inner_cone_angle);

    float get_outer_cone_angle();
    void set_outer_cone_angle(float p_outer_cone_angle);
};

#endif // GLTF_LIGHT_H
