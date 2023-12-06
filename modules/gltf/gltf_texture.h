// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_TEXTURE_H
#define GLTF_TEXTURE_H

#include "core/resource.h"
#include "gltf_document.h"

class GLTFTexture : public Resource {
    GDCLASS(GLTFTexture, Resource);

private:
    GLTFImageIndex src_image = 0;

protected:
    static void _bind_methods();

public:
    GLTFImageIndex get_src_image() const;
    void set_src_image(GLTFImageIndex val);
};

#endif // GLTF_TEXTURE_H
