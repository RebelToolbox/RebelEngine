// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef EDITORRESOURCEPREVIEW_H
#define EDITORRESOURCEPREVIEW_H

#include "core/os/semaphore.h"
#include "core/os/thread.h"
#include "core/safe_refcount.h"
#include "scene/main/node.h"
#include "scene/resources/texture.h"

class EditorResourcePreviewGenerator : public Reference {
    GDCLASS(EditorResourcePreviewGenerator, Reference);

protected:
    static void _bind_methods();

public:
    virtual bool handles(const String& p_type) const;
    virtual Ref<Texture> generate(const RES& p_from, const Size2& p_size) const;
    virtual Ref<Texture> generate_from_path(
        const String& p_path,
        const Size2& p_size
    ) const;

    virtual bool generate_small_preview_automatically() const;
    virtual bool can_generate_small_preview() const;

    EditorResourcePreviewGenerator();
};

class EditorResourcePreview : public Node {
    GDCLASS(EditorResourcePreview, Node);

    static EditorResourcePreview* singleton;

    struct QueueItem {
        Ref<Resource> resource;
        String path;
        ObjectID id;
        StringName function;
        Variant userdata;
    };

    List<QueueItem> queue;

    Mutex preview_mutex;
    Semaphore preview_sem;
    Thread thread;
    SafeFlag exit;
    SafeFlag exited;

    struct Item {
        Ref<Texture> preview;
        Ref<Texture> small_preview;
        int order;
        uint32_t last_hash;
        uint64_t modified_time;
    };

    int order;

    Map<String, Item> cache;

    void _preview_ready(
        const String& p_str,
        const Ref<Texture>& p_texture,
        const Ref<Texture>& p_small_texture,
        ObjectID id,
        const StringName& p_func,
        const Variant& p_ud
    );
    void _generate_preview(
        Ref<ImageTexture>& r_texture,
        Ref<ImageTexture>& r_small_texture,
        const QueueItem& p_item,
        const String& cache_base
    );

    static void _thread_func(void* ud);
    void _thread();

    Vector<Ref<EditorResourcePreviewGenerator>> preview_generators;

protected:
    static void _bind_methods();

public:
    static EditorResourcePreview* get_singleton();

    // p_receiver_func callback has signature (String p_path, Ref<Texture>
    // p_preview, Ref<Texture> p_preview_small, Variant p_userdata) p_preview
    // will be null if there was an error
    void queue_resource_preview(
        const String& p_path,
        Object* p_receiver,
        const StringName& p_receiver_func,
        const Variant& p_userdata
    );
    void queue_edited_resource_preview(
        const Ref<Resource>& p_res,
        Object* p_receiver,
        const StringName& p_receiver_func,
        const Variant& p_userdata
    );

    void add_preview_generator(
        const Ref<EditorResourcePreviewGenerator>& p_generator
    );
    void remove_preview_generator(
        const Ref<EditorResourcePreviewGenerator>& p_generator
    );
    void check_for_invalidation(const String& p_path);

    void start();
    void stop();

    EditorResourcePreview();
    ~EditorResourcePreview();
};

#endif // EDITORRESOURCEPREVIEW_H
