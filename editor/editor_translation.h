// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_TRANSLATION_H
#define EDITOR_TRANSLATION_H

#include "core/ustring.h"
#include "core/vector.h"

Vector<String> get_editor_locales();
void load_editor_translations(const String& p_locale);
void load_doc_translations(const String& p_locale);

#endif // EDITOR_TRANSLATION_H
