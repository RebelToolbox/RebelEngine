// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VISIBILITY_NOTIFIER_H
#define VISIBILITY_NOTIFIER_H

#include "scene/3d/cull_instance.h"

class World;
class Camera;

class VisibilityNotifier : public CullInstance {
    GDCLASS(VisibilityNotifier, CullInstance);

    Ref<World> world;
    Set<Camera*> cameras;

    AABB aabb;

    // if using rooms and portals
    RID _cull_instance_rid;
    bool _in_gameplay;

protected:
    virtual void _screen_enter() {}

    virtual void _screen_exit() {}

    virtual void _refresh_portal_mode();

    void _notification(int p_what);
    static void _bind_methods();
    friend struct SpatialIndexer;

    void _enter_camera(Camera* p_camera);
    void _exit_camera(Camera* p_camera);

public:
    void set_aabb(const AABB& p_aabb);
    AABB get_aabb() const;
    bool is_on_screen() const;

    VisibilityNotifier();
    ~VisibilityNotifier();
};

class VisibilityEnabler : public VisibilityNotifier {
    GDCLASS(VisibilityEnabler, VisibilityNotifier);

public:
    enum Enabler {
        ENABLER_PAUSE_ANIMATIONS,
        ENABLER_FREEZE_BODIES,
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

    VisibilityEnabler();
};

VARIANT_ENUM_CAST(VisibilityEnabler::Enabler);

#endif // VISIBILITY_NOTIFIER_H
