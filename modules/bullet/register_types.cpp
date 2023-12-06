// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "bullet_physics_server.h"
#include "core/class_db.h"
#include "core/project_settings.h"

/**
    @author AndreaCatania
*/

#ifndef _3D_DISABLED
PhysicsServer* _createBulletPhysicsCallback() {
    return memnew(BulletPhysicsServer);
}
#endif

void register_bullet_types() {
#ifndef _3D_DISABLED
    PhysicsServerManager::register_server(
        "Bullet",
        &_createBulletPhysicsCallback
    );
    PhysicsServerManager::set_default_server("Bullet", 1);

    GLOBAL_DEF("physics/3d/active_soft_world", true);
    ProjectSettings::get_singleton()->set_custom_property_info(
        "physics/3d/active_soft_world",
        PropertyInfo(Variant::BOOL, "physics/3d/active_soft_world")
    );
#endif
}

void unregister_bullet_types() {}
