// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CAMERAWIN_H
#define CAMERAWIN_H

#include "servers/camera/camera_feed.h"
#include "servers/camera_server.h"

class CameraWindows : public CameraServer {
private:
    void add_active_cameras();

public:
    CameraWindows();
    ~CameraWindows();
};

#endif /* CAMERAWIN_H */
