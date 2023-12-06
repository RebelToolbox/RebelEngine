// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIO_STREAM_EDITOR_PLUGIN_H
#define AUDIO_STREAM_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/audio/audio_stream_player.h"
#include "scene/gui/color_rect.h"
#include "scene/resources/texture.h"

class AudioStreamEditor : public ColorRect {
    GDCLASS(AudioStreamEditor, ColorRect);

    Ref<AudioStream> stream;
    AudioStreamPlayer* _player = nullptr;
    ColorRect* _preview        = nullptr;
    Control* _indicator        = nullptr;
    Label* _current_label      = nullptr;
    Label* _duration_label     = nullptr;

    ToolButton* _play_button = nullptr;
    ToolButton* _stop_button = nullptr;

    float _current = 0;
    bool _dragging = false;
    bool _pausing  = false;

protected:
    void _notification(int p_what);
    void _preview_changed(ObjectID p_which);
    void _play();
    void _stop();
    void _on_finished();
    void _draw_preview();
    void _draw_indicator();
    void _on_input_indicator(Ref<InputEvent> p_event);
    void _seek_to(real_t p_x);
    void _changed_callback(Object* p_changed, const char* p_prop);
    static void _bind_methods();

public:
    void edit(Ref<AudioStream> p_stream);
    AudioStreamEditor();
};

class AudioStreamEditorPlugin : public EditorPlugin {
    GDCLASS(AudioStreamEditorPlugin, EditorPlugin);

    AudioStreamEditor* audio_editor;
    EditorNode* editor;

public:
    virtual String get_name() const {
        return "Audio";
    }

    bool has_main_screen() const {
        return false;
    }

    virtual void edit(Object* p_object);
    virtual bool handles(Object* p_object) const;
    virtual void make_visible(bool p_visible);

    AudioStreamEditorPlugin(EditorNode* p_node);
    ~AudioStreamEditorPlugin();
};

#endif // AUDIO_STREAM_EDITOR_PLUGIN_H
