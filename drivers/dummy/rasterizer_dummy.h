// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RASTERIZER_DUMMY_H
#define RASTERIZER_DUMMY_H

#include "core/math/camera_matrix.h"
#include "core/self_list.h"
#include "scene/resources/mesh.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual_server.h"

class RasterizerSceneDummy : public RasterizerScene {
public:
    /* SHADOW ATLAS API */

    RID shadow_atlas_create() override {
        return RID();
    }

    void shadow_atlas_set_size(RID p_atlas, int p_size) override {}

    void shadow_atlas_set_quadrant_subdivision(
        RID p_atlas,
        int p_quadrant,
        int p_subdivision
    ) override {}

    bool shadow_atlas_update_light(
        RID p_atlas,
        RID p_light_intance,
        float p_coverage,
        uint64_t p_light_version
    ) override {
        return false;
    }

    int get_directional_light_shadow_size(RID p_light_intance) override {
        return 0;
    }

    void set_directional_shadow_count(int p_count) override {}

    /* ENVIRONMENT API */

    RID environment_create() override {
        return RID();
    }

    void environment_set_background(RID p_env, VS::EnvironmentBG p_bg)
        override {}

    void environment_set_sky(RID p_env, RID p_sky) override {}

    void environment_set_sky_custom_fov(RID p_env, float p_scale) override {}

    void environment_set_sky_orientation(RID p_env, const Basis& p_orientation)
        override {}

    void environment_set_bg_color(RID p_env, const Color& p_color) override {}

    void environment_set_bg_energy(RID p_env, float p_energy) override {}

    void environment_set_canvas_max_layer(RID p_env, int p_max_layer) override {
    }

    void environment_set_ambient_light(
        RID p_env,
        const Color& p_color,
        float p_energy           = 1.0,
        float p_sky_contribution = 0.0
    ) override {}

    void environment_set_camera_feed_id(RID p_env, int p_camera_feed_id)
        override {};

    void environment_set_dof_blur_near(
        RID p_env,
        bool p_enable,
        float p_distance,
        float p_transition,
        float p_far_amount,
        VS::EnvironmentDOFBlurQuality p_quality
    ) override {}

    void environment_set_dof_blur_far(
        RID p_env,
        bool p_enable,
        float p_distance,
        float p_transition,
        float p_far_amount,
        VS::EnvironmentDOFBlurQuality p_quality
    ) override {}

    void environment_set_glow(
        RID p_env,
        bool p_enable,
        int p_level_flags,
        float p_intensity,
        float p_strength,
        float p_bloom_threshold,
        VS::EnvironmentGlowBlendMode p_blend_mode,
        float p_hdr_bleed_threshold,
        float p_hdr_bleed_scale,
        float p_hdr_luminance_cap,
        bool p_bicubic_upscale,
        bool p_high_quality
    ) override {}

    void environment_set_fog(
        RID p_env,
        bool p_enable,
        float p_begin,
        float p_end,
        RID p_gradient_texture
    ) override {}

    void environment_set_ssr(
        RID p_env,
        bool p_enable,
        int p_max_steps,
        float p_fade_int,
        float p_fade_out,
        float p_depth_tolerance,
        bool p_roughness
    ) override {}

    void environment_set_ssao(
        RID p_env,
        bool p_enable,
        float p_radius,
        float p_intensity,
        float p_radius2,
        float p_intensity2,
        float p_bias,
        float p_light_affect,
        float p_ao_channel_affect,
        const Color& p_color,
        VS::EnvironmentSSAOQuality p_quality,
        VS::EnvironmentSSAOBlur p_blur,
        float p_bilateral_sharpness
    ) override {}

    void environment_set_tonemap(
        RID p_env,
        VS::EnvironmentToneMapper p_tone_mapper,
        float p_exposure,
        float p_white,
        bool p_auto_exposure,
        float p_min_luminance,
        float p_max_luminance,
        float p_auto_exp_speed,
        float p_auto_exp_scale
    ) override {}

    void environment_set_adjustment(
        RID p_env,
        bool p_enable,
        float p_brightness,
        float p_contrast,
        float p_saturation,
        RID p_ramp
    ) override {}

    void environment_set_fog(
        RID p_env,
        bool p_enable,
        const Color& p_color,
        const Color& p_sun_color,
        float p_sun_amount
    ) override {}

    void environment_set_fog_depth(
        RID p_env,
        bool p_enable,
        float p_depth_begin,
        float p_depth_end,
        float p_depth_curve,
        bool p_transmit,
        float p_transmit_curve
    ) override {}

    void environment_set_fog_height(
        RID p_env,
        bool p_enable,
        float p_min_height,
        float p_max_height,
        float p_height_curve
    ) override {}

    bool is_environment(RID p_env) override {
        return false;
    }

    VS::EnvironmentBG environment_get_background(RID p_env) override {
        return VS::ENV_BG_KEEP;
    }

    int environment_get_canvas_max_layer(RID p_env) override {
        return 0;
    }

    RID light_instance_create(RID p_light) override {
        return RID();
    }

    void light_instance_set_transform(
        RID p_light_instance,
        const Transform& p_transform
    ) override {}

    void light_instance_set_shadow_transform(
        RID p_light_instance,
        const CameraMatrix& p_projection,
        const Transform& p_transform,
        float p_far,
        float p_split,
        int p_pass,
        float p_bias_scale = 1.0
    ) override {}

    void light_instance_mark_visible(RID p_light_instance) override {}

    RID reflection_atlas_create() override {
        return RID();
    }

    void reflection_atlas_set_size(RID p_ref_atlas, int p_size) override {}

    void reflection_atlas_set_subdivision(RID p_ref_atlas, int p_subdiv)
        override {}

    RID reflection_probe_instance_create(RID p_probe) override {
        return RID();
    }

    void reflection_probe_instance_set_transform(
        RID p_instance,
        const Transform& p_transform
    ) override {}

    void reflection_probe_release_atlas_index(RID p_instance) override {}

    bool reflection_probe_instance_needs_redraw(RID p_instance) override {
        return false;
    }

    bool reflection_probe_instance_has_reflection(RID p_instance) override {
        return false;
    }

    bool reflection_probe_instance_begin_render(
        RID p_instance,
        RID p_reflection_atlas
    ) override {
        return false;
    }

    bool reflection_probe_instance_postprocess_step(RID p_instance) override {
        return true;
    }

    RID gi_probe_instance_create() override {
        return RID();
    }

    void gi_probe_instance_set_light_data(RID p_probe, RID p_base, RID p_data)
        override {}

    void gi_probe_instance_set_transform_to_data(
        RID p_probe,
        const Transform& p_xform
    ) override {}

    void gi_probe_instance_set_bounds(RID p_probe, const Vector3& p_bounds)
        override {}

    void render_scene(
        const Transform& p_cam_transform,
        const CameraMatrix& p_cam_projection,
        const int p_eye,
        bool p_cam_ortogonal,
        InstanceBase** p_cull_result,
        int p_cull_count,
        RID* p_light_cull_result,
        int p_light_cull_count,
        RID* p_reflection_probe_cull_result,
        int p_reflection_probe_cull_count,
        RID p_environment,
        RID p_shadow_atlas,
        RID p_reflection_atlas,
        RID p_reflection_probe,
        int p_reflection_probe_pass
    ) override {}

    void render_shadow(
        RID p_light,
        RID p_shadow_atlas,
        int p_pass,
        InstanceBase** p_cull_result,
        int p_cull_count
    ) override {}

    void set_scene_pass(uint64_t p_pass) override {}

    void set_debug_draw_mode(VS::ViewportDebugDraw p_debug_draw) override {}

    bool free(RID p_rid) override {
        return true;
    }

    RasterizerSceneDummy() = default;

    ~RasterizerSceneDummy() override = default;
};

class RasterizerStorageDummy : public RasterizerStorage {
public:
    /* TEXTURE API */
    struct DummyTexture : public RID_Data {
        int width;
        int height;
        uint32_t flags;
        Image::Format format;
        Ref<Image> image;
        String path;
    };

    struct DummySurface {
        uint32_t format;
        VS::PrimitiveType primitive;
        PoolVector<uint8_t> array;
        int vertex_count;
        PoolVector<uint8_t> index_array;
        int index_count;
        AABB aabb;
        Vector<PoolVector<uint8_t>> blend_shapes;
        Vector<AABB> bone_aabbs;
    };

    struct DummyMesh : public RID_Data {
        Vector<DummySurface> surfaces;
        int blend_shape_count;
        VS::BlendShapeMode blend_shape_mode;
        PoolRealArray blend_shape_values;
    };

    mutable RID_Owner<DummyTexture> texture_owner;
    mutable RID_Owner<DummyMesh> mesh_owner;

    RID texture_create() override {
        DummyTexture* texture = memnew(DummyTexture);
        ERR_FAIL_COND_V(!texture, RID());
        return texture_owner.make_rid(texture);
    }

    void texture_allocate(
        RID p_texture,
        int p_width,
        int p_height,
        int p_depth_3d,
        Image::Format p_format,
        VisualServer::TextureType p_type = VS::TEXTURE_TYPE_2D,
        uint32_t p_flags                 = VS::TEXTURE_FLAGS_DEFAULT
    ) override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND(!t);
        t->width  = p_width;
        t->height = p_height;
        t->flags  = p_flags;
        t->format = p_format;
        t->image  = Ref<Image>(memnew(Image));
        t->image->create(p_width, p_height, false, p_format);
    }

    void texture_set_data(RID p_texture, const Ref<Image>& p_image, int p_level)
        override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND(!t);
        t->width  = p_image->get_width();
        t->height = p_image->get_height();
        t->format = p_image->get_format();
        t->image->create(
            t->width,
            t->height,
            false,
            t->format,
            p_image->get_data()
        );
    }

    void texture_set_data_partial(
        RID p_texture,
        const Ref<Image>& p_image,
        int src_x,
        int src_y,
        int src_w,
        int src_h,
        int dst_x,
        int dst_y,
        int p_dst_mip,
        int p_level
    ) override {
        DummyTexture* t = texture_owner.get(p_texture);

        ERR_FAIL_COND(!t);
        ERR_FAIL_COND_MSG(
            p_image.is_null(),
            "It's not a reference to a valid Image object."
        );
        ERR_FAIL_COND(t->format != p_image->get_format());
        ERR_FAIL_COND(src_w <= 0 || src_h <= 0);
        ERR_FAIL_COND(
            src_x < 0 || src_y < 0 || src_x + src_w > p_image->get_width()
            || src_y + src_h > p_image->get_height()
        );
        ERR_FAIL_COND(
            dst_x < 0 || dst_y < 0 || dst_x + src_w > t->width
            || dst_y + src_h > t->height
        );

        t->image->blit_rect(
            p_image,
            Rect2(src_x, src_y, src_w, src_h),
            Vector2(dst_x, dst_y)
        );
    }

    Ref<Image> texture_get_data(RID p_texture, int p_level) const override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND_V(!t, Ref<Image>());
        return t->image;
    }

    void texture_set_flags(RID p_texture, uint32_t p_flags) override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND(!t);
        t->flags = p_flags;
    }

    uint32_t texture_get_flags(RID p_texture) const override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND_V(!t, 0);
        return t->flags;
    }

    Image::Format texture_get_format(RID p_texture) const override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND_V(!t, Image::FORMAT_RGB8);
        return t->format;
    }

    VisualServer::TextureType texture_get_type(RID p_texture) const override {
        return VS::TEXTURE_TYPE_2D;
    }

    uint32_t texture_get_texid(RID p_texture) const override {
        return 0;
    }

    uint32_t texture_get_width(RID p_texture) const override {
        return 0;
    }

    uint32_t texture_get_height(RID p_texture) const override {
        return 0;
    }

    uint32_t texture_get_depth(RID p_texture) const override {
        return 0;
    }

    void texture_set_size_override(
        RID p_texture,
        int p_width,
        int p_height,
        int p_depth_3d
    ) override {}

    void texture_bind(RID p_texture, uint32_t p_texture_no) override {}

    void texture_set_path(RID p_texture, const String& p_path) override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND(!t);
        t->path = p_path;
    }

    String texture_get_path(RID p_texture) const override {
        DummyTexture* t = texture_owner.getornull(p_texture);
        ERR_FAIL_COND_V(!t, String());
        return t->path;
    }

    void texture_set_shrink_all_x2_on_set_data(bool p_enable) override {}

    void texture_debug_usage(List<VS::TextureInfo>* r_info) override {}

    RID texture_create_radiance_cubemap(RID p_source, int p_resolution = -1)
        const override {
        return RID();
    }

    void texture_set_detect_3d_callback(
        RID p_texture,
        VisualServer::TextureDetectCallback p_callback,
        void* p_userdata
    ) override {}

    void texture_set_detect_srgb_callback(
        RID p_texture,
        VisualServer::TextureDetectCallback p_callback,
        void* p_userdata
    ) override {}

    void texture_set_detect_normal_callback(
        RID p_texture,
        VisualServer::TextureDetectCallback p_callback,
        void* p_userdata
    ) override {}

    void textures_keep_original(bool p_enable) override {}

    void texture_set_proxy(RID p_proxy, RID p_base) override {}

    Size2 texture_size_with_proxy(RID p_texture) const override {
        return Size2();
    }

    void texture_set_force_redraw_if_visible(RID p_texture, bool p_enable)
        override {}

    /* SKY API */

    RID sky_create() override {
        return RID();
    }

    void sky_set_texture(RID p_sky, RID p_cube_map, int p_radiance_size)
        override {}

    /* SHADER API */

    RID shader_create() override {
        return RID();
    }

    void shader_set_code(RID p_shader, const String& p_code) override {}

    String shader_get_code(RID p_shader) const override {
        return "";
    }

    void shader_get_param_list(RID p_shader, List<PropertyInfo>* p_param_list)
        const override {}

    void shader_set_default_texture_param(
        RID p_shader,
        const StringName& p_name,
        RID p_texture
    ) override {}

    RID shader_get_default_texture_param(RID p_shader, const StringName& p_name)
        const override {
        return RID();
    }

    void shader_add_custom_define(RID p_shader, const String& p_define)
        override {}

    void shader_get_custom_defines(RID p_shader, Vector<String>* p_defines)
        const override {}

    void shader_remove_custom_define(RID p_shader, const String& p_define)
        override {}

    /* COMMON MATERIAL API */

    RID material_create() override {
        return RID();
    }

    void material_set_render_priority(RID p_material, int priority) override {}

    void material_set_shader(RID p_shader_material, RID p_shader) override {}

    RID material_get_shader(RID p_shader_material) const override {
        return RID();
    }

    void material_set_param(
        RID p_material,
        const StringName& p_param,
        const Variant& p_value
    ) override {}

    Variant material_get_param(RID p_material, const StringName& p_param)
        const override {
        return Variant();
    }

    Variant material_get_param_default(
        RID p_material,
        const StringName& p_param
    ) const override {
        return Variant();
    }

    void material_set_line_width(RID p_material, float p_width) override {}

    void material_set_next_pass(RID p_material, RID p_next_material) override {}

    bool material_is_animated(RID p_material) override {
        return false;
    }

    bool material_casts_shadows(RID p_material) override {
        return false;
    }

    void material_add_instance_owner(
        RID p_material,
        RasterizerScene::InstanceBase* p_instance
    ) override {}

    void material_remove_instance_owner(
        RID p_material,
        RasterizerScene::InstanceBase* p_instance
    ) override {}

    /* MESH API */

    RID mesh_create() override {
        DummyMesh* mesh = memnew(DummyMesh);
        ERR_FAIL_COND_V(!mesh, RID());
        mesh->blend_shape_count = 0;
        mesh->blend_shape_mode  = VS::BLEND_SHAPE_MODE_NORMALIZED;
        return mesh_owner.make_rid(mesh);
    }

    void mesh_add_surface(
        RID p_mesh,
        uint32_t p_format,
        VS::PrimitiveType p_primitive,
        const PoolVector<uint8_t>& p_array,
        int p_vertex_count,
        const PoolVector<uint8_t>& p_index_array,
        int p_index_count,
        const AABB& p_aabb,
        const Vector<PoolVector<uint8_t>>& p_blend_shapes =
            Vector<PoolVector<uint8_t>>(),
        const Vector<AABB>& p_bone_aabbs = Vector<AABB>()
    ) override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND(!m);

        m->surfaces.push_back(DummySurface());
        DummySurface* s = &m->surfaces.write[m->surfaces.size() - 1];
        s->format       = p_format;
        s->primitive    = p_primitive;
        s->array        = p_array;
        s->vertex_count = p_vertex_count;
        s->index_array  = p_index_array;
        s->index_count  = p_index_count;
        s->aabb         = p_aabb;
        s->blend_shapes = p_blend_shapes;
        s->bone_aabbs   = p_bone_aabbs;
    }

    void mesh_set_blend_shape_count(RID p_mesh, int p_amount) override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND(!m);
        m->blend_shape_count = p_amount;
    }

    int mesh_get_blend_shape_count(RID p_mesh) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, 0);
        return m->blend_shape_count;
    }

    void mesh_set_blend_shape_mode(RID p_mesh, VS::BlendShapeMode p_mode)
        override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND(!m);
        m->blend_shape_mode = p_mode;
    }

    VS::BlendShapeMode mesh_get_blend_shape_mode(RID p_mesh) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, VS::BLEND_SHAPE_MODE_NORMALIZED);
        return m->blend_shape_mode;
    }

    void mesh_set_blend_shape_values(RID p_mesh, PoolVector<float> p_values)
        override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND(!m);
        m->blend_shape_values = p_values;
    }

    PoolVector<float> mesh_get_blend_shape_values(RID p_mesh) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, PoolRealArray());
        return m->blend_shape_values;
    }

    void mesh_surface_update_region(
        RID p_mesh,
        int p_surface,
        int p_offset,
        const PoolVector<uint8_t>& p_data
    ) override {}

    void mesh_surface_set_material(RID p_mesh, int p_surface, RID p_material)
        override {}

    RID mesh_surface_get_material(RID p_mesh, int p_surface) const override {
        return RID();
    }

    int mesh_surface_get_array_len(RID p_mesh, int p_surface) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, 0);

        return m->surfaces[p_surface].vertex_count;
    }

    int mesh_surface_get_array_index_len(RID p_mesh, int p_surface)
        const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, 0);

        return m->surfaces[p_surface].index_count;
    }

    PoolVector<uint8_t> mesh_surface_get_array(RID p_mesh, int p_surface)
        const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, PoolVector<uint8_t>());

        return m->surfaces[p_surface].array;
    }

    PoolVector<uint8_t> mesh_surface_get_index_array(RID p_mesh, int p_surface)
        const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, PoolVector<uint8_t>());

        return m->surfaces[p_surface].index_array;
    }

    uint32_t mesh_surface_get_format(RID p_mesh, int p_surface) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, 0);

        return m->surfaces[p_surface].format;
    }

    VS::PrimitiveType mesh_surface_get_primitive_type(RID p_mesh, int p_surface)
        const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, VS::PRIMITIVE_POINTS);

        return m->surfaces[p_surface].primitive;
    }

    AABB mesh_surface_get_aabb(RID p_mesh, int p_surface) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, AABB());

        return m->surfaces[p_surface].aabb;
    }

    Vector<PoolVector<uint8_t>> mesh_surface_get_blend_shapes(
        RID p_mesh,
        int p_surface
    ) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, Vector<PoolVector<uint8_t>>());

        return m->surfaces[p_surface].blend_shapes;
    }

    Vector<AABB> mesh_surface_get_skeleton_aabb(RID p_mesh, int p_surface)
        const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, Vector<AABB>());

        return m->surfaces[p_surface].bone_aabbs;
    }

    void mesh_remove_surface(RID p_mesh, int p_index) override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND(!m);
        ERR_FAIL_COND(p_index >= m->surfaces.size());

        m->surfaces.remove(p_index);
    }

    int mesh_get_surface_count(RID p_mesh) const override {
        DummyMesh* m = mesh_owner.getornull(p_mesh);
        ERR_FAIL_COND_V(!m, 0);
        return m->surfaces.size();
    }

    void mesh_set_custom_aabb(RID p_mesh, const AABB& p_aabb) override {}

    AABB mesh_get_custom_aabb(RID p_mesh) const override {
        return AABB();
    }

    AABB mesh_get_aabb(RID p_mesh, RID p_skeleton) const override {
        return AABB();
    }

    void mesh_clear(RID p_mesh) override {}

    /* MULTIMESH API */

    RID multimesh_create() override {
        return RID();
    }

    void multimesh_allocate(
        RID p_multimesh,
        int p_instances,
        VS::MultimeshTransformFormat p_transform_format,
        VS::MultimeshColorFormat p_color_format,
        VS::MultimeshCustomDataFormat p_data = VS::MULTIMESH_CUSTOM_DATA_NONE
    ) override {}

    int multimesh_get_instance_count(RID p_multimesh) const override {
        return 0;
    }

    void multimesh_set_mesh(RID p_multimesh, RID p_mesh) override {}

    void multimesh_instance_set_transform(
        RID p_multimesh,
        int p_index,
        const Transform& p_transform
    ) override {}

    void multimesh_instance_set_transform_2d(
        RID p_multimesh,
        int p_index,
        const Transform2D& p_transform
    ) override {}

    void multimesh_instance_set_color(
        RID p_multimesh,
        int p_index,
        const Color& p_color
    ) override {}

    void multimesh_instance_set_custom_data(
        RID p_multimesh,
        int p_index,
        const Color& p_color
    ) override {}

    RID multimesh_get_mesh(RID p_multimesh) const override {
        return RID();
    }

    Transform multimesh_instance_get_transform(RID p_multimesh, int p_index)
        const override {
        return Transform();
    }

    Transform2D multimesh_instance_get_transform_2d(
        RID p_multimesh,
        int p_index
    ) const override {
        return Transform2D();
    }

    Color multimesh_instance_get_color(RID p_multimesh, int p_index)
        const override {
        return Color();
    }

    Color multimesh_instance_get_custom_data(RID p_multimesh, int p_index)
        const override {
        return Color();
    }

    void multimesh_set_as_bulk_array(
        RID p_multimesh,
        const PoolVector<float>& p_array
    ) override {}

    void multimesh_set_visible_instances(RID p_multimesh, int p_visible)
        override {}

    int multimesh_get_visible_instances(RID p_multimesh) const override {
        return 0;
    }

    AABB multimesh_get_aabb(RID p_multimesh) const override {
        return AABB();
    }

    /* IMMEDIATE API */

    RID immediate_create() override {
        return RID();
    }

    void immediate_begin(
        RID p_immediate,
        VS::PrimitiveType p_rimitive,
        RID p_texture = RID()
    ) override {}

    void immediate_vertex(RID p_immediate, const Vector3& p_vertex) override {}

    void immediate_normal(RID p_immediate, const Vector3& p_normal) override {}

    void immediate_tangent(RID p_immediate, const Plane& p_tangent) override {}

    void immediate_color(RID p_immediate, const Color& p_color) override {}

    void immediate_uv(RID p_immediate, const Vector2& tex_uv) override {}

    void immediate_uv2(RID p_immediate, const Vector2& tex_uv) override {}

    void immediate_end(RID p_immediate) override {}

    void immediate_clear(RID p_immediate) override {}

    void immediate_set_material(RID p_immediate, RID p_material) override {}

    RID immediate_get_material(RID p_immediate) const override {
        return RID();
    }

    AABB immediate_get_aabb(RID p_immediate) const override {
        return AABB();
    }

    /* SKELETON API */

    RID skeleton_create() override {
        return RID();
    }

    void skeleton_allocate(
        RID p_skeleton,
        int p_bones,
        bool p_2d_skeleton = false
    ) override {}

    void skeleton_set_base_transform_2d(
        RID p_skeleton,
        const Transform2D& p_base_transform
    ) override {}

    void skeleton_set_world_transform(
        RID p_skeleton,
        bool p_enable,
        const Transform& p_world_transform
    ) {}

    int skeleton_get_bone_count(RID p_skeleton) const override {
        return 0;
    }

    void skeleton_bone_set_transform(
        RID p_skeleton,
        int p_bone,
        const Transform& p_transform
    ) override {}

    Transform skeleton_bone_get_transform(RID p_skeleton, int p_bone)
        const override {
        return Transform();
    }

    void skeleton_bone_set_transform_2d(
        RID p_skeleton,
        int p_bone,
        const Transform2D& p_transform
    ) override {}

    Transform2D skeleton_bone_get_transform_2d(RID p_skeleton, int p_bone)
        const override {
        return Transform2D();
    }

    /* Light API */

    RID light_create(VS::LightType p_type) override {
        return RID();
    }

    RID directional_light_create() {
        return light_create(VS::LIGHT_DIRECTIONAL);
    }

    RID omni_light_create() {
        return light_create(VS::LIGHT_OMNI);
    }

    RID spot_light_create() {
        return light_create(VS::LIGHT_SPOT);
    }

    void light_set_color(RID p_light, const Color& p_color) override {}

    void light_set_param(RID p_light, VS::LightParam p_param, float p_value)
        override {}

    void light_set_shadow(RID p_light, bool p_enabled) override {}

    void light_set_shadow_color(RID p_light, const Color& p_color) override {}

    void light_set_projector(RID p_light, RID p_texture) override {}

    void light_set_negative(RID p_light, bool p_enable) override {}

    void light_set_cull_mask(RID p_light, uint32_t p_mask) override {}

    void light_set_reverse_cull_face_mode(RID p_light, bool p_enabled)
        override {}

    void light_set_use_gi(RID p_light, bool p_enabled) override {}

    void light_set_bake_mode(RID p_light, VS::LightBakeMode p_bake_mode)
        override {}

    void light_omni_set_shadow_mode(RID p_light, VS::LightOmniShadowMode p_mode)
        override {}

    void light_omni_set_shadow_detail(
        RID p_light,
        VS::LightOmniShadowDetail p_detail
    ) override {}

    void light_directional_set_shadow_mode(
        RID p_light,
        VS::LightDirectionalShadowMode p_mode
    ) override {}

    void light_directional_set_blend_splits(RID p_light, bool p_enable)
        override {}

    bool light_directional_get_blend_splits(RID p_light) const override {
        return false;
    }

    void light_directional_set_shadow_depth_range_mode(
        RID p_light,
        VS::LightDirectionalShadowDepthRangeMode p_range_mode
    ) override {}

    VS::LightDirectionalShadowDepthRangeMode
    light_directional_get_shadow_depth_range_mode(RID p_light) const override {
        return VS::LIGHT_DIRECTIONAL_SHADOW_DEPTH_RANGE_STABLE;
    }

    VS::LightDirectionalShadowMode light_directional_get_shadow_mode(RID p_light
    ) override {
        return VS::LIGHT_DIRECTIONAL_SHADOW_ORTHOGONAL;
    }

    VS::LightOmniShadowMode light_omni_get_shadow_mode(RID p_light) override {
        return VS::LIGHT_OMNI_SHADOW_DUAL_PARABOLOID;
    }

    bool light_has_shadow(RID p_light) const override {
        return false;
    }

    VS::LightType light_get_type(RID p_light) const override {
        return VS::LIGHT_OMNI;
    }

    AABB light_get_aabb(RID p_light) const override {
        return AABB();
    }

    float light_get_param(RID p_light, VS::LightParam p_param) override {
        return 0.0;
    }

    Color light_get_color(RID p_light) override {
        return Color();
    }

    bool light_get_use_gi(RID p_light) override {
        return false;
    }

    VS::LightBakeMode light_get_bake_mode(RID p_light) override {
        return VS::LightBakeMode::LIGHT_BAKE_DISABLED;
    }

    uint64_t light_get_version(RID p_light) const override {
        return 0;
    }

    /* PROBE API */

    RID reflection_probe_create() override {
        return RID();
    }

    void reflection_probe_set_update_mode(
        RID p_probe,
        VS::ReflectionProbeUpdateMode p_mode
    ) override {}

    void reflection_probe_set_intensity(RID p_probe, float p_intensity)
        override {}

    void reflection_probe_set_interior_ambient(
        RID p_probe,
        const Color& p_ambient
    ) override {}

    void reflection_probe_set_interior_ambient_energy(
        RID p_probe,
        float p_energy
    ) override {}

    void reflection_probe_set_interior_ambient_probe_contribution(
        RID p_probe,
        float p_contrib
    ) override {}

    void reflection_probe_set_max_distance(RID p_probe, float p_distance)
        override {}

    void reflection_probe_set_extents(RID p_probe, const Vector3& p_extents)
        override {}

    void reflection_probe_set_origin_offset(
        RID p_probe,
        const Vector3& p_offset
    ) override {}

    void reflection_probe_set_as_interior(RID p_probe, bool p_enable) override {
    }

    void reflection_probe_set_enable_box_projection(RID p_probe, bool p_enable)
        override {}

    void reflection_probe_set_enable_shadows(RID p_probe, bool p_enable)
        override {}

    void reflection_probe_set_cull_mask(RID p_probe, uint32_t p_layers)
        override {}

    void reflection_probe_set_resolution(RID p_probe, int p_resolution)
        override {}

    AABB reflection_probe_get_aabb(RID p_probe) const override {
        return AABB();
    }

    VS::ReflectionProbeUpdateMode reflection_probe_get_update_mode(RID p_probe
    ) const override {
        return VisualServer::REFLECTION_PROBE_UPDATE_ONCE;
    }

    uint32_t reflection_probe_get_cull_mask(RID p_probe) const override {
        return 0;
    }

    Vector3 reflection_probe_get_extents(RID p_probe) const override {
        return Vector3();
    }

    Vector3 reflection_probe_get_origin_offset(RID p_probe) const override {
        return Vector3();
    }

    float reflection_probe_get_origin_max_distance(RID p_probe) const override {
        return 0.0;
    }

    bool reflection_probe_renders_shadows(RID p_probe) const override {
        return false;
    }

    void instance_add_skeleton(
        RID p_skeleton,
        RasterizerScene::InstanceBase* p_instance
    ) override {}

    void instance_remove_skeleton(
        RID p_skeleton,
        RasterizerScene::InstanceBase* p_instance
    ) override {}

    void instance_add_dependency(
        RID p_base,
        RasterizerScene::InstanceBase* p_instance
    ) override {}

    void instance_remove_dependency(
        RID p_base,
        RasterizerScene::InstanceBase* p_instance
    ) override {}

    /* GI PROBE API */

    RID gi_probe_create() override {
        return RID();
    }

    void gi_probe_set_bounds(RID p_probe, const AABB& p_bounds) override {}

    AABB gi_probe_get_bounds(RID p_probe) const override {
        return AABB();
    }

    void gi_probe_set_cell_size(RID p_probe, float p_range) override {}

    float gi_probe_get_cell_size(RID p_probe) const override {
        return 0.0;
    }

    void gi_probe_set_to_cell_xform(RID p_probe, const Transform& p_xform)
        override {}

    Transform gi_probe_get_to_cell_xform(RID p_probe) const override {
        return Transform();
    }

    void gi_probe_set_dynamic_data(RID p_probe, const PoolVector<int>& p_data)
        override {}

    PoolVector<int> gi_probe_get_dynamic_data(RID p_probe) const override {
        PoolVector<int> p;
        return p;
    }

    void gi_probe_set_dynamic_range(RID p_probe, int p_range) override {}

    int gi_probe_get_dynamic_range(RID p_probe) const override {
        return 0;
    }

    void gi_probe_set_energy(RID p_probe, float p_range) override {}

    float gi_probe_get_energy(RID p_probe) const override {
        return 0.0;
    }

    void gi_probe_set_bias(RID p_probe, float p_range) override {}

    float gi_probe_get_bias(RID p_probe) const override {
        return 0.0;
    }

    void gi_probe_set_normal_bias(RID p_probe, float p_range) override {}

    float gi_probe_get_normal_bias(RID p_probe) const override {
        return 0.0;
    }

    void gi_probe_set_propagation(RID p_probe, float p_range) override {}

    float gi_probe_get_propagation(RID p_probe) const override {
        return 0.0;
    }

    void gi_probe_set_interior(RID p_probe, bool p_enable) override {}

    bool gi_probe_is_interior(RID p_probe) const override {
        return false;
    }

    void gi_probe_set_compress(RID p_probe, bool p_enable) override {}

    bool gi_probe_is_compressed(RID p_probe) const override {
        return false;
    }

    uint32_t gi_probe_get_version(RID p_probe) override {
        return 0;
    }

    RID gi_probe_dynamic_data_create(
        int p_width,
        int p_height,
        int p_depth,
        GIProbeCompression p_compression
    ) override {
        return RID();
    }

    void gi_probe_dynamic_data_update(
        RID p_gi_probe_data,
        int p_depth_slice,
        int p_slice_count,
        int p_mipmap,
        const void* p_data
    ) override {}

    /* LIGHTMAP CAPTURE */
    struct Instantiable : public RID_Data {
        SelfList<RasterizerScene::InstanceBase>::List instance_list;

        _FORCE_INLINE_ void instance_change_notify(
            bool p_aabb      = true,
            bool p_materials = true
        ) {
            SelfList<RasterizerScene::InstanceBase>* instances =
                instance_list.get_first();
            while (instances) {
                instances->get_self()->base_changed(p_aabb, p_materials);
                instances = instances->get_next();
            }
        }

        _FORCE_INLINE_ void instance_remove_deps() {
            SelfList<RasterizerScene::InstanceBase>* instances =
                instance_list.get_first();
            while (instances) {
                SelfList<RasterizerScene::InstanceBase>* next =
                    instances->get_next();
                instances->get_self()->base_removed();
                instances = next;
            }
        }

        Instantiable() = default;

        ~Instantiable() override = default;
    };

    struct LightmapCapture : public Instantiable {
        PoolVector<LightmapCaptureOctree> octree;
        AABB bounds;
        Transform cell_xform;
        int cell_subdiv;
        float energy;

        LightmapCapture() {
            energy      = 1.0;
            cell_subdiv = 1;
        }
    };

    mutable RID_Owner<LightmapCapture> lightmap_capture_data_owner;

    void lightmap_capture_set_bounds(RID p_capture, const AABB& p_bounds)
        override {}

    AABB lightmap_capture_get_bounds(RID p_capture) const override {
        return AABB();
    }

    void lightmap_capture_set_octree(
        RID p_capture,
        const PoolVector<uint8_t>& p_octree
    ) override {}

    RID lightmap_capture_create() override {
        LightmapCapture* capture = memnew(LightmapCapture);
        return lightmap_capture_data_owner.make_rid(capture);
    }

    PoolVector<uint8_t> lightmap_capture_get_octree(RID p_capture
    ) const override {
        const LightmapCapture* capture =
            lightmap_capture_data_owner.getornull(p_capture);
        ERR_FAIL_COND_V(!capture, PoolVector<uint8_t>());
        return PoolVector<uint8_t>();
    }

    void lightmap_capture_set_octree_cell_transform(
        RID p_capture,
        const Transform& p_xform
    ) override {}

    Transform lightmap_capture_get_octree_cell_transform(RID p_capture
    ) const override {
        return Transform();
    }

    void lightmap_capture_set_octree_cell_subdiv(RID p_capture, int p_subdiv)
        override {}

    int lightmap_capture_get_octree_cell_subdiv(RID p_capture) const override {
        return 0;
    }

    void lightmap_capture_set_energy(RID p_capture, float p_energy) override {}

    float lightmap_capture_get_energy(RID p_capture) const override {
        return 0.0;
    }

    void lightmap_capture_set_interior(RID p_capture, bool p_interior)
        override {}

    bool lightmap_capture_is_interior(RID p_capture) const override {
        return false;
    }

    const PoolVector<LightmapCaptureOctree>* lightmap_capture_get_octree_ptr(
        RID p_capture
    ) const override {
        const LightmapCapture* capture =
            lightmap_capture_data_owner.getornull(p_capture);
        ERR_FAIL_COND_V(!capture, NULL);
        return &capture->octree;
    }

    /* PARTICLES */

    RID particles_create() override {
        return RID();
    }

    void particles_set_emitting(RID p_particles, bool p_emitting) override {}

    void particles_set_amount(RID p_particles, int p_amount) override {}

    void particles_set_lifetime(RID p_particles, float p_lifetime) override {}

    void particles_set_one_shot(RID p_particles, bool p_one_shot) override {}

    void particles_set_pre_process_time(RID p_particles, float p_time)
        override {}

    void particles_set_explosiveness_ratio(RID p_particles, float p_ratio)
        override {}

    void particles_set_randomness_ratio(RID p_particles, float p_ratio)
        override {}

    void particles_set_custom_aabb(RID p_particles, const AABB& p_aabb)
        override {}

    void particles_set_speed_scale(RID p_particles, float p_scale) override {}

    void particles_set_use_local_coordinates(RID p_particles, bool p_enable)
        override {}

    void particles_set_process_material(RID p_particles, RID p_material)
        override {}

    void particles_set_fixed_fps(RID p_particles, int p_fps) override {}

    void particles_set_fractional_delta(RID p_particles, bool p_enable)
        override {}

    void particles_restart(RID p_particles) override {}

    void particles_set_draw_order(
        RID p_particles,
        VS::ParticlesDrawOrder p_order
    ) override {}

    void particles_set_draw_passes(RID p_particles, int p_count) override {}

    void particles_set_draw_pass_mesh(RID p_particles, int p_pass, RID p_mesh)
        override {}

    void particles_request_process(RID p_particles) override {}

    AABB particles_get_current_aabb(RID p_particles) override {
        return AABB();
    }

    AABB particles_get_aabb(RID p_particles) const override {
        return AABB();
    }

    void particles_set_emission_transform(
        RID p_particles,
        const Transform& p_transform
    ) override {}

    bool particles_get_emitting(RID p_particles) override {
        return false;
    }

    int particles_get_draw_passes(RID p_particles) const override {
        return 0;
    }

    RID particles_get_draw_pass_mesh(RID p_particles, int p_pass)
        const override {
        return RID();
    }

    bool particles_is_inactive(RID p_particles) const override {
        return false;
    }

    /* RENDER TARGET */

    RID render_target_create() override {
        return RID();
    }

    void render_target_set_position(RID p_render_target, int p_x, int p_y)
        override {}

    void render_target_set_size(RID p_render_target, int p_width, int p_height)
        override {}

    RID render_target_get_texture(RID p_render_target) const override {
        return RID();
    }

    uint32_t render_target_get_depth_texture_id(RID p_render_target
    ) const override {
        return 0;
    }

    void render_target_set_external_texture(
        RID p_render_target,
        unsigned int p_texture_id,
        unsigned int p_depth_id
    ) override {}

    void render_target_set_flag(
        RID p_render_target,
        RenderTargetFlags p_flag,
        bool p_value
    ) override {}

    bool render_target_was_used(RID p_render_target) override {
        return false;
    }

    void render_target_clear_used(RID p_render_target) override {}

    void render_target_set_msaa(RID p_render_target, VS::ViewportMSAA p_msaa)
        override {}

    void render_target_set_use_fxaa(RID p_render_target, bool p_fxaa) override {
    }

    void render_target_set_use_debanding(RID p_render_target, bool p_debanding)
        override {}

    void render_target_set_sharpen_intensity(
        RID p_render_target,
        float p_intensity
    ) override {}

    /* CANVAS SHADOW */

    RID canvas_light_shadow_buffer_create(int p_width) override {
        return RID();
    }

    /* LIGHT SHADOW MAPPING */

    RID canvas_light_occluder_create() override {
        return RID();
    }

    void canvas_light_occluder_set_polylines(
        RID p_occluder,
        const PoolVector<Vector2>& p_lines
    ) override {}

    VS::InstanceType get_base_type(RID p_rid) const override {
        if (mesh_owner.owns(p_rid)) {
            return VS::INSTANCE_MESH;
        } else if (lightmap_capture_data_owner.owns(p_rid)) {
            return VS::INSTANCE_LIGHTMAP_CAPTURE;
        }

        return VS::INSTANCE_NONE;
    }

    bool free(RID p_rid) override {
        if (texture_owner.owns(p_rid)) {
            // delete the texture
            DummyTexture* texture = texture_owner.get(p_rid);
            texture_owner.free(p_rid);
            memdelete(texture);
        } else if (mesh_owner.owns(p_rid)) {
            // delete the mesh
            DummyMesh* mesh = mesh_owner.getornull(p_rid);
            mesh_owner.free(p_rid);
            memdelete(mesh);
        } else if (lightmap_capture_data_owner.owns(p_rid)) {
            // delete the lightmap
            LightmapCapture* lightmap_capture =
                lightmap_capture_data_owner.getornull(p_rid);
            lightmap_capture_data_owner.free(p_rid);
            memdelete(lightmap_capture);
        } else {
            return false;
        }

        return true;
    }

    bool has_os_feature(const String& p_feature) const override {
        return false;
    }

    void update_dirty_resources() override {}

    void set_debug_generate_wireframes(bool p_generate) override {}

    void render_info_begin_capture() override {}

    void render_info_end_capture() override {}

    int get_captured_render_info(VS::RenderInfo p_info) override {
        return 0;
    }

    uint64_t get_render_info(VS::RenderInfo p_info) override {
        return 0;
    }

    String get_video_adapter_name() const override {
        return String();
    }

    String get_video_adapter_vendor() const override {
        return String();
    }

    static RasterizerStorage* base_singleton;

    RasterizerStorageDummy() = default;

    ~RasterizerStorageDummy() override = default;
};

class RasterizerCanvasDummy : public RasterizerCanvas {
public:
    RID light_internal_create() override {
        return RID();
    }

    void light_internal_update(RID p_rid, Light* p_light) override {}

    void light_internal_free(RID p_rid) override {}

    void canvas_begin() override {};
    void canvas_end() override {};

    void canvas_render_items(
        Item* p_item_list,
        int p_z,
        const Color& p_modulate,
        Light* p_light,
        const Transform2D& p_transform
    ) override {};
    void canvas_debug_viewport_shadows(Light* p_lights_with_shadow) override {};

    void canvas_light_shadow_buffer_update(
        RID p_buffer,
        const Transform2D& p_light_xform,
        int p_light_mask,
        float p_near,
        float p_far,
        LightOccluderInstance* p_occluders,
        CameraMatrix* p_xform_cache
    ) override {}

    void reset_canvas() override {}

    void draw_window_margins(int* p_margins, RID* p_margin_textures) override {}

    RasterizerCanvasDummy() = default;

    ~RasterizerCanvasDummy() override = default;
};

class RasterizerDummy : public Rasterizer {
protected:
    RasterizerCanvasDummy canvas;
    RasterizerStorageDummy storage;
    RasterizerSceneDummy scene;

public:
    RasterizerStorage* get_storage() override {
        return &storage;
    }

    RasterizerCanvas* get_canvas() override {
        return &canvas;
    }

    RasterizerScene* get_scene() override {
        return &scene;
    }

    void set_boot_image(
        const Ref<Image>& p_image,
        const Color& p_color,
        bool p_scale,
        bool p_use_filter = true
    ) override {}

    void set_shader_time_scale(float p_scale) override {}

    void initialize() override {}

    void begin_frame(double frame_step) override {}

    void set_current_render_target(RID p_render_target) override {}

    void restore_render_target(bool p_3d_was_drawn) override {}

    void clear_render_target(const Color& p_color) override {}

    void blit_render_target_to_screen(
        RID p_render_target,
        const Rect2& p_screen_rect,
        int p_screen = 0
    ) override {}

    void output_lens_distorted_to_screen(
        RID p_render_target,
        const Rect2& p_screen_rect,
        float p_k1,
        float p_k2,
        const Vector2& p_eye_center,
        float p_oversample
    ) override {}

    void end_frame(bool p_swap_buffers) override {}

    void finalize() override {}

    static Error is_viable() {
        return OK;
    }

    static Rasterizer* _create_current() {
        return memnew(RasterizerDummy);
    }

    static void make_current() {
        _create_func = _create_current;
    }

    bool is_low_end() const override {
        return true;
    }

    RasterizerDummy() = default;

    ~RasterizerDummy() override = default;
};

#endif // RASTERIZER_DUMMY_H
