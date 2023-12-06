// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SHADERTYPES_H
#define SHADERTYPES_H

#include "core/ordered_hash_map.h"
#include "servers/visual_server.h"
#include "shader_language.h"

class ShaderTypes {
    struct Type {
        Map<StringName, ShaderLanguage::FunctionInfo> functions;
        Vector<StringName> modes;
    };

    Map<VS::ShaderMode, Type> shader_modes;

    static ShaderTypes* singleton;

    Set<String> shader_types;

public:
    static ShaderTypes* get_singleton() {
        return singleton;
    }

    const Map<StringName, ShaderLanguage::FunctionInfo>& get_functions(
        VS::ShaderMode p_mode
    );
    const Vector<StringName>& get_modes(VS::ShaderMode p_mode);
    const Set<String>& get_types();

    ShaderTypes();
};

#endif // SHADERTYPES_H
