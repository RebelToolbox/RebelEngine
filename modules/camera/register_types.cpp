// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#if defined(WINDOWS_ENABLED)
#include "camera_win.h"
#endif
#if defined(MACOS_ENABLED)
#include "macos_camera.h"
#endif

void register_camera_types() {
#if defined(WINDOWS_ENABLED)
    CameraServer::make_default<CameraWindows>();
#endif
#if defined(MACOS_ENABLED)
    CameraServer::make_default<MacOSCamera>();
#endif
}

void unregister_camera_types() {}
