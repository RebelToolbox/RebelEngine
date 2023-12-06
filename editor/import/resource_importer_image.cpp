// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "resource_importer_image.h"

#include "core/io/image_loader.h"
#include "core/io/resource_saver.h"
#include "core/os/file_access.h"
#include "scene/resources/texture.h"

String ResourceImporterImage::get_importer_name() const {
    return "image";
}

String ResourceImporterImage::get_visible_name() const {
    return "Image";
}

void ResourceImporterImage::get_recognized_extensions(List<String>* p_extensions
) const {
    ImageLoader::get_recognized_extensions(p_extensions);
}

String ResourceImporterImage::get_save_extension() const {
    return "image";
}

String ResourceImporterImage::get_resource_type() const {
    return "Image";
}

bool ResourceImporterImage::get_option_visibility(
    const String& p_option,
    const Map<StringName, Variant>& p_options
) const {
    return true;
}

int ResourceImporterImage::get_preset_count() const {
    return 0;
}

String ResourceImporterImage::get_preset_name(int p_idx) const {
    return String();
}

void ResourceImporterImage::get_import_options(
    List<ImportOption>* r_options,
    int p_preset
) const {}

Error ResourceImporterImage::import(
    const String& p_source_file,
    const String& p_save_path,
    const Map<StringName, Variant>& p_options,
    List<String>* r_platform_variants,
    List<String>* r_gen_files,
    Variant* r_metadata
) {
    FileAccess* f = FileAccess::open(p_source_file, FileAccess::READ);

    ERR_FAIL_COND_V_MSG(
        !f,
        ERR_CANT_OPEN,
        "Cannot open file from path '" + p_source_file + "'."
    );

    uint64_t len = f->get_len();

    Vector<uint8_t> data;
    data.resize(len);

    f->get_buffer(data.ptrw(), len);

    memdelete(f);

    f = FileAccess::open(p_save_path + ".image", FileAccess::WRITE);
    ERR_FAIL_COND_V_MSG(
        !f,
        ERR_CANT_CREATE,
        "Cannot create file in path '" + p_save_path + ".image'."
    );

    // save the header GDIM
    const uint8_t header[4] = {'G', 'D', 'I', 'M'};
    f->store_buffer(header, 4);
    // SAVE the extension (so it can be recognized by the loader later
    f->store_pascal_string(p_source_file.get_extension().to_lower());
    // SAVE the actual image
    f->store_buffer(data.ptr(), len);

    memdelete(f);

    return OK;
}

ResourceImporterImage::ResourceImporterImage() {}
