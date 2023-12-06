// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

class TextFile : public Resource {
    GDCLASS(TextFile, Resource);

private:
    String text;
    String path;

public:
    virtual bool has_text() const;
    virtual String get_text() const;
    virtual void set_text(const String& p_code);
    virtual void reload_from_file();

    void set_file_path(const String& p_path) {
        path = p_path;
    }

    Error load_text(const String& p_path);
};

#endif // TEXTFILE_H
