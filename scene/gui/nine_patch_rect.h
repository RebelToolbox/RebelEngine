// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NINE_PATCH_RECT_H
#define NINE_PATCH_RECT_H

#include "scene/gui/control.h"

class NinePatchRect : public Control {
    GDCLASS(NinePatchRect, Control);

public:
    enum AxisStretchMode {
        AXIS_STRETCH_MODE_STRETCH,
        AXIS_STRETCH_MODE_TILE,
        AXIS_STRETCH_MODE_TILE_FIT,
    };

    bool draw_center;
    int margin[4];
    Rect2 region_rect;
    Ref<Texture> texture;

    AxisStretchMode axis_h, axis_v;

protected:
    void _notification(int p_what);
    virtual Size2 get_minimum_size() const;
    static void _bind_methods();

public:
    void set_texture(const Ref<Texture>& p_tex);
    Ref<Texture> get_texture() const;

    void set_patch_margin(Margin p_margin, int p_size);
    int get_patch_margin(Margin p_margin) const;

    void set_region_rect(const Rect2& p_region_rect);
    Rect2 get_region_rect() const;

    void set_draw_center(bool p_enabled);
    bool is_draw_center_enabled() const;

    void set_h_axis_stretch_mode(AxisStretchMode p_mode);
    AxisStretchMode get_h_axis_stretch_mode() const;

    void set_v_axis_stretch_mode(AxisStretchMode p_mode);
    AxisStretchMode get_v_axis_stretch_mode() const;

    String get_configuration_warning() const;

    NinePatchRect();
    ~NinePatchRect();
};

VARIANT_ENUM_CAST(NinePatchRect::AxisStretchMode)
#endif // NINE_PATCH_RECT_H
