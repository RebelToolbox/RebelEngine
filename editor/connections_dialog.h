// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

/**
@author Juan Linietsky <reduzio@gmail.com>
*/

#ifndef CONNECTIONS_DIALOG_H
#define CONNECTIONS_DIALOG_H

#include "core/undo_redo.h"
#include "editor/editor_inspector.h"
#include "editor/scene_tree_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/check_button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/popup.h"
#include "scene/gui/tree.h"

class PopupMenu;
class ConnectDialogBinds;

class ConnectDialog : public ConfirmationDialog {
    GDCLASS(ConnectDialog, ConfirmationDialog);

    Label* connect_to_label;
    LineEdit* from_signal;
    Node* source;
    StringName signal;
    LineEdit* dst_method;
    ConnectDialogBinds* cdbinds;
    bool bEditMode;
    NodePath dst_path;
    VBoxContainer* vbc_right;

    SceneTreeEditor* tree;
    AcceptDialog* error;
    EditorInspector* bind_editor;
    OptionButton* type_list;
    CheckBox* deferred;
    CheckBox* oneshot;
    CheckButton* advanced;

    Label* error_label;

    void ok_pressed();
    void _cancel_pressed();
    void _tree_node_selected();
    void _add_bind();
    void _remove_bind();
    void _advanced_pressed();
    void _update_ok_enabled();

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    Node* get_source() const;
    StringName get_signal_name() const;
    NodePath get_dst_path() const;
    void set_dst_node(Node* p_node);
    StringName get_dst_method_name() const;
    void set_dst_method(const StringName& p_method);
    Vector<Variant> get_binds() const;

    bool get_deferred() const;
    bool get_oneshot() const;
    bool is_editing() const;

    void init(Connection c, bool bEdit = false);

    void popup_dialog(const String& p_for_signal);
    ConnectDialog();
    ~ConnectDialog();
};

//////////////////////////////////////////

// Custom Tree needed to use a RichTextLabel as tooltip control
// when display signal documentation.
class ConnectionsDockTree : public Tree {
    virtual Control* make_custom_tooltip(const String& p_text) const;
};

class ConnectionsDock : public VBoxContainer {
    GDCLASS(ConnectionsDock, VBoxContainer);

    // Right-click Pop-up Menu Options.
    enum SignalMenuOption {
        CONNECT,
        DISCONNECT_ALL
    };

    enum SlotMenuOption {
        EDIT,
        GO_TO_SCRIPT,
        DISCONNECT
    };

    Node* selectedNode;
    ConnectionsDockTree* tree;
    EditorNode* editor;

    ConfirmationDialog* disconnect_all_dialog;
    ConnectDialog* connect_dialog;
    Button* connect_button;
    PopupMenu* signal_menu;
    PopupMenu* slot_menu;
    UndoRedo* undo_redo;
    LineEdit* search_box;

    Map<StringName, Map<StringName, String>> descr_cache;

    void _filter_changed(const String& p_text);

    void _make_or_edit_connection();
    void _connect(Connection cToMake);
    void _disconnect(TreeItem& item);
    void _disconnect_all();

    void _tree_item_selected();
    void _tree_item_activated();
    bool _is_item_signal(TreeItem& item);

    void _open_connection_dialog(TreeItem& item);
    void _open_connection_dialog(Connection cToEdit);
    void _go_to_script(TreeItem& item);

    void _handle_signal_menu_option(int option);
    void _handle_slot_menu_option(int option);
    void _rmb_pressed(Vector2 position);
    void _close();

protected:
    void _connect_pressed();
    void _notification(int p_what);
    static void _bind_methods();

public:
    void set_undoredo(UndoRedo* p_undo_redo) {
        undo_redo = p_undo_redo;
    }

    void set_node(Node* p_node);
    void update_tree();

    ConnectionsDock(EditorNode* p_editor = nullptr);
    ~ConnectionsDock();
};

#endif
