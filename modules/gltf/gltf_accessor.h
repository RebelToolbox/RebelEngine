// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GLTF_ACCESSOR_H
#define GLTF_ACCESSOR_H

#include "core/resource.h"
#include "gltf_document.h"

struct GLTFAccessor : public Resource {
    GDCLASS(GLTFAccessor, Resource);
    friend class GLTFDocument;

private:
    GLTFBufferViewIndex buffer_view = 0;
    int byte_offset                 = 0;
    int component_type              = 0;
    bool normalized                 = false;
    int count                       = 0;
    GLTFDocument::GLTFType type     = GLTFDocument::TYPE_SCALAR;
    PoolVector<float> min;
    PoolVector<float> max;
    int sparse_count                  = 0;
    int sparse_indices_buffer_view    = 0;
    int sparse_indices_byte_offset    = 0;
    int sparse_indices_component_type = 0;
    int sparse_values_buffer_view     = 0;
    int sparse_values_byte_offset     = 0;

protected:
    static void _bind_methods();

public:
    GLTFBufferViewIndex get_buffer_view();
    void set_buffer_view(GLTFBufferViewIndex p_buffer_view);

    int get_byte_offset();
    void set_byte_offset(int p_byte_offset);

    int get_component_type();
    void set_component_type(int p_component_type);

    bool get_normalized();
    void set_normalized(bool p_normalized);

    int get_count();
    void set_count(int p_count);

    int get_type();
    void set_type(int p_type);

    PoolVector<float> get_min();
    void set_min(PoolVector<float> p_min);

    PoolVector<float> get_max();
    void set_max(PoolVector<float> p_max);

    int get_sparse_count();
    void set_sparse_count(int p_sparse_count);

    int get_sparse_indices_buffer_view();
    void set_sparse_indices_buffer_view(int p_sparse_indices_buffer_view);

    int get_sparse_indices_byte_offset();
    void set_sparse_indices_byte_offset(int p_sparse_indices_byte_offset);

    int get_sparse_indices_component_type();
    void set_sparse_indices_component_type(int p_sparse_indices_component_type);

    int get_sparse_values_buffer_view();
    void set_sparse_values_buffer_view(int p_sparse_values_buffer_view);

    int get_sparse_values_byte_offset();
    void set_sparse_values_byte_offset(int p_sparse_values_byte_offset);
};
#endif // GLTF_ACCESSOR_H
