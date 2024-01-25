#!/usr/bin/env python3

import argparse
import os
import textwrap
import shutil
import xml.etree.ElementTree as ET
from collections import OrderedDict

HEADER = """\
# SPDX-FileCopyrightText: 2023 Rebel Engine contributors
# SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
# SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
#
# SPDX-License-Identifier: MIT
#
# LANGUAGE translation of the Rebel Engine class reference
#
# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.
#
#, fuzzy
msgid ""
msgstr ""
"Project-Id-Version: Rebel Engine class reference\\n"
"Report-Msgid-Bugs-To: https://github.com/RebelToolbox/RebelEngine\\n"
"MIME-Version: 1.0\\n"
"Content-Type: text/plain; charset=UTF-8\\n"
"Content-Transfer-Encoding: 8-bit\\n"
"""

EXTRACT_TAGS = [
    "description",
    "brief_description",
    "member",
    "constant",
    "theme_item",
    "link",
]

# Additional heading strings added by rst_from_xml.py.
HEADING_STRINGS = [
    "Description",
    "Tutorials",
    "Properties",
    "Methods",
    "Theme Properties",
    "Signals",
    "Enumerations",
    "Constants",
    "Property Descriptions",
    "Method Descriptions",
]


class XMLData:
    def __init__(self, xml_root, class_name, xml_filepath):
        self.xml_root = xml_root
        self.class_name = class_name
        self.xml_filepath = xml_filepath


def get_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--path",
        "-p",
        nargs="+",
        default=".",
        help="The directory or directories containing API XML files.",
    )
    parser.add_argument(
        "--output",
        "-o",
        default="api.pot",
        help="The translation template .pot file.",
    )
    return parser.parse_args()


def print_error(error):
    print("ERROR: {}".format(error))


def escape_special_characters(text):
    escaped_text = ""
    for character in text:
        if character == "\n":
            escaped_text += "\\n"
        elif character == '"':
            escaped_text += '\\"'
        elif character == "\\":
            escaped_text += "\\\\"
        else:
            escaped_text += character
    return escaped_text


def parse_xml_files_in_directory(directory):
    print("Parsing files in: {}".format(directory))
    classes_xml_data = OrderedDict()
    for filepath in map(
        lambda filename: os.path.join(directory, filename), os.listdir(directory)
    ):
        if not os.path.isfile(filepath) or not filepath.endswith(".xml"):
            continue
        xml_data = parse_xml_file(filepath)
        class_name = xml_data.class_name
        if class_name in classes_xml_data:
            print_error(
                "Duplicate class {} in XML file {}".format(class_name, filepath)
            )
            exit(1)
        classes_xml_data[class_name] = xml_data
    return list(classes_xml_data.values())


def parse_xml_file(xml_filepath):
    print("Parsing file: {}".format(os.path.basename(xml_filepath)))
    try:
        tree = ET.parse(xml_filepath)
    except ET.ParseError as e:
        print_error("Failed to parse '{}': {}".format(xml_filepath, e))
        exit(1)
    xml_root = tree.getroot()
    if "name" not in xml_root.attrib:
        print_error("Skipping unknown XML file: {}".format(xml_filepath))
        return
    if "version" not in xml_root.attrib:
        print_error("Version missing from file: {}".format(xml_filepath))
        return
    class_name = xml_root.attrib["name"]
    xml_filepath = xml_filepath.replace("\\", "/")
    if xml_filepath.startswith("./"):
        xml_filepath = xml_filepath[2:]
    return XMLData(xml_root, class_name, xml_filepath)


def extract_unique_strings(xml_data_list):
    unique_strings_filepaths = OrderedDict()
    for xml_data in xml_data_list:
        for elem in xml_data.xml_root.iter():
            if elem.tag not in EXTRACT_TAGS or not elem.text:
                continue
            text = textwrap.dedent(elem.text).strip()
            string = escape_special_characters(text)
            xml_filepath = xml_data.xml_filepath
            if string not in unique_strings_filepaths:
                unique_strings_filepaths[string] = [xml_filepath]
            else:
                unique_strings_filepaths[string].append(xml_filepath)
    return unique_strings_filepaths


def create_translation_template(unique_strings_filepaths, output):
    with open(output, "w", encoding="utf8") as f:
        f.write(HEADER)
        for string in HEADING_STRINGS:
            f.write("#: tools/scripts/rst_from_xml.py\n")
            f.write('msgid "{}"\n'.format(string))
            f.write('msgstr ""\n\n')
        for string in unique_strings_filepaths:
            if not string or string in HEADING_STRINGS:
                continue
            f.write("#:")
            xml_filepaths = unique_strings_filepaths[string]
            for xml_filepath in xml_filepaths:
                f.write(" {}".format(xml_filepath))
            f.write("\n")
            f.write('msgid "{}"\n'.format(string))
            f.write('msgstr ""\n\n')


def wrap_strings(output):
    print("Wrapping strings at 79 characters for compatibility with Weblate.")
    os.system("msgmerge -w79 {0} {0} > {0}.wrap".format(output))
    shutil.move("{}.wrap".format(output), output)


def main():
    if not shutil.which("msgmerge"):
        print_error("'msgmerge' is required, but it's not installed.")
        exit(1)
    arguments = get_arguments()
    output = os.path.abspath(arguments.output)
    if not os.path.isdir(os.path.dirname(output)) or not output.endswith(".pot"):
        print_error("Invalid translation template .pot file: {}".format(output))
        exit(1)
    xml_data_list = []
    for directory in arguments.path:
        if not os.path.isdir(directory):
            print_error("Invalid directory: {}".format(directory))
            exit(1)
        xml_data_list += parse_xml_files_in_directory(directory)
    xml_data_list = sorted(xml_data_list, key=lambda data: data.class_name.lower())
    unique_strings_filepaths = extract_unique_strings(xml_data_list)
    create_translation_template(unique_strings_filepaths, output)
    wrap_strings(output)


if __name__ == "__main__":
    main()
