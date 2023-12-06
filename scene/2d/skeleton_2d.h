// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SKELETON_2D_H
#define SKELETON_2D_H

#include "scene/2d/node_2d.h"

class Skeleton2D;

class Bone2D : public Node2D {
    GDCLASS(Bone2D, Node2D);

    friend class Skeleton2D;
#ifdef TOOLS_ENABLED
    friend class AnimatedValuesBackup;
#endif

    Bone2D* parent_bone;
    Skeleton2D* skeleton;
    Transform2D rest;
    float default_length;

    int skeleton_index;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_rest(const Transform2D& p_rest);
    Transform2D get_rest() const;
    void apply_rest();
    Transform2D get_skeleton_rest() const;

    String get_configuration_warning() const;

    void set_default_length(float p_length);
    float get_default_length() const;

    int get_index_in_skeleton() const;

    Bone2D();
};

class Skeleton2D : public Node2D {
    GDCLASS(Skeleton2D, Node2D);

    friend class Bone2D;
#ifdef TOOLS_ENABLED
    friend class AnimatedValuesBackup;
#endif

    struct Bone {
        bool operator<(const Bone& p_bone) const {
            return p_bone.bone->is_greater_than(bone);
        }

        Bone2D* bone;
        int parent_index;
        Transform2D accum_transform;
        Transform2D rest_inverse;
    };

    Vector<Bone> bones;

    bool bone_setup_dirty;
    void _make_bone_setup_dirty();
    void _update_bone_setup();

    bool transform_dirty;
    void _make_transform_dirty();
    void _update_transform();

    RID skeleton;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    int get_bone_count() const;
    Bone2D* get_bone(int p_idx);

    RID get_skeleton() const;
    Skeleton2D();
    ~Skeleton2D();
};

#endif // SKELETON_2D_H
