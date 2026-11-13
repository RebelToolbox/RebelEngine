// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef VISUAL_SHADER_NODES_H
#define VISUAL_SHADER_NODES_H

#include "scene/resources/visual_shader.h"

///////////////////////////////////////
/// CONSTANTS
///////////////////////////////////////

class VisualShaderNodeScalarConstant : public VisualShaderNode {
    GDCLASS(VisualShaderNodeScalarConstant, VisualShaderNode);
    float constant;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_constant(float p_value);
    float get_constant() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeScalarConstant();
};

///////////////////////////////////////

class VisualShaderNodeBooleanConstant : public VisualShaderNode {
    GDCLASS(VisualShaderNodeBooleanConstant, VisualShaderNode);
    bool constant;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_constant(bool p_value);
    bool get_constant() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeBooleanConstant();
};

///////////////////////////////////////

class VisualShaderNodeColorConstant : public VisualShaderNode {
    GDCLASS(VisualShaderNodeColorConstant, VisualShaderNode);
    Color constant;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_constant(Color p_value);
    Color get_constant() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeColorConstant();
};

///////////////////////////////////////

class VisualShaderNodeVec3Constant : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVec3Constant, VisualShaderNode);
    Vector3 constant;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_constant(Vector3 p_value);
    Vector3 get_constant() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeVec3Constant();
};

///////////////////////////////////////

class VisualShaderNodeTransformConstant : public VisualShaderNode {
    GDCLASS(VisualShaderNodeTransformConstant, VisualShaderNode);
    Transform constant;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_constant(Transform p_value);
    Transform get_constant() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeTransformConstant();
};

///////////////////////////////////////
/// TEXTURES
///////////////////////////////////////

class VisualShaderNodeTexture : public VisualShaderNode {
    GDCLASS(VisualShaderNodeTexture, VisualShaderNode);
    Ref<Texture> texture;

public:
    enum Source {
        SOURCE_TEXTURE,
        SOURCE_SCREEN,
        SOURCE_2D_TEXTURE,
        SOURCE_2D_NORMAL,
        SOURCE_DEPTH,
        SOURCE_PORT,
    };

    enum TextureType {
        TYPE_DATA,
        TYPE_COLOR,
        TYPE_NORMALMAP
    };

private:
    Source source;
    TextureType texture_type;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String get_input_port_default_hint(int p_port) const override;

    Vector<VisualShader::DefaultTextureParam> get_default_texture_parameters(
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_source(Source p_source);
    Source get_source() const;

    void set_texture(Ref<Texture> p_value);
    Ref<Texture> get_texture() const;

    void set_texture_type(TextureType p_type);
    TextureType get_texture_type() const;

    Vector<StringName> get_editable_properties() const override;

    String get_warning(
        Shader::Mode p_mode,
        VisualShader::Type p_type
    ) const override;

    VisualShaderNodeTexture();
};

VARIANT_ENUM_CAST(VisualShaderNodeTexture::TextureType)
VARIANT_ENUM_CAST(VisualShaderNodeTexture::Source)

///////////////////////////////////////

class VisualShaderNodeCubeMap : public VisualShaderNode {
    GDCLASS(VisualShaderNodeCubeMap, VisualShaderNode);
    Ref<CubeMap> cube_map;

public:
    enum Source {
        SOURCE_TEXTURE,
        SOURCE_PORT
    };

    enum TextureType {
        TYPE_DATA,
        TYPE_COLOR,
        TYPE_NORMALMAP
    };

private:
    Source source;
    TextureType texture_type;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;
    String get_input_port_default_hint(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    Vector<VisualShader::DefaultTextureParam> get_default_texture_parameters(
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_source(Source p_source);
    Source get_source() const;

    void set_cube_map(Ref<CubeMap> p_value);
    Ref<CubeMap> get_cube_map() const;

    void set_texture_type(TextureType p_type);
    TextureType get_texture_type() const;

    Vector<StringName> get_editable_properties() const override;
    String get_warning(
        Shader::Mode p_mode,
        VisualShader::Type p_type
    ) const override;

    VisualShaderNodeCubeMap();
};

VARIANT_ENUM_CAST(VisualShaderNodeCubeMap::TextureType)
VARIANT_ENUM_CAST(VisualShaderNodeCubeMap::Source)

///////////////////////////////////////
/// OPS
///////////////////////////////////////

class VisualShaderNodeScalarOp : public VisualShaderNode {
    GDCLASS(VisualShaderNodeScalarOp, VisualShaderNode);

public:
    enum Operator {
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
        OP_MOD,
        OP_POW,
        OP_MAX,
        OP_MIN,
        OP_ATAN2,
        OP_STEP
    };

protected:
    Operator op;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_operator(Operator p_op);
    Operator get_operator() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeScalarOp();
};

VARIANT_ENUM_CAST(VisualShaderNodeScalarOp::Operator)

class VisualShaderNodeVectorOp : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorOp, VisualShaderNode);

public:
    enum Operator {
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
        OP_MOD,
        OP_POW,
        OP_MAX,
        OP_MIN,
        OP_CROSS,
        OP_ATAN2,
        OP_REFLECT,
        OP_STEP
    };

protected:
    Operator op;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_operator(Operator p_op);
    Operator get_operator() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeVectorOp();
};

VARIANT_ENUM_CAST(VisualShaderNodeVectorOp::Operator)

///////////////////////////////////////

class VisualShaderNodeColorOp : public VisualShaderNode {
    GDCLASS(VisualShaderNodeColorOp, VisualShaderNode);

public:
    enum Operator {
        OP_SCREEN,
        OP_DIFFERENCE,
        OP_DARKEN,
        OP_LIGHTEN,
        OP_OVERLAY,
        OP_DODGE,
        OP_BURN,
        OP_SOFT_LIGHT,
        OP_HARD_LIGHT
    };

protected:
    Operator op;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_operator(Operator p_op);
    Operator get_operator() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeColorOp();
};

VARIANT_ENUM_CAST(VisualShaderNodeColorOp::Operator)

///////////////////////////////////////
/// TRANSFORM-TRANSFORM MULTIPLICATION
///////////////////////////////////////

class VisualShaderNodeTransformMult : public VisualShaderNode {
    GDCLASS(VisualShaderNodeTransformMult, VisualShaderNode);

public:
    enum Operator {
        OP_AxB,
        OP_BxA,
        OP_AxB_COMP,
        OP_BxA_COMP
    };

protected:
    Operator op;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_operator(Operator p_op);
    Operator get_operator() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeTransformMult();
};

VARIANT_ENUM_CAST(VisualShaderNodeTransformMult::Operator)

///////////////////////////////////////
/// TRANSFORM-VECTOR MULTIPLICATION
///////////////////////////////////////

class VisualShaderNodeTransformVecMult : public VisualShaderNode {
    GDCLASS(VisualShaderNodeTransformVecMult, VisualShaderNode);

public:
    enum Operator {
        OP_AxB,
        OP_BxA,
        OP_3x3_AxB,
        OP_3x3_BxA,
    };

protected:
    Operator op;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_operator(Operator p_op);
    Operator get_operator() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeTransformVecMult();
};

VARIANT_ENUM_CAST(VisualShaderNodeTransformVecMult::Operator)

///////////////////////////////////////
/// SCALAR FUNC
///////////////////////////////////////

class VisualShaderNodeScalarFunc : public VisualShaderNode {
    GDCLASS(VisualShaderNodeScalarFunc, VisualShaderNode);

public:
    enum Function {
        FUNC_SIN,
        FUNC_COS,
        FUNC_TAN,
        FUNC_ASIN,
        FUNC_ACOS,
        FUNC_ATAN,
        FUNC_SINH,
        FUNC_COSH,
        FUNC_TANH,
        FUNC_LOG,
        FUNC_EXP,
        FUNC_SQRT,
        FUNC_ABS,
        FUNC_SIGN,
        FUNC_FLOOR,
        FUNC_ROUND,
        FUNC_CEIL,
        FUNC_FRAC,
        FUNC_SATURATE,
        FUNC_NEGATE,
        FUNC_ACOSH,
        FUNC_ASINH,
        FUNC_ATANH,
        FUNC_DEGREES,
        FUNC_EXP2,
        FUNC_INVERSE_SQRT,
        FUNC_LOG2,
        FUNC_RADIANS,
        FUNC_RECIPROCAL,
        FUNC_ROUNDEVEN,
        FUNC_TRUNC,
        FUNC_ONEMINUS
    };

protected:
    Function func;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_function(Function p_func);
    Function get_function() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeScalarFunc();
};

VARIANT_ENUM_CAST(VisualShaderNodeScalarFunc::Function)

///////////////////////////////////////
/// VECTOR FUNC
///////////////////////////////////////

class VisualShaderNodeVectorFunc : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorFunc, VisualShaderNode);

public:
    enum Function {
        FUNC_NORMALIZE,
        FUNC_SATURATE,
        FUNC_NEGATE,
        FUNC_RECIPROCAL,
        FUNC_RGB2HSV,
        FUNC_HSV2RGB,
        FUNC_ABS,
        FUNC_ACOS,
        FUNC_ACOSH,
        FUNC_ASIN,
        FUNC_ASINH,
        FUNC_ATAN,
        FUNC_ATANH,
        FUNC_CEIL,
        FUNC_COS,
        FUNC_COSH,
        FUNC_DEGREES,
        FUNC_EXP,
        FUNC_EXP2,
        FUNC_FLOOR,
        FUNC_FRAC,
        FUNC_INVERSE_SQRT,
        FUNC_LOG,
        FUNC_LOG2,
        FUNC_RADIANS,
        FUNC_ROUND,
        FUNC_ROUNDEVEN,
        FUNC_SIGN,
        FUNC_SIN,
        FUNC_SINH,
        FUNC_SQRT,
        FUNC_TAN,
        FUNC_TANH,
        FUNC_TRUNC,
        FUNC_ONEMINUS
    };

protected:
    Function func;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_function(Function p_func);
    Function get_function() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeVectorFunc();
};

VARIANT_ENUM_CAST(VisualShaderNodeVectorFunc::Function)

///////////////////////////////////////
/// COLOR FUNC
///////////////////////////////////////

class VisualShaderNodeColorFunc : public VisualShaderNode {
    GDCLASS(VisualShaderNodeColorFunc, VisualShaderNode);

public:
    enum Function {
        FUNC_GRAYSCALE,
        FUNC_SEPIA
    };

protected:
    Function func;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_function(Function p_func);
    Function get_function() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeColorFunc();
};

VARIANT_ENUM_CAST(VisualShaderNodeColorFunc::Function)

///////////////////////////////////////
/// TRANSFORM FUNC
///////////////////////////////////////

class VisualShaderNodeTransformFunc : public VisualShaderNode {
    GDCLASS(VisualShaderNodeTransformFunc, VisualShaderNode);

public:
    enum Function {
        FUNC_INVERSE,
        FUNC_TRANSPOSE
    };

protected:
    Function func;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_function(Function p_func);
    Function get_function() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeTransformFunc();
};

VARIANT_ENUM_CAST(VisualShaderNodeTransformFunc::Function)

///////////////////////////////////////
/// DOT
///////////////////////////////////////

class VisualShaderNodeDotProduct : public VisualShaderNode {
    GDCLASS(VisualShaderNodeDotProduct, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeDotProduct();
};

///////////////////////////////////////
/// LENGTH
///////////////////////////////////////

class VisualShaderNodeVectorLen : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorLen, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorLen();
};

///////////////////////////////////////
/// DETERMINANT
///////////////////////////////////////

class VisualShaderNodeDeterminant : public VisualShaderNode {
    GDCLASS(VisualShaderNodeDeterminant, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeDeterminant();
};

///////////////////////////////////////
/// CLAMP
///////////////////////////////////////

class VisualShaderNodeScalarClamp : public VisualShaderNode {
    GDCLASS(VisualShaderNodeScalarClamp, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeScalarClamp();
};

///////////////////////////////////////

class VisualShaderNodeVectorClamp : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorClamp, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorClamp();
};

///////////////////////////////////////
/// DERIVATIVE FUNCTIONS
///////////////////////////////////////

class VisualShaderNodeScalarDerivativeFunc : public VisualShaderNode {
    GDCLASS(VisualShaderNodeScalarDerivativeFunc, VisualShaderNode);

public:
    enum Function {
        FUNC_SUM,
        FUNC_X,
        FUNC_Y
    };

protected:
    Function func;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_function(Function p_func);
    Function get_function() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeScalarDerivativeFunc();
};

VARIANT_ENUM_CAST(VisualShaderNodeScalarDerivativeFunc::Function)

///////////////////////////////////////

class VisualShaderNodeVectorDerivativeFunc : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorDerivativeFunc, VisualShaderNode);

public:
    enum Function {
        FUNC_SUM,
        FUNC_X,
        FUNC_Y
    };

protected:
    Function func;

    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_function(Function p_func);
    Function get_function() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeVectorDerivativeFunc();
};

VARIANT_ENUM_CAST(VisualShaderNodeVectorDerivativeFunc::Function)

///////////////////////////////////////
/// FACEFORWARD
///////////////////////////////////////

class VisualShaderNodeFaceForward : public VisualShaderNode {
    GDCLASS(VisualShaderNodeFaceForward, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeFaceForward();
};

///////////////////////////////////////
/// OUTER PRODUCT
///////////////////////////////////////

class VisualShaderNodeOuterProduct : public VisualShaderNode {
    GDCLASS(VisualShaderNodeOuterProduct, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeOuterProduct();
};

///////////////////////////////////////
/// STEP
///////////////////////////////////////

class VisualShaderNodeVectorScalarStep : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorScalarStep, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorScalarStep();
};

///////////////////////////////////////
/// SMOOTHSTEP
///////////////////////////////////////

class VisualShaderNodeScalarSmoothStep : public VisualShaderNode {
    GDCLASS(VisualShaderNodeScalarSmoothStep, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeScalarSmoothStep();
};

///////////////////////////////////////

class VisualShaderNodeVectorSmoothStep : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorSmoothStep, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorSmoothStep();
};

///////////////////////////////////////

class VisualShaderNodeVectorScalarSmoothStep : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorScalarSmoothStep, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorScalarSmoothStep();
};

///////////////////////////////////////
/// DISTANCE
///////////////////////////////////////

class VisualShaderNodeVectorDistance : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorDistance, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorDistance();
};

///////////////////////////////////////
/// REFRACT
///////////////////////////////////////

class VisualShaderNodeVectorRefract : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorRefract, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorRefract();
};

///////////////////////////////////////
/// MIX
///////////////////////////////////////

class VisualShaderNodeScalarInterp : public VisualShaderNode {
    GDCLASS(VisualShaderNodeScalarInterp, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeScalarInterp();
};

///////////////////////////////////////

class VisualShaderNodeVectorInterp : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorInterp, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorInterp();
};

///////////////////////////////////////

class VisualShaderNodeVectorScalarMix : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorScalarMix, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorScalarMix();
};

///////////////////////////////////////
/// COMPOSE
///////////////////////////////////////

class VisualShaderNodeVectorCompose : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorCompose, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorCompose();
};

///////////////////////////////////////

class VisualShaderNodeTransformCompose : public VisualShaderNode {
    GDCLASS(VisualShaderNodeTransformCompose, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeTransformCompose();
};

///////////////////////////////////////
/// DECOMPOSE
///////////////////////////////////////

class VisualShaderNodeVectorDecompose : public VisualShaderNode {
    GDCLASS(VisualShaderNodeVectorDecompose, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVectorDecompose();
};

///////////////////////////////////////

class VisualShaderNodeTransformDecompose : public VisualShaderNode {
    GDCLASS(VisualShaderNodeTransformDecompose, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeTransformDecompose();
};

///////////////////////////////////////
/// UNIFORMS
///////////////////////////////////////

class VisualShaderNodeScalarUniform : public VisualShaderNodeUniform {
    GDCLASS(VisualShaderNodeScalarUniform, VisualShaderNodeUniform);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeScalarUniform();
};

///////////////////////////////////////

class VisualShaderNodeBooleanUniform : public VisualShaderNodeUniform {
    GDCLASS(VisualShaderNodeBooleanUniform, VisualShaderNodeUniform);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeBooleanUniform();
};

///////////////////////////////////////

class VisualShaderNodeColorUniform : public VisualShaderNodeUniform {
    GDCLASS(VisualShaderNodeColorUniform, VisualShaderNodeUniform);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeColorUniform();
};

///////////////////////////////////////

class VisualShaderNodeVec3Uniform : public VisualShaderNodeUniform {
    GDCLASS(VisualShaderNodeVec3Uniform, VisualShaderNodeUniform);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeVec3Uniform();
};

///////////////////////////////////////

class VisualShaderNodeTransformUniform : public VisualShaderNodeUniform {
    GDCLASS(VisualShaderNodeTransformUniform, VisualShaderNodeUniform);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeTransformUniform();
};

///////////////////////////////////////

class VisualShaderNodeTextureUniform : public VisualShaderNodeUniform {
    GDCLASS(VisualShaderNodeTextureUniform, VisualShaderNodeUniform);

public:
    enum TextureType {
        TYPE_DATA,
        TYPE_COLOR,
        TYPE_NORMALMAP,
        TYPE_ANISO,
    };

    enum ColorDefault {
        COLOR_DEFAULT_WHITE,
        COLOR_DEFAULT_BLACK
    };

protected:
    TextureType texture_type;
    ColorDefault color_default;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;
    String get_input_port_default_hint(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    bool is_code_generated() const override;

    Vector<StringName> get_editable_properties() const override;

    void set_texture_type(TextureType p_type);
    TextureType get_texture_type() const;

    void set_color_default(ColorDefault p_default);
    ColorDefault get_color_default() const;

    VisualShaderNodeTextureUniform();
};

VARIANT_ENUM_CAST(VisualShaderNodeTextureUniform::TextureType)
VARIANT_ENUM_CAST(VisualShaderNodeTextureUniform::ColorDefault)

///////////////////////////////////////

class VisualShaderNodeTextureUniformTriplanar :
    public VisualShaderNodeTextureUniform {
    GDCLASS(
        VisualShaderNodeTextureUniformTriplanar,
        VisualShaderNodeTextureUniform
    );

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    String get_input_port_default_hint(int p_port) const override;

    String generate_global_per_node(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_global_per_func(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeTextureUniformTriplanar();
};

///////////////////////////////////////

class VisualShaderNodeCubeMapUniform : public VisualShaderNodeTextureUniform {
    GDCLASS(VisualShaderNodeCubeMapUniform, VisualShaderNodeTextureUniform);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String get_input_port_default_hint(int p_port) const override;
    String generate_global(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id
    ) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeCubeMapUniform();
};

///////////////////////////////////////
/// IF
///////////////////////////////////////

class VisualShaderNodeIf : public VisualShaderNode {
    GDCLASS(VisualShaderNodeIf, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeIf();
};

///////////////////////////////////////
/// SWITCH
///////////////////////////////////////

class VisualShaderNodeSwitch : public VisualShaderNode {
    GDCLASS(VisualShaderNodeSwitch, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeSwitch();
};

class VisualShaderNodeScalarSwitch : public VisualShaderNodeSwitch {
    GDCLASS(VisualShaderNodeScalarSwitch, VisualShaderNodeSwitch);

public:
    String get_caption() const override;

    PortType get_input_port_type(int p_port) const override;
    PortType get_output_port_type(int p_port) const override;

    VisualShaderNodeScalarSwitch();
};

///////////////////////////////////////
/// FRESNEL
///////////////////////////////////////

class VisualShaderNodeFresnel : public VisualShaderNode {
    GDCLASS(VisualShaderNodeFresnel, VisualShaderNode);

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String get_input_port_default_hint(int p_port) const override;
    bool is_generate_input_var(int p_port) const override;
    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    VisualShaderNodeFresnel();
};

///////////////////////////////////////
/// Is
///////////////////////////////////////

class VisualShaderNodeIs : public VisualShaderNode {
    GDCLASS(VisualShaderNodeIs, VisualShaderNode);

public:
    enum Function {
        FUNC_IS_INF,
        FUNC_IS_NAN,
    };

protected:
    Function func;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_function(Function p_func);
    Function get_function() const;

    Vector<StringName> get_editable_properties() const override;

    VisualShaderNodeIs();
};

VARIANT_ENUM_CAST(VisualShaderNodeIs::Function)

///////////////////////////////////////
/// Compare
///////////////////////////////////////

class VisualShaderNodeCompare : public VisualShaderNode {
    GDCLASS(VisualShaderNodeCompare, VisualShaderNode);

public:
    enum ComparisonType {
        CTYPE_SCALAR,
        CTYPE_VECTOR,
        CTYPE_BOOLEAN,
        CTYPE_TRANSFORM
    };

    enum Function {
        FUNC_EQUAL,
        FUNC_NOT_EQUAL,
        FUNC_GREATER_THAN,
        FUNC_GREATER_THAN_EQUAL,
        FUNC_LESS_THAN,
        FUNC_LESS_THAN_EQUAL,
    };

    enum Condition {
        COND_ALL,
        COND_ANY,
    };

protected:
    ComparisonType ctype;
    Function func;
    Condition condition;

protected:
    static void _bind_methods();

public:
    String get_caption() const override;

    int get_input_port_count() const override;
    PortType get_input_port_type(int p_port) const override;
    String get_input_port_name(int p_port) const override;

    int get_output_port_count() const override;
    PortType get_output_port_type(int p_port) const override;
    String get_output_port_name(int p_port) const override;

    String generate_code(
        Shader::Mode p_mode,
        VisualShader::Type p_type,
        int p_id,
        const String* p_input_vars,
        const String* p_output_vars,
        bool p_for_preview = false
    ) const override;

    void set_comparison_type(ComparisonType p_type);
    ComparisonType get_comparison_type() const;

    void set_function(Function p_func);
    Function get_function() const;

    void set_condition(Condition p_cond);
    Condition get_condition() const;

    Vector<StringName> get_editable_properties() const override;
    String get_warning(
        Shader::Mode p_mode,
        VisualShader::Type p_type
    ) const override;

    VisualShaderNodeCompare();
};

VARIANT_ENUM_CAST(VisualShaderNodeCompare::ComparisonType)
VARIANT_ENUM_CAST(VisualShaderNodeCompare::Function)
VARIANT_ENUM_CAST(VisualShaderNodeCompare::Condition)

#endif // VISUAL_SHADER_NODES_H
