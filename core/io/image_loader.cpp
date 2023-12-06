// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "image_loader.h"

#include "core/print_string.h"

bool ImageFormatLoader::recognize(const String& p_extension) const {
    List<String> extensions;
    get_recognized_extensions(&extensions);
    for (List<String>::Element* E = extensions.front(); E; E = E->next()) {
        if (E->get().nocasecmp_to(p_extension) == 0) {
            return true;
        }
    }

    return false;
}

Error ImageLoader::load_image(
    String p_file,
    Ref<Image> p_image,
    FileAccess* p_custom,
    bool p_force_linear,
    float p_scale
) {
    ERR_FAIL_COND_V_MSG(
        p_image.is_null(),
        ERR_INVALID_PARAMETER,
        "It's not a reference to a valid Image object."
    );

    FileAccess* f = p_custom;
    if (!f) {
        Error err;
        f = FileAccess::open(p_file, FileAccess::READ, &err);
        if (!f) {
            ERR_PRINT("Error opening file '" + p_file + "'.");
            return err;
        }
    }

    String extension = p_file.get_extension();

    for (int i = 0; i < loader.size(); i++) {
        if (!loader[i]->recognize(extension)) {
            continue;
        }
        Error err = loader[i]->load_image(p_image, f, p_force_linear, p_scale);
        if (err != OK) {
            ERR_PRINT("Error loading image: " + p_file);
        }

        if (err != ERR_FILE_UNRECOGNIZED) {
            if (!p_custom) {
                memdelete(f);
            }

            return err;
        }
    }

    if (!p_custom) {
        memdelete(f);
    }

    return ERR_FILE_UNRECOGNIZED;
}

void ImageLoader::get_recognized_extensions(List<String>* p_extensions) {
    for (int i = 0; i < loader.size(); i++) {
        loader[i]->get_recognized_extensions(p_extensions);
    }
}

ImageFormatLoader* ImageLoader::recognize(const String& p_extension) {
    for (int i = 0; i < loader.size(); i++) {
        if (loader[i]->recognize(p_extension)) {
            return loader[i];
        }
    }

    return nullptr;
}

Vector<ImageFormatLoader*> ImageLoader::loader;

void ImageLoader::add_image_format_loader(ImageFormatLoader* p_loader) {
    loader.push_back(p_loader);
}

void ImageLoader::remove_image_format_loader(ImageFormatLoader* p_loader) {
    loader.erase(p_loader);
}

const Vector<ImageFormatLoader*>& ImageLoader::get_image_format_loaders() {
    return loader;
}

void ImageLoader::cleanup() {
    while (loader.size()) {
        remove_image_format_loader(loader[0]);
    }
}

/////////////////

RES ResourceFormatLoaderImage::load(
    const String& p_path,
    const String& p_original_path,
    Error* r_error
) {
    FileAccess* f = FileAccess::open(p_path, FileAccess::READ);
    if (!f) {
        if (r_error) {
            *r_error = ERR_CANT_OPEN;
        }
        return RES();
    }

    uint8_t header[4] = {0, 0, 0, 0};
    f->get_buffer(header, 4);

    bool unrecognized = header[0] != 'G' || header[1] != 'D' || header[2] != 'I'
                     || header[3] != 'M';
    if (unrecognized) {
        memdelete(f);
        if (r_error) {
            *r_error = ERR_FILE_UNRECOGNIZED;
        }
        ERR_FAIL_V(RES());
    }

    String extension = f->get_pascal_string();

    int idx = -1;

    for (int i = 0; i < ImageLoader::loader.size(); i++) {
        if (ImageLoader::loader[i]->recognize(extension)) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        memdelete(f);
        if (r_error) {
            *r_error = ERR_FILE_UNRECOGNIZED;
        }
        ERR_FAIL_V(RES());
    }

    Ref<Image> image;
    image.instance();

    Error err = ImageLoader::loader[idx]->load_image(image, f, false, 1.0);

    memdelete(f);

    if (err != OK) {
        if (r_error) {
            *r_error = err;
        }
        return RES();
    }

    if (r_error) {
        *r_error = OK;
    }

    return image;
}

void ResourceFormatLoaderImage::get_recognized_extensions(
    List<String>* p_extensions
) const {
    p_extensions->push_back("image");
}

bool ResourceFormatLoaderImage::handles_type(const String& p_type) const {
    return p_type == "Image";
}

String ResourceFormatLoaderImage::get_resource_type(const String& p_path
) const {
    return p_path.get_extension().to_lower() == "image" ? "Image" : String();
}
