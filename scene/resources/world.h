// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WORLD_H
#define WORLD_H

#include "core/resource.h"
#include "scene/resources/environment.h"
#include "servers/physics_server.h"
#include "servers/visual_server.h"

class Camera;
class VisibilityNotifier;
struct SpatialIndexer;

class World : public Resource {
    GDCLASS(World, Resource);
    RES_BASE_EXTENSION("world");

private:
    RID space;
    RID scenario;
    SpatialIndexer* indexer;
    Ref<Environment> environment;
    Ref<Environment> fallback_environment;

protected:
    static void _bind_methods();

    friend class Camera;
    friend class VisibilityNotifier;

    void _register_camera(Camera* p_camera);
    void _update_camera(Camera* p_camera);
    void _remove_camera(Camera* p_camera);

    void _register_notifier(VisibilityNotifier* p_notifier, const AABB& p_rect);
    void _update_notifier(VisibilityNotifier* p_notifier, const AABB& p_rect);
    void _remove_notifier(VisibilityNotifier* p_notifier);
    friend class Viewport;
    void _update(uint64_t p_frame);

public:
    RID get_space() const;
    RID get_scenario() const;

    void set_environment(const Ref<Environment>& p_environment);
    Ref<Environment> get_environment() const;

    void set_fallback_environment(const Ref<Environment>& p_environment);
    Ref<Environment> get_fallback_environment() const;

    void get_camera_list(List<Camera*>* r_cameras);

    PhysicsDirectSpaceState* get_direct_space_state();

    World();
    ~World();
};

#endif // WORLD_H
