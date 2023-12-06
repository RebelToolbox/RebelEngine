// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "test_xml_parser.h"

#include "core/os/os.h"

namespace TestXMLParser
{
#define CHECK(X)                                                               \
    if (!(X)) {                                                                \
        OS::get_singleton()->print("\tFAIL at %s\n", #X);                      \
        return false;                                                          \
    } else {                                                                   \
        OS::get_singleton()->print("\tPASS\n");                                \
    }

bool test_1() {
    String source =
        "<?xml version = \"1.0\" encoding=\"UTF-8\" ?>\
<top attr=\"attr value\">\
  Text&lt;&#65;&#x42;&gt;\
</top>";
    Vector<uint8_t> buff;
    for (int i = 0; i < source.length(); i++) {
        buff.push_back(source.get(i));
    }
    XMLParser parser;
    parser.open_buffer(buff);

    // <?xml ...?> gets parsed as NODE_UNKNOWN
    CHECK(parser.read() == OK);
    CHECK(parser.get_node_type() == XMLParser::NodeType::NODE_UNKNOWN);

    CHECK(parser.read() == OK);
    CHECK(parser.get_node_type() == XMLParser::NodeType::NODE_ELEMENT);
    CHECK(parser.get_node_name() == "top");
    CHECK(parser.has_attribute("attr"));
    CHECK(parser.get_attribute_value("attr") == "attr value");

    CHECK(parser.read() == OK);
    CHECK(parser.get_node_type() == XMLParser::NodeType::NODE_TEXT);
    CHECK(parser.get_node_data().lstrip(" \t") == "Text<AB>");

    CHECK(parser.read() == OK);
    CHECK(parser.get_node_type() == XMLParser::NodeType::NODE_ELEMENT_END);
    CHECK(parser.get_node_name() == "top");

    parser.close();
    return true;
}

typedef bool (*TestFunc)();
TestFunc test_funcs[] = {test_1, nullptr};

MainLoop* test() {
    int count  = 0;
    int passed = 0;

    while (true) {
        if (!test_funcs[count]) {
            break;
        }
        bool pass = test_funcs[count]();
        if (pass) {
            passed++;
        }
        OS::get_singleton()->print("\t%s\n", pass ? "PASS" : "FAILED");

        count++;
    }

    OS::get_singleton()->print("\n\n\n");
    OS::get_singleton()->print("*************\n");
    OS::get_singleton()->print("***TOTALS!***\n");
    OS::get_singleton()->print("*************\n");

    OS::get_singleton()->print("Passed %i of %i tests\n", passed, count);

    return nullptr;
}
} // namespace TestXMLParser
