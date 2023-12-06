// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef INTERPOLATED_CAMERA_H
#define INTERPOLATED_CAMERA_H

#include "scene/3d/camera.h"

class InterpolatedCamera : public Camera {
    GDCLASS(InterpolatedCamera, Camera);

    bool enabled;
    real_t speed;
    NodePath target;

protected:
    void _notification(int p_what);
    static void _bind_methods();
    void _set_target(const Object* p_target);

public:
    void set_target(const Spatial* p_target);
    void set_target_path(const NodePath& p_path);
    NodePath get_target_path() const;

    void set_speed(real_t p_speed);
    real_t get_speed() const;

    void set_interpolation_enabled(bool p_enable);
    bool is_interpolation_enabled() const;

    InterpolatedCamera();
};

#endif // INTERPOLATED_CAMERA_H
