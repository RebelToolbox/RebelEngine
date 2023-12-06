// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef FBX_ANIM_CONTAINER_H
#define FBX_ANIM_CONTAINER_H

#include "core/vector.h"

// Generic keyframes 99.99 percent of files will be vector3, except if quat
// interp is used, or visibility tracks FBXTrack is used in a map in the
// implementation in fbx/editor_scene_importer_fbx.cpp to avoid having to
// rewrite the entire logic I refactored this into the code instead. once it
// works I can rewrite so we can add the fun misc features / small features
struct FBXTrack {
    bool has_default = false;
    Vector3 default_value;
    std::map<int64_t, Vector3> keyframes;
};

#endif // MODEL_ABSTRACTION_ANIM_CONTAINER_H
