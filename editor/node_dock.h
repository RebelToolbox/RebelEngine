// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NODE_DOCK_H
#define NODE_DOCK_H

#include "connections_dialog.h"
#include "groups_editor.h"

class NodeDock : public VBoxContainer {
    GDCLASS(NodeDock, VBoxContainer);

    ToolButton* connections_button;
    ToolButton* groups_button;

    ConnectionsDock* connections;
    GroupsEditor* groups;

    HBoxContainer* mode_hb;

    Label* select_a_node;

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    static NodeDock* singleton;

    void set_node(Node* p_node);

    void show_groups();
    void show_connections();

    void update_lists();

    NodeDock();
};

#endif // NODE_DOCK_H
