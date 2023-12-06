// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ASPECT_RATIO_CONTAINER_H
#define ASPECT_RATIO_CONTAINER_H

#include "scene/gui/container.h"

class AspectRatioContainer : public Container {
    GDCLASS(AspectRatioContainer, Container);

protected:
    void _notification(int p_what);
    static void _bind_methods();
    virtual Size2 get_minimum_size() const;

public:
    enum StretchMode {
        STRETCH_WIDTH_CONTROLS_HEIGHT,
        STRETCH_HEIGHT_CONTROLS_WIDTH,
        STRETCH_FIT,
        STRETCH_COVER,
    };

    enum AlignMode {
        ALIGN_BEGIN,
        ALIGN_CENTER,
        ALIGN_END,
    };

private:
    float ratio                    = 1.0;
    StretchMode stretch_mode       = STRETCH_FIT;
    AlignMode alignment_horizontal = ALIGN_CENTER;
    AlignMode alignment_vertical   = ALIGN_CENTER;

public:
    void set_ratio(float p_ratio);

    float get_ratio() const {
        return ratio;
    }

    void set_stretch_mode(StretchMode p_mode);

    StretchMode get_stretch_mode() const {
        return stretch_mode;
    }

    void set_alignment_horizontal(AlignMode p_alignment_horizontal);

    AlignMode get_alignment_horizontal() const {
        return alignment_horizontal;
    }

    void set_alignment_vertical(AlignMode p_alignment_vertical);

    AlignMode get_alignment_vertical() const {
        return alignment_vertical;
    }
};

VARIANT_ENUM_CAST(AspectRatioContainer::StretchMode);
VARIANT_ENUM_CAST(AspectRatioContainer::AlignMode);

#endif // ASPECT_RATIO_CONTAINER_H
