// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "test_main.h"

#include "core/list.h"

#ifdef DEBUG_ENABLED

#include "test_astar.h"
#include "test_basis.h"
#include "test_crypto.h"
#include "test_gdscript.h"
#include "test_gui.h"
#include "test_math.h"
#include "test_oa_hash_map.h"
#include "test_ordered_hash_map.h"
#include "test_physics.h"
#include "test_physics_2d.h"
#include "test_render.h"
#include "test_shader_lang.h"
#include "test_string.h"
#include "test_transform.h"
#include "test_xml_parser.h"

const char** tests_get_names() {
    static const char* test_names[] = {
        "string",
        "math",
        "basis",
        "transform",
        "physics",
        "physics_2d",
        "render",
        "oa_hash_map",
        "gui",
        "shaderlang",
        "gd_tokenizer",
        "gd_parser",
        "gd_compiler",
        "gd_bytecode",
        "ordered_hash_map",
        "astar",
        "xml_parser",
        nullptr
    };

    return test_names;
}

MainLoop* test_main(String p_test, const List<String>& p_args) {
    if (p_test == "string") {
        return TestString::test();
    }

    if (p_test == "math") {
        return TestMath::test();
    }

    if (p_test == "basis") {
        return TestBasis::test();
    }

    if (p_test == "transform") {
        return TestTransform::test();
    }

    if (p_test == "physics") {
        return TestPhysics::test();
    }

    if (p_test == "physics_2d") {
        return TestPhysics2D::test();
    }

    if (p_test == "render") {
        return TestRender::test();
    }

    if (p_test == "oa_hash_map") {
        return TestOAHashMap::test();
    }

#ifndef _3D_DISABLED
    if (p_test == "gui") {
        return TestGUI::test();
    }
#endif

    if (p_test == "shaderlang") {
        return TestShaderLang::test();
    }

    if (p_test == "crypto") {
        return TestCrypto::test();
    }

    if (p_test == "gd_tokenizer") {
        return TestGDScript::test(TestGDScript::TEST_TOKENIZER);
    }

    if (p_test == "gd_parser") {
        return TestGDScript::test(TestGDScript::TEST_PARSER);
    }

    if (p_test == "gd_compiler") {
        return TestGDScript::test(TestGDScript::TEST_COMPILER);
    }

    if (p_test == "gd_bytecode") {
        return TestGDScript::test(TestGDScript::TEST_BYTECODE);
    }

    if (p_test == "ordered_hash_map") {
        return TestOrderedHashMap::test();
    }

    if (p_test == "astar") {
        return TestAStar::test();
    }

    if (p_test == "xml_parser") {
        return TestXMLParser::test();
    }

    print_line("Unknown test: " + p_test);
    return nullptr;
}

#else

const char** tests_get_names() {
    static const char* test_names[] = {NULL};

    return test_names;
}

MainLoop* test_main(String p_test, const List<String>& p_args) {
    return NULL;
}

#endif
