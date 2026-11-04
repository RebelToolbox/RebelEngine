// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITOR_PREVIEW_PLUGINS_H
#define EDITOR_PREVIEW_PLUGINS_H

#include "core/safe_refcount.h"
#include "editor/editor_resource_preview.h"

void post_process_preview(Ref<Image> p_image);

class EditorTexturePreviewPlugin : public EditorResourcePreviewGenerator {
    GDCLASS(EditorTexturePreviewPlugin, EditorResourcePreviewGenerator);

public:
    bool handles(const String& p_type) const override;
    bool generate_small_preview_automatically() const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;

    EditorTexturePreviewPlugin();
};

class EditorImagePreviewPlugin : public EditorResourcePreviewGenerator {
    GDCLASS(EditorImagePreviewPlugin, EditorResourcePreviewGenerator);

public:
    bool handles(const String& p_type) const override;
    bool generate_small_preview_automatically() const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;

    EditorImagePreviewPlugin();
};

class EditorBitmapPreviewPlugin : public EditorResourcePreviewGenerator {
    GDCLASS(EditorBitmapPreviewPlugin, EditorResourcePreviewGenerator);

public:
    bool handles(const String& p_type) const override;
    bool generate_small_preview_automatically() const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;

    EditorBitmapPreviewPlugin();
};

class EditorPackedScenePreviewPlugin : public EditorResourcePreviewGenerator {
public:
    bool handles(const String& p_type) const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;
    Ref<Texture> generate_from_path(
        const String& p_path,
        const Size2& p_size
    ) const override;

    EditorPackedScenePreviewPlugin();
};

class EditorMaterialPreviewPlugin : public EditorResourcePreviewGenerator {
    GDCLASS(EditorMaterialPreviewPlugin, EditorResourcePreviewGenerator);

    RID scenario;
    RID sphere;
    RID sphere_instance;
    RID viewport;
    RID viewport_texture;
    RID light;
    RID light_instance;
    RID light2;
    RID light_instance2;
    RID camera;
    mutable SafeFlag preview_done;

    void _preview_done(const Variant& p_udata);

protected:
    static void _bind_methods();

public:
    bool handles(const String& p_type) const override;
    bool generate_small_preview_automatically() const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;

    EditorMaterialPreviewPlugin();
    ~EditorMaterialPreviewPlugin() override;
};

class EditorScriptPreviewPlugin : public EditorResourcePreviewGenerator {
public:
    bool handles(const String& p_type) const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;

    EditorScriptPreviewPlugin();
};

class EditorAudioStreamPreviewPlugin : public EditorResourcePreviewGenerator {
public:
    bool handles(const String& p_type) const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;

    EditorAudioStreamPreviewPlugin();
};

class EditorMeshPreviewPlugin : public EditorResourcePreviewGenerator {
    GDCLASS(EditorMeshPreviewPlugin, EditorResourcePreviewGenerator);

    RID scenario;
    RID mesh_instance;
    RID viewport;
    RID viewport_texture;
    RID light;
    RID light_instance;
    RID light2;
    RID light_instance2;
    RID camera;
    mutable SafeFlag preview_done;

    void _preview_done(const Variant& p_udata);

protected:
    static void _bind_methods();

public:
    bool handles(const String& p_type) const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;

    EditorMeshPreviewPlugin();
    ~EditorMeshPreviewPlugin() override;
};

class EditorFontPreviewPlugin : public EditorResourcePreviewGenerator {
    GDCLASS(EditorFontPreviewPlugin, EditorResourcePreviewGenerator);

    RID viewport;
    RID viewport_texture;
    RID canvas;
    RID canvas_item;
    mutable SafeFlag preview_done;

    void _preview_done(const Variant& p_udata);

protected:
    static void _bind_methods();

public:
    bool handles(const String& p_type) const override;
    Ref<Texture> generate(
        const RES& p_from,
        const Size2& p_size
    ) const override;
    Ref<Texture> generate_from_path(
        const String& p_path,
        const Size2& p_size
    ) const override;

    EditorFontPreviewPlugin();
    ~EditorFontPreviewPlugin() override;
};
#endif // EDITOR_PREVIEW_PLUGINS_H
