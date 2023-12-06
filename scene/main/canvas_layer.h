// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CANVAS_LAYER_H
#define CANVAS_LAYER_H

#include "scene/main/node.h"
#include "scene/resources/world_2d.h"

class Viewport;

class CanvasLayer : public Node {
    GDCLASS(CanvasLayer, Node);

    bool locrotscale_dirty;
    Vector2 ofs;
    Size2 scale;
    real_t rot;
    int layer;
    Transform2D transform;
    RID canvas;

    ObjectID custom_viewport_id; // to check validity
    Viewport* custom_viewport;

    RID viewport;
    Viewport* vp;

    int sort_index;

    bool follow_viewport;
    float follow_viewport_scale;

    void _update_xform();
    void _update_locrotscale();
    void _update_follow_viewport(bool p_force_exit = false);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_layer(int p_xform);
    int get_layer() const;

    void set_transform(const Transform2D& p_xform);
    Transform2D get_transform() const;

    void set_offset(const Vector2& p_offset);
    Vector2 get_offset() const;

    void set_rotation(real_t p_radians);
    real_t get_rotation() const;

    void set_rotation_degrees(real_t p_degrees);
    real_t get_rotation_degrees() const;

    void set_scale(const Size2& p_scale);
    Size2 get_scale() const;

    Size2 get_viewport_size() const;

    RID get_viewport() const;

    void set_custom_viewport(Node* p_viewport);
    Node* get_custom_viewport() const;

    void reset_sort_index();
    int get_sort_index();

    void set_follow_viewport(bool p_enable);
    bool is_following_viewport() const;

    void set_follow_viewport_scale(float p_ratio);
    float get_follow_viewport_scale() const;

    RID get_canvas() const;

    CanvasLayer();
    ~CanvasLayer();
};

#endif // CANVAS_LAYER_H
