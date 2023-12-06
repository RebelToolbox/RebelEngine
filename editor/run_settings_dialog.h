// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RUN_SETTINGS_DIALOG_H
#define RUN_SETTINGS_DIALOG_H

#include "scene/gui/check_button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/line_edit.h"

class RunSettingsDialog : public AcceptDialog {
    GDCLASS(RunSettingsDialog, AcceptDialog);

public:
    enum RunMode {
        RUN_LOCAL_SCENE,
        RUN_MAIN_SCENE,
    };

private:
    OptionButton* run_mode;
    LineEdit* arguments;

    void _run_mode_changed(int idx);

protected:
    static void _bind_methods();

public:
    int get_run_mode() const;
    void set_run_mode(int p_run_mode);

    void set_custom_arguments(const String& p_arguments);
    String get_custom_arguments() const;

    void popup_run_settings();

    RunSettingsDialog();
};

#endif // RUN_SETTINGS_DIALOG_H
