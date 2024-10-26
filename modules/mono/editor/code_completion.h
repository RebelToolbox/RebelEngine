// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CODE_COMPLETION_H
#define CODE_COMPLETION_H

#include "core/ustring.h"
#include "core/variant.h"

namespace gdmono {

enum class CompletionKind {
    INPUT_ACTIONS = 0,
    NODE_PATHS,
    RESOURCE_PATHS,
    SCENE_PATHS,
    SHADER_PARAMS,
    SIGNALS,
    THEME_COLORS,
    THEME_CONSTANTS,
    THEME_FONTS,
    THEME_STYLES
};

PoolStringArray get_code_completion(
    CompletionKind p_kind,
    const String& p_script_file
);

} // namespace gdmono

#endif // CODE_COMPLETION_H
