// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "editor_run_script.h"

#include "editor_node.h"

void EditorScript::add_root_node(Node* p_node) {
    if (!editor) {
        EditorNode::add_io_error(
            "EditorScript::add_root_node: "
            + TTR("Write your logic in the _run() method.")
        );
        return;
    }

    if (editor->get_edited_scene()) {
        EditorNode::add_io_error(
            "EditorScript::add_root_node: "
            + TTR("There is an edited scene already.")
        );
        return;
    }

    // editor->set_edited_scene(p_node);
}

EditorInterface* EditorScript::get_editor_interface() {
    return EditorInterface::get_singleton();
}

Node* EditorScript::get_scene() {
    if (!editor) {
        EditorNode::add_io_error(
            "EditorScript::get_scene: "
            + TTR("Write your logic in the _run() method.")
        );
        return nullptr;
    }

    return editor->get_edited_scene();
}

void EditorScript::_run() {
    Ref<Script> s = get_script();
    ERR_FAIL_COND(!s.is_valid());
    if (!get_script_instance()) {
        EditorNode::add_io_error(
            TTR("Couldn't instance script:") + "\n " + s->get_path() + "\n"
            + TTR("Did you forget the 'tool' keyword?")
        );
        return;
    }

    Variant::CallError ce;
    ce.error = Variant::CallError::CALL_OK;
    get_script_instance()->call("_run", nullptr, 0, ce);
    if (ce.error != Variant::CallError::CALL_OK) {
        EditorNode::add_io_error(
            TTR("Couldn't run script:") + "\n " + s->get_path() + "\n"
            + TTR("Did you forget the '_run' method?")
        );
    }
}

void EditorScript::set_editor(EditorNode* p_editor) {
    editor = p_editor;
}

void EditorScript::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("add_root_node", "node"),
        &EditorScript::add_root_node
    );
    ClassDB::bind_method(D_METHOD("get_scene"), &EditorScript::get_scene);
    ClassDB::bind_method(
        D_METHOD("get_editor_interface"),
        &EditorScript::get_editor_interface
    );
    BIND_VMETHOD(MethodInfo("_run"));
}

EditorScript::EditorScript() {
    editor = nullptr;
}
