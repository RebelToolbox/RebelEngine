// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BONE_ATTACHMENT_H
#define BONE_ATTACHMENT_H

#include "scene/3d/skeleton.h"

class BoneAttachment : public Spatial {
    GDCLASS(BoneAttachment, Spatial);

    bool bound;
    String bone_name;

    void _check_bind();
    void _check_unbind();

protected:
    virtual void _validate_property(PropertyInfo& property) const;
    void _notification(int p_what);

    static void _bind_methods();

public:
    void set_bone_name(const String& p_name);
    String get_bone_name() const;

    BoneAttachment();
};

#endif // BONE_ATTACHMENT_H
