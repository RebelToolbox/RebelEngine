// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_ABOUT_H
#define EDITOR_ABOUT_H

#include "editor_scale.h"
#include "scene/gui/control.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/item_list.h"
#include "scene/gui/link_button.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tree.h"

/**
 * NOTE: Do not assume the EditorNode singleton to be available in this class'
 * methods. EditorAbout is also used from the Projects Manager where EditorNode
 * isn't initialized.
 */
class EditorAbout : public AcceptDialog {
    GDCLASS(EditorAbout, AcceptDialog);

private:
    void _license_tree_selected();
    void _version_button_pressed();
    ScrollContainer* _populate_list(
        const String& p_name,
        const List<String>& p_sections,
        const char* const* const p_src[],
        const int p_flag_single_column = 0
    );

    LinkButton* version_btn;
    Tree* _tpl_tree;
    RichTextLabel* _license_text;
    RichTextLabel* _tpl_text;
    TextureRect* _logo;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    TextureRect* get_logo() const;

    EditorAbout();
    ~EditorAbout();
};

#endif
