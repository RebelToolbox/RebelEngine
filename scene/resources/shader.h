// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SHADER_H
#define SHADER_H

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/resource.h"
#include "scene/resources/texture.h"

class Shader : public Resource {
    GDCLASS(Shader, Resource);
    OBJ_SAVE_TYPE(Shader);

public:
    enum Mode {
        MODE_SPATIAL,
        MODE_CANVAS_ITEM,
        MODE_PARTICLES,
        MODE_MAX
    };

private:
    RID shader;
    Mode mode;
    String shader_custom_defines;

    // hack the name of performance
    // shaders keep a list of ShaderMaterial -> VisualServer name translations,
    // to make conversion fast and save memory.
    mutable bool params_cache_dirty;
    mutable Map<StringName, StringName>
        params_cache; // map a shader param to a material param..
    Map<StringName, Ref<Texture>> default_textures;

    virtual void _update_shader() const; // used for visual shader

protected:
    static void _bind_methods();

public:
    // void set_mode(Mode p_mode);
    virtual Mode get_mode() const;

    void set_code(const String& p_code);
    String get_code() const;

    void get_param_list(List<PropertyInfo>* p_params) const;
    bool has_param(const StringName& p_param) const;

    void set_default_texture_param(
        const StringName& p_param,
        const Ref<Texture>& p_texture
    );
    Ref<Texture> get_default_texture_param(const StringName& p_param) const;
    void get_default_texture_param_list(List<StringName>* r_textures) const;

    void set_custom_defines(const String& p_defines);
    String get_custom_defines() const;

    virtual bool is_text_shader() const;

    _FORCE_INLINE_ StringName remap_param(const StringName& p_param) const {
        if (params_cache_dirty) {
            get_param_list(nullptr);
        }

        const Map<StringName, StringName>::Element* E =
            params_cache.find(p_param);
        if (E) {
            return E->get();
        }
        return StringName();
    }

    virtual RID get_rid() const;

    Shader();
    ~Shader();
};

VARIANT_ENUM_CAST(Shader::Mode);

class ResourceFormatLoaderShader : public ResourceFormatLoader {
public:
    virtual RES load(
        const String& p_path,
        const String& p_original_path = "",
        Error* r_error                = nullptr
    );
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual bool handles_type(const String& p_type) const;
    virtual String get_resource_type(const String& p_path) const;
};

class ResourceFormatSaverShader : public ResourceFormatSaver {
public:
    virtual Error save(
        const String& p_path,
        const RES& p_resource,
        uint32_t p_flags = 0
    );
    virtual void get_recognized_extensions(
        const RES& p_resource,
        List<String>* p_extensions
    ) const;
    virtual bool recognize(const RES& p_resource) const;
};

#endif // SHADER_H
