// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BACKBUFFERCOPY_H
#define BACKBUFFERCOPY_H

#include "scene/2d/node_2d.h"

class BackBufferCopy : public Node2D {
    GDCLASS(BackBufferCopy, Node2D);

public:
    enum CopyMode {
        COPY_MODE_DISABLED,
        COPY_MODE_RECT,
        COPY_MODE_VIEWPORT
    };

private:
    Rect2 rect;
    CopyMode copy_mode;

    void _update_copy_mode();

protected:
    static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
    Rect2 _edit_get_rect() const;
    virtual bool _edit_use_rect() const;
#endif

    void set_rect(const Rect2& p_rect);
    Rect2 get_rect() const;
    Rect2 get_anchorable_rect() const;

    void set_copy_mode(CopyMode p_mode);
    CopyMode get_copy_mode() const;

    BackBufferCopy();
    ~BackBufferCopy();
};

VARIANT_ENUM_CAST(BackBufferCopy::CopyMode);

#endif // BACKBUFFERCOPY_H
