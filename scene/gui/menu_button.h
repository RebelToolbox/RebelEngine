// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include "scene/gui/button.h"
#include "scene/gui/popup_menu.h"

class MenuButton : public Button {
    GDCLASS(MenuButton, Button);

    bool clicked;
    bool switch_on_hover;
    bool disable_shortcuts;
    PopupMenu* popup;

    void _unhandled_key_input(Ref<InputEvent> p_event);
    Array _get_items() const;
    void _set_items(const Array& p_items);

    void _gui_input(Ref<InputEvent> p_event);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    virtual void pressed();

    PopupMenu* get_popup() const;
    void set_switch_on_hover(bool p_enabled);
    bool is_switch_on_hover();
    void set_disable_shortcuts(bool p_disabled);

    MenuButton();
    ~MenuButton();
};

#endif
