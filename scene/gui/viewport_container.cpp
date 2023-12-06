// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "viewport_container.h"

#include "core/engine.h"
#include "scene/main/viewport.h"

Size2 ViewportContainer::get_minimum_size() const {
    if (stretch) {
        return Size2();
    }
    Size2 ms;
    for (int i = 0; i < get_child_count(); i++) {
        Viewport* c = Object::cast_to<Viewport>(get_child(i));
        if (!c) {
            continue;
        }

        Size2 minsize = c->get_size();
        ms.width      = MAX(ms.width, minsize.width);
        ms.height     = MAX(ms.height, minsize.height);
    }

    return ms;
}

void ViewportContainer::set_stretch(bool p_enable) {
    stretch = p_enable;
    queue_sort();
    update();
}

bool ViewportContainer::is_stretch_enabled() const {
    return stretch;
}

void ViewportContainer::set_stretch_shrink(int p_shrink) {
    ERR_FAIL_COND(p_shrink < 1);
    if (shrink == p_shrink) {
        return;
    }

    shrink = p_shrink;

    if (!stretch) {
        return;
    }

    for (int i = 0; i < get_child_count(); i++) {
        Viewport* c = Object::cast_to<Viewport>(get_child(i));
        if (!c) {
            continue;
        }

        c->set_size(get_size() / shrink);
    }

    update();
}

int ViewportContainer::get_stretch_shrink() const {
    return shrink;
}

void ViewportContainer::_notification(int p_what) {
    if (p_what == NOTIFICATION_RESIZED) {
        if (!stretch) {
            return;
        }

        for (int i = 0; i < get_child_count(); i++) {
            Viewport* c = Object::cast_to<Viewport>(get_child(i));
            if (!c) {
                continue;
            }

            c->set_size(get_size() / shrink);
        }
    }

    if (p_what == NOTIFICATION_ENTER_TREE
        || p_what == NOTIFICATION_VISIBILITY_CHANGED) {
        for (int i = 0; i < get_child_count(); i++) {
            Viewport* c = Object::cast_to<Viewport>(get_child(i));
            if (!c) {
                continue;
            }

            if (is_visible_in_tree()) {
                c->set_update_mode(Viewport::UPDATE_ALWAYS);
            } else {
                c->set_update_mode(Viewport::UPDATE_DISABLED);
            }

            c->set_handle_input_locally(false
            ); // do not handle input locally here
        }
    }

    if (p_what == NOTIFICATION_DRAW) {
        for (int i = 0; i < get_child_count(); i++) {
            Viewport* c = Object::cast_to<Viewport>(get_child(i));
            if (!c) {
                continue;
            }

            if (stretch) {
                draw_texture_rect(
                    c->get_texture(),
                    Rect2(Vector2(), get_size() * Size2(1, -1))
                );
            } else {
                draw_texture_rect(
                    c->get_texture(),
                    Rect2(Vector2(), c->get_size() * Size2(1, -1))
                );
            }
        }
    }
}

void ViewportContainer::_input(const Ref<InputEvent>& p_event) {
    ERR_FAIL_COND(p_event.is_null());

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    Transform2D xform = get_global_transform();

    if (stretch) {
        Transform2D scale_xf;
        scale_xf.scale(Vector2(shrink, shrink));
        xform *= scale_xf;
    }

    Ref<InputEvent> ev = p_event->xformed_by(xform.affine_inverse());

    for (int i = 0; i < get_child_count(); i++) {
        Viewport* c = Object::cast_to<Viewport>(get_child(i));
        if (!c || c->is_input_disabled()) {
            continue;
        }

        c->input(ev);
    }
}

void ViewportContainer::_unhandled_input(const Ref<InputEvent>& p_event) {
    ERR_FAIL_COND(p_event.is_null());

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    Transform2D xform = get_global_transform();

    if (stretch) {
        Transform2D scale_xf;
        scale_xf.scale(Vector2(shrink, shrink));
        xform *= scale_xf;
    }

    Ref<InputEvent> ev = p_event->xformed_by(xform.affine_inverse());

    for (int i = 0; i < get_child_count(); i++) {
        Viewport* c = Object::cast_to<Viewport>(get_child(i));
        if (!c || c->is_input_disabled()) {
            continue;
        }

        c->unhandled_input(ev);
    }
}

void ViewportContainer::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("_unhandled_input", "event"),
        &ViewportContainer::_unhandled_input
    );
    ClassDB::bind_method(
        D_METHOD("_input", "event"),
        &ViewportContainer::_input
    );
    ClassDB::bind_method(
        D_METHOD("set_stretch", "enable"),
        &ViewportContainer::set_stretch
    );
    ClassDB::bind_method(
        D_METHOD("is_stretch_enabled"),
        &ViewportContainer::is_stretch_enabled
    );

    ClassDB::bind_method(
        D_METHOD("set_stretch_shrink", "amount"),
        &ViewportContainer::set_stretch_shrink
    );
    ClassDB::bind_method(
        D_METHOD("get_stretch_shrink"),
        &ViewportContainer::get_stretch_shrink
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "stretch"),
        "set_stretch",
        "is_stretch_enabled"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "stretch_shrink"),
        "set_stretch_shrink",
        "get_stretch_shrink"
    );
}

ViewportContainer::ViewportContainer() {
    stretch = false;
    shrink  = 1;
    set_process_input(true);
    set_process_unhandled_input(true);
}
