// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TEXTURE_LOADER_PKM_H
#define TEXTURE_LOADER_PKM_H

#include "core/io/resource_loader.h"
#include "scene/resources/texture.h"

class ResourceFormatPKM : public ResourceFormatLoader {
public:
    virtual RES load(
        const String& p_path,
        const String& p_original_path = "",
        Error* r_error                = nullptr
    );
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual bool handles_type(const String& p_type) const;
    virtual String get_resource_type(const String& p_path) const;

    virtual ~ResourceFormatPKM() {}
};

#endif // TEXTURE_LOADER_PKM_H
