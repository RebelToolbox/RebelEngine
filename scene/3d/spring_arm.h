// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SPRING_ARM_H
#define SPRING_ARM_H

#include "scene/3d/spatial.h"

class SpringArm : public Spatial {
    GDCLASS(SpringArm, Spatial);

    Ref<Shape> shape;
    Set<RID> excluded_objects;
    float spring_length;
    float current_spring_length;
    bool keep_child_basis;
    uint32_t mask;
    float margin;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_length(float p_length);
    float get_length() const;
    void set_shape(Ref<Shape> p_shape);
    Ref<Shape> get_shape() const;
    void set_mask(uint32_t p_mask);
    uint32_t get_mask();
    void add_excluded_object(RID p_rid);
    bool remove_excluded_object(RID p_rid);
    void clear_excluded_objects();
    float get_hit_length();
    void set_margin(float p_margin);
    float get_margin();

    SpringArm();

private:
    void process_spring();
};

#endif
