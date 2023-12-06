// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LIGHTOCCLUDER2D_H
#define LIGHTOCCLUDER2D_H

#include "scene/2d/node_2d.h"

class OccluderPolygon2D : public Resource {
    GDCLASS(OccluderPolygon2D, Resource);

public:
    enum CullMode {
        CULL_DISABLED,
        CULL_CLOCKWISE,
        CULL_COUNTER_CLOCKWISE
    };

private:
    RID occ_polygon;
    PoolVector<Vector2> polygon;
    bool closed;
    CullMode cull;

    mutable Rect2 item_rect;
    mutable bool rect_cache_dirty;

protected:
    static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
    virtual Rect2 _edit_get_rect() const;
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const;
#endif

    void set_polygon(const PoolVector<Vector2>& p_polygon);
    PoolVector<Vector2> get_polygon() const;

    void set_closed(bool p_closed);
    bool is_closed() const;

    void set_cull_mode(CullMode p_mode);
    CullMode get_cull_mode() const;

    virtual RID get_rid() const;
    OccluderPolygon2D();
    ~OccluderPolygon2D();
};

VARIANT_ENUM_CAST(OccluderPolygon2D::CullMode);

class LightOccluder2D : public Node2D {
    GDCLASS(LightOccluder2D, Node2D);

    RID occluder;
    bool enabled;
    int mask;
    Ref<OccluderPolygon2D> occluder_polygon;

    void _poly_changed();

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
    virtual Rect2 _edit_get_rect() const;
    virtual bool _edit_is_selected_on_click(
        const Point2& p_point,
        double p_tolerance
    ) const;
#endif

    void set_occluder_polygon(const Ref<OccluderPolygon2D>& p_polygon);
    Ref<OccluderPolygon2D> get_occluder_polygon() const;

    void set_occluder_light_mask(int p_mask);
    int get_occluder_light_mask() const;

    String get_configuration_warning() const;

    LightOccluder2D();
    ~LightOccluder2D();
};

#endif // LIGHTOCCLUDER2D_H
