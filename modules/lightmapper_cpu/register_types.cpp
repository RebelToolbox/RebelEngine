// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/project_settings.h"
#include "lightmapper_cpu.h"
#include "scene/3d/lightmapper.h"

#ifndef _3D_DISABLED
static Lightmapper* create_lightmapper_cpu() {
    return memnew(LightmapperCPU);
}
#endif

void register_lightmapper_cpu_types() {
    GLOBAL_DEF("rendering/cpu_lightmapper/quality/low_quality_ray_count", 64);
    GLOBAL_DEF(
        "rendering/cpu_lightmapper/quality/medium_quality_ray_count",
        256
    );
    GLOBAL_DEF("rendering/cpu_lightmapper/quality/high_quality_ray_count", 512);
    GLOBAL_DEF(
        "rendering/cpu_lightmapper/quality/ultra_quality_ray_count",
        1024
    );
#ifndef _3D_DISABLED
    Lightmapper::create_cpu = create_lightmapper_cpu;
#endif
}

void unregister_lightmapper_cpu_types() {}
