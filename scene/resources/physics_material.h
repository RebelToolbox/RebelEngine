// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef physics_material_override_H
#define physics_material_override_H

#include "core/resource.h"
#include "servers/physics_server.h"

class PhysicsMaterial : public Resource {
    GDCLASS(PhysicsMaterial, Resource);
    OBJ_SAVE_TYPE(PhysicsMaterial);
    RES_BASE_EXTENSION("phymat");

    real_t friction;
    bool rough;
    real_t bounce;
    bool absorbent;

protected:
    static void _bind_methods();

public:
    void set_friction(real_t p_val);

    _FORCE_INLINE_ real_t get_friction() const {
        return friction;
    }

    void set_rough(bool p_val);

    _FORCE_INLINE_ bool is_rough() const {
        return rough;
    }

    _FORCE_INLINE_ real_t computed_friction() const {
        return rough ? -friction : friction;
    }

    void set_bounce(real_t p_val);

    _FORCE_INLINE_ real_t get_bounce() const {
        return bounce;
    }

    void set_absorbent(bool p_val);

    _FORCE_INLINE_ bool is_absorbent() const {
        return absorbent;
    }

    _FORCE_INLINE_ real_t computed_bounce() const {
        return absorbent ? -bounce : bounce;
    }

    PhysicsMaterial();
};

#endif // physics_material_override_H
