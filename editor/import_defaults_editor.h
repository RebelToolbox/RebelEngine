// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IMPORT_DEFAULTS_EDITOR_H
#define IMPORT_DEFAULTS_EDITOR_H

#include "core/undo_redo.h"
#include "editor/editor_data.h"
#include "editor/editor_plugin_settings.h"
#include "editor/editor_sectioned_inspector.h"
#include "editor_autoload_settings.h"
#include "scene/gui/center_container.h"
#include "scene/gui/option_button.h"

class ImportDefaultsEditorSettings;

class ImportDefaultsEditor : public VBoxContainer {
    GDCLASS(ImportDefaultsEditor, VBoxContainer)

    OptionButton* importers;
    Button* save_defaults;
    Button* reset_defaults;

    EditorInspector* inspector;

    ImportDefaultsEditorSettings* settings;

    void _update_importer();
    void _importer_selected(int p_index);

    void _reset();
    void _save();

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void clear();

    ImportDefaultsEditor();
    ~ImportDefaultsEditor();
};

#endif // IMPORT_DEFAULTS_EDITOR_H
