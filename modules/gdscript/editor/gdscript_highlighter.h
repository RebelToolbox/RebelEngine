// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDSCRIPT_HIGHLIGHTER_H
#define GDSCRIPT_HIGHLIGHTER_H

#include "scene/gui/text_edit.h"

class GDScriptSyntaxHighlighter : public SyntaxHighlighter {
private:
    enum Type {
        NONE,
        REGION,
        NODE_PATH,
        SYMBOL,
        NUMBER,
        FUNCTION,
        KEYWORD,
        MEMBER,
        IDENTIFIER,
        TYPE,
    };

    // colours
    Color font_color;
    Color symbol_color;
    Color function_color;
    Color function_definition_color;
    Color built_in_type_color;
    Color number_color;
    Color member_color;
    Color node_path_color;
    Color type_color;

public:
    static SyntaxHighlighter* create();

    virtual void _update_cache();
    virtual Map<int, TextEdit::HighlighterInfo> _get_line_syntax_highlighting(
        int p_line
    );

    virtual String get_name() const;
    virtual List<String> get_supported_languages();
};

#endif // GDSCRIPT_HIGHLIGHTER_H
