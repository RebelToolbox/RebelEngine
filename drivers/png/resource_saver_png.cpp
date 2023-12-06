// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "resource_saver_png.h"

#include "core/image.h"
#include "core/os/file_access.h"
#include "drivers/png/png_driver_common.h"
#include "scene/resources/texture.h"

Error ResourceSaverPNG::save(
    const String& p_path,
    const RES& p_resource,
    uint32_t p_flags
) {
    Ref<ImageTexture> texture = p_resource;

    ERR_FAIL_COND_V_MSG(
        !texture.is_valid(),
        ERR_INVALID_PARAMETER,
        "Can't save invalid texture as PNG."
    );
    ERR_FAIL_COND_V_MSG(
        !texture->get_width(),
        ERR_INVALID_PARAMETER,
        "Can't save empty texture as PNG."
    );

    Ref<Image> img = texture->get_data();

    Error err = save_image(p_path, img);

    return err;
};

Error ResourceSaverPNG::save_image(
    const String& p_path,
    const Ref<Image>& p_img
) {
    PoolVector<uint8_t> buffer;
    Error err = PNGDriverCommon::image_to_png(p_img, buffer);
    ERR_FAIL_COND_V_MSG(err, err, "Can't convert image to PNG.");
    FileAccess* file = FileAccess::open(p_path, FileAccess::WRITE, &err);
    ERR_FAIL_COND_V_MSG(
        err,
        err,
        vformat("Can't save PNG at path: '%s'.", p_path)
    );

    PoolVector<uint8_t>::Read reader = buffer.read();

    file->store_buffer(reader.ptr(), buffer.size());
    if (file->get_error() != OK && file->get_error() != ERR_FILE_EOF) {
        memdelete(file);
        return ERR_CANT_CREATE;
    }

    file->close();
    memdelete(file);

    return OK;
}

PoolVector<uint8_t> ResourceSaverPNG::save_image_to_buffer(
    const Ref<Image>& p_img
) {
    PoolVector<uint8_t> buffer;
    Error err = PNGDriverCommon::image_to_png(p_img, buffer);
    ERR_FAIL_COND_V_MSG(
        err,
        PoolVector<uint8_t>(),
        "Can't convert image to PNG."
    );
    return buffer;
}

bool ResourceSaverPNG::recognize(const RES& p_resource) const {
    return (p_resource.is_valid() && p_resource->is_class("ImageTexture"));
}

void ResourceSaverPNG::get_recognized_extensions(
    const RES& p_resource,
    List<String>* p_extensions
) const {
    if (Object::cast_to<ImageTexture>(*p_resource)) {
        p_extensions->push_back("png");
    }
}

ResourceSaverPNG::ResourceSaverPNG() {
    Image::save_png_func        = &save_image;
    Image::save_png_buffer_func = &save_image_to_buffer;
};
