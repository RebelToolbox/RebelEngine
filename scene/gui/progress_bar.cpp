// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "progress_bar.h"

Size2 ProgressBar::get_minimum_size() const {
    Ref<StyleBox> bg = get_stylebox("bg");
    Ref<StyleBox> fg = get_stylebox("fg");
    Ref<Font> font   = get_font("font");

    Size2 minimum_size = bg->get_minimum_size();
    minimum_size.height =
        MAX(minimum_size.height, fg->get_minimum_size().height);
    minimum_size.width = MAX(minimum_size.width, fg->get_minimum_size().width);
    if (percent_visible) {
        minimum_size.height =
            MAX(minimum_size.height,
                bg->get_minimum_size().height + font->get_height());
    } else { // this is needed, else the progressbar will collapse
        minimum_size.width  = MAX(minimum_size.width, 1);
        minimum_size.height = MAX(minimum_size.height, 1);
    }
    return minimum_size;
}

void ProgressBar::_notification(int p_what) {
    if (p_what == NOTIFICATION_DRAW) {
        Ref<StyleBox> bg = get_stylebox("bg");
        Ref<StyleBox> fg = get_stylebox("fg");
        Ref<Font> font   = get_font("font");
        Color font_color = get_color("font_color");

        draw_style_box(bg, Rect2(Point2(), get_size()));
        float r = get_as_ratio();
        int mp  = fg->get_minimum_size().width;
        int p   = r * (get_size().width - mp);
        if (p > 0) {
            draw_style_box(
                fg,
                Rect2(
                    Point2(),
                    Size2(p + fg->get_minimum_size().width, get_size().height)
                )
            );
        }

        if (percent_visible) {
            String txt = itos(int(get_as_ratio() * 100)) + "%";
            font->draw_halign(
                get_canvas_item(),
                Point2(
                    0,
                    font->get_ascent()
                        + (get_size().height - font->get_height()) / 2
                ),
                HALIGN_CENTER,
                get_size().width,
                txt,
                font_color
            );
        }
    }
}

void ProgressBar::set_percent_visible(bool p_visible) {
    percent_visible = p_visible;
    update();
}

bool ProgressBar::is_percent_visible() const {
    return percent_visible;
}

void ProgressBar::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("set_percent_visible", "visible"),
        &ProgressBar::set_percent_visible
    );
    ClassDB::bind_method(
        D_METHOD("is_percent_visible"),
        &ProgressBar::is_percent_visible
    );
    ADD_GROUP("Percent", "percent_");
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "percent_visible"),
        "set_percent_visible",
        "is_percent_visible"
    );
}

ProgressBar::ProgressBar() {
    set_v_size_flags(0);
    set_step(0.01);
    percent_visible = true;
}
