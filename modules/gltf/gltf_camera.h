// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_CAMERA_H
#define GLTF_CAMERA_H

#include "core/resource.h"

class GLTFCamera : public Resource {
    GDCLASS(GLTFCamera, Resource);

private:
    bool perspective = true;
    float fov_size   = 75.f;
    float zfar       = 4000.f;
    float znear      = 0.05f;

protected:
    static void _bind_methods();

public:
    bool get_perspective() const {
        return perspective;
    }

    void set_perspective(bool p_val) {
        perspective = p_val;
    }

    float get_fov_size() const {
        return fov_size;
    }

    void set_fov_size(float p_val) {
        fov_size = p_val;
    }

    float get_zfar() const {
        return zfar;
    }

    void set_zfar(float p_val) {
        zfar = p_val;
    }

    float get_znear() const {
        return znear;
    }

    void set_znear(float p_val) {
        znear = p_val;
    }
};
#endif // GLTF_CAMERA_H
