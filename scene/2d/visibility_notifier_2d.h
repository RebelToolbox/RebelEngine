// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VISIBILITY_NOTIFIER_2D_H
#define VISIBILITY_NOTIFIER_2D_H

#include "scene/2d/node_2d.h"

class Viewport;

class VisibilityNotifier2D : public Node2D {
    GDCLASS(VisibilityNotifier2D, Node2D);

    Set<Viewport*> viewports;

    Rect2 rect;

protected:
    friend struct SpatialIndexer2D;

    void _enter_viewport(Viewport* p_viewport);
    void _exit_viewport(Viewport* p_viewport);

    virtual void _screen_enter() {}

    virtual void _screen_exit() {}

    void _notification(int p_what);
    static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
    virtual Rect2 _edit_get_rect() const;
    virtual bool _edit_use_rect() const;
#endif

    void set_rect(const Rect2& p_rect);
    Rect2 get_rect() const;

    bool is_on_screen() const;

    VisibilityNotifier2D();
};

class VisibilityEnabler2D : public VisibilityNotifier2D {
    GDCLASS(VisibilityEnabler2D, VisibilityNotifier2D);

public:
    enum Enabler {
        ENABLER_PAUSE_ANIMATIONS,
        ENABLER_FREEZE_BODIES,
        ENABLER_PAUSE_PARTICLES,
        ENABLER_PARENT_PROCESS,
        ENABLER_PARENT_PHYSICS_PROCESS,
        ENABLER_PAUSE_ANIMATED_SPRITES,
        ENABLER_MAX
    };

protected:
    virtual void _screen_enter();
    virtual void _screen_exit();

    bool visible;

    void _find_nodes(Node* p_node);

    Map<Node*, Variant> nodes;
    void _node_removed(Node* p_node);
    bool enabler[ENABLER_MAX];

    void _change_node_state(Node* p_node, bool p_enabled);

    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_enabler(Enabler p_enabler, bool p_enable);
    bool is_enabler_enabled(Enabler p_enabler) const;

    String get_configuration_warning() const;

    VisibilityEnabler2D();
};

VARIANT_ENUM_CAST(VisibilityEnabler2D::Enabler);

#endif // VISIBILITY_NOTIFIER_2D_H
