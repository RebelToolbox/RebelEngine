// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "visual_server_globals.h"

RasterizerStorage* VisualServerGlobals::storage      = nullptr;
RasterizerCanvas* VisualServerGlobals::canvas_render = nullptr;
RasterizerScene* VisualServerGlobals::scene_render   = nullptr;
Rasterizer* VisualServerGlobals::rasterizer          = nullptr;

VisualServerCanvas* VisualServerGlobals::canvas     = nullptr;
VisualServerViewport* VisualServerGlobals::viewport = nullptr;
VisualServerScene* VisualServerGlobals::scene       = nullptr;
