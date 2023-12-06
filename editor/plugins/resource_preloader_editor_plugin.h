// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RESOURCE_PRELOADER_EDITOR_PLUGIN_H
#define RESOURCE_PRELOADER_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/tree.h"
#include "scene/main/resource_preloader.h"

class ResourcePreloaderEditor : public PanelContainer {
    GDCLASS(ResourcePreloaderEditor, PanelContainer);

    enum {
        BUTTON_OPEN_SCENE,
        BUTTON_EDIT_RESOURCE,
        BUTTON_REMOVE
    };

    Button* load;
    Button* paste;
    Tree* tree;
    bool loading_scene;

    EditorFileDialog* file;

    AcceptDialog* dialog;

    ResourcePreloader* preloader;

    void _load_pressed();
    void _load_scene_pressed();
    void _files_load_request(const Vector<String>& p_paths);
    void _paste_pressed();
    void _remove_resource(const String& p_to_remove);
    void _update_library();
    void _cell_button_pressed(Object* p_item, int p_column, int p_id);
    void _item_edited();

    UndoRedo* undo_redo;

    Variant get_drag_data_fw(const Point2& p_point, Control* p_from);
    bool can_drop_data_fw(
        const Point2& p_point,
        const Variant& p_data,
        Control* p_from
    ) const;
    void drop_data_fw(
        const Point2& p_point,
        const Variant& p_data,
        Control* p_from
    );

protected:
    void _notification(int p_what);
    void _gui_input(Ref<InputEvent> p_event);
    static void _bind_methods();

public:
    void set_undo_redo(UndoRedo* p_undo_redo) {
        undo_redo = p_undo_redo;
    }

    void edit(ResourcePreloader* p_preloader);
    ResourcePreloaderEditor();
};

class ResourcePreloaderEditorPlugin : public EditorPlugin {
    GDCLASS(ResourcePreloaderEditorPlugin, EditorPlugin);

    ResourcePreloaderEditor* preloader_editor;
    EditorNode* editor;
    Button* button;

public:
    virtual String get_name() const {
        return "ResourcePreloader";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    ResourcePreloaderEditorPlugin(EditorNode* p_node);
    ~ResourcePreloaderEditorPlugin();
};

#endif // RESOURCE_PRELOADER_EDITOR_PLUGIN_H
