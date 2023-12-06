// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TRANSLATION_LOADER_PO_H
#define TRANSLATION_LOADER_PO_H

#include "core/io/resource_loader.h"
#include "core/os/file_access.h"
#include "core/translation.h"

class TranslationLoaderPO : public ResourceFormatLoader {
public:
    static RES load_translation(FileAccess* f, Error* r_error = nullptr);
    virtual RES load(
        const String& p_path,
        const String& p_original_path = "",
        Error* r_error                = nullptr
    );
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual bool handles_type(const String& p_type) const;
    virtual String get_resource_type(const String& p_path) const;

    TranslationLoaderPO();
};

#endif // TRANSLATION_LOADER_PO_H
