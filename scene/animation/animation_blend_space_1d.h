// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANIMATION_BLEND_SPACE_1D_H
#define ANIMATION_BLEND_SPACE_1D_H

#include "scene/animation/animation_tree.h"

class AnimationNodeBlendSpace1D : public AnimationRootNode {
    GDCLASS(AnimationNodeBlendSpace1D, AnimationRootNode);

    enum {
        MAX_BLEND_POINTS = 64
    };

    struct BlendPoint {
        StringName name;
        Ref<AnimationRootNode> node;
        float position;
    };

    BlendPoint blend_points[MAX_BLEND_POINTS];
    int blend_points_used;

    float max_space;
    float min_space;

    float snap;

    String value_label;

    void _add_blend_point(int p_index, const Ref<AnimationRootNode>& p_node);

    void _tree_changed();

    StringName blend_position;

protected:
    virtual void _validate_property(PropertyInfo& property) const;
    static void _bind_methods();

public:
    virtual void get_parameter_list(List<PropertyInfo>* r_list) const;
    virtual Variant get_parameter_default_value(const StringName& p_parameter
    ) const;

    virtual void get_child_nodes(List<ChildNode>* r_child_nodes);

    void add_blend_point(
        const Ref<AnimationRootNode>& p_node,
        float p_position,
        int p_at_index = -1
    );
    void set_blend_point_position(int p_point, float p_position);
    void set_blend_point_node(
        int p_point,
        const Ref<AnimationRootNode>& p_node
    );

    float get_blend_point_position(int p_point) const;
    Ref<AnimationRootNode> get_blend_point_node(int p_point) const;
    void remove_blend_point(int p_point);
    int get_blend_point_count() const;

    void set_min_space(float p_min);
    float get_min_space() const;

    void set_max_space(float p_max);
    float get_max_space() const;

    void set_snap(float p_snap);
    float get_snap() const;

    void set_value_label(const String& p_label);
    String get_value_label() const;

    float process(float p_time, bool p_seek);
    String get_caption() const;

    Ref<AnimationNode> get_child_by_name(const StringName& p_name);

    AnimationNodeBlendSpace1D();
    ~AnimationNodeBlendSpace1D();
};

#endif // ANIMATION_BLEND_SPACE_1D_H
