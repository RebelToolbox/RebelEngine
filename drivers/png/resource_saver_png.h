// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_SAVER_PNG_H
#define RESOURCE_SAVER_PNG_H

#include "core/image.h"
#include "core/io/resource_saver.h"

class ResourceSaverPNG : public ResourceFormatSaver {
public:
    static Error save_image(const String& p_path, const Ref<Image>& p_img);
    static PoolVector<uint8_t> save_image_to_buffer(const Ref<Image>& p_img);

    virtual Error save(
        const String& p_path,
        const RES& p_resource,
        uint32_t p_flags = 0
    );
    virtual bool recognize(const RES& p_resource) const;
    virtual void get_recognized_extensions(
        const RES& p_resource,
        List<String>* p_extensions
    ) const;

    ResourceSaverPNG();
};

#endif // RESOURCE_SAVER_PNG_H
