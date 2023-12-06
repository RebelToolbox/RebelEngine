// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "rasterizer.h"

#include "core/os/os.h"
#include "core/print_string.h"

Rasterizer* (*Rasterizer::_create_func)() = nullptr;

Rasterizer* Rasterizer::create() {
    return _create_func();
}

RasterizerStorage* RasterizerStorage::base_singleton = nullptr;

RasterizerStorage::RasterizerStorage() {
    base_singleton = this;
}

bool RasterizerStorage::material_uses_tangents(RID p_material) {
    return false;
}

bool RasterizerStorage::material_uses_ensure_correct_normals(RID p_material) {
    return false;
}
