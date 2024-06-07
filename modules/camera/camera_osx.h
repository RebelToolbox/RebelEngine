// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CAMERAOSX_H
#define CAMERAOSX_H

///@TODO this is a near duplicate of IosCamera, we should find a way to combine
/// those to minimize code duplication!!!!
// If you fix something here, make sure you fix it there as well!

#include "servers/camera_server.h"

class CameraOSX : public CameraServer {
public:
    CameraOSX();
    ~CameraOSX();

    void update_feeds();
};

#endif /* CAMERAOSX_H */
