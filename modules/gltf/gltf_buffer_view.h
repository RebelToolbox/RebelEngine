// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_BUFFER_VIEW_H
#define GLTF_BUFFER_VIEW_H

#include "core/resource.h"
#include "gltf_document.h"

class GLTFBufferView : public Resource {
    GDCLASS(GLTFBufferView, Resource);
    friend class GLTFDocument;

private:
    GLTFBufferIndex buffer = -1;
    int byte_offset        = 0;
    int byte_length        = 0;
    int byte_stride        = -1;
    bool indices           = false;

protected:
    static void _bind_methods();

public:
    GLTFBufferIndex get_buffer();
    void set_buffer(GLTFBufferIndex p_buffer);

    int get_byte_offset();
    void set_byte_offset(int p_byte_offset);

    int get_byte_length();
    void set_byte_length(int p_byte_length);

    int get_byte_stride();
    void set_byte_stride(int p_byte_stride);

    bool get_indices();
    void set_indices(bool p_indices);
    // matrices need to be transformed to this
};
#endif // GLTF_BUFFER_VIEW_H
