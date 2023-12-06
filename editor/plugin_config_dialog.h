// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PLUGIN_CONFIG_DIALOG_H
#define PLUGIN_CONFIG_DIALOG_H

#include "scene/gui/check_box.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/text_edit.h"

class PluginConfigDialog : public ConfirmationDialog {
    GDCLASS(PluginConfigDialog, ConfirmationDialog);

    LineEdit* name_edit;
    LineEdit* subfolder_edit;
    TextEdit* desc_edit;
    LineEdit* author_edit;
    LineEdit* version_edit;
    OptionButton* script_option_edit;
    LineEdit* script_edit;
    CheckBox* active_edit;

    bool _edit_mode;

    void _clear_fields();
    void _on_confirmed();
    void _on_cancelled();
    void _on_required_text_changed(const String& p_text);

protected:
    virtual void _notification(int p_what);
    static void _bind_methods();

public:
    void config(const String& p_config_path);

    PluginConfigDialog();
    ~PluginConfigDialog();
};

#endif // PLUGIN_CONFIG_DIALOG_H
