#!/usr/bin/env python3

# Used by RebelDocumentation
# Creates RST API documenation files from XML API documentation files.

import argparse
import os
import re
import textwrap
import xml.etree.ElementTree as ET
from collections import OrderedDict


class_defs = OrderedDict()
current_file = ""
current_class = ""
error = False


def main():
    global class_defs
    global current_class
    arguments = get_arguments()
    files = get_files(arguments.path)
    class_defs = parse_xml_files(files)
    if not os.path.isdir(arguments.output):
        os.makedirs(arguments.output)
    for class_def in class_defs.values():
        current_class = class_def.name
        filename = os.path.join(
            arguments.output, "class_" + class_def.name.lower() + ".rst"
        )
        if arguments.dry_run:
            filename = os.devnull
        rst_file = open(filename, "w", encoding="utf-8")
        write_rst_file(rst_file, class_def)
    if error:
        print("Errors were found in the XML API Documentation.")
        print("Please fix the errors and try again.")
        exit(1)
    print("No errors found.")
    if not arguments.dry_run:
        print("Wrote reStructuredText files for each class to: %s" % arguments.output)


def parse_error(message):
    global error
    error = True
    print("XML ERROR: {1}): {0}".format(message, current_file))


def rst_error(message):
    global error
    error = True
    print("RST ERROR: {1}: {0}".format(message, current_class))


def get_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "path",
        nargs="+",
        help="A path to an XML file or a directory containing XML files to parse.",
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "--output",
        "-o",
        default=".",
        help="The output directory to save the .rst files in.",
    )
    group.add_argument(
        "--dry-run",
        action="store_true",
        help="Only check XML files for errors, do not create .rst files.",
    )
    return parser.parse_args()


def get_files(paths):
    files = []
    for path in paths:
        path = os.path.normpath(path)
        if os.path.isdir(path):
            files += get_files_in_directory(path)
        elif os.path.isfile(path) and path.endswith(".xml"):
            files.append(path)
        else:
            print("File or directory not found: {}".format(path))
    return files


def get_files_in_directory(directory):
    files = []
    if os.path.basename(directory) == "modules":
        # Special case: Look for docs folders in each module
        return get_files_in_modules(directory)
    for entry in os.scandir(directory):
        if os.path.isfile(entry.path) and entry.name.endswith(".xml"):
            files.append(entry.path)
    return files


def get_files_in_modules(modules_path):
    files = []
    for modules_entry in os.scandir(modules_path):
        if not modules_entry.is_dir():
            continue
        for module_entry in os.scandir(modules_entry.path):
            if module_entry.is_dir() and module_entry.name == "docs":
                files += get_files_in_directory(module_entry.path)
    return files


def parse_xml_files(files):
    global current_file
    class_defs = OrderedDict()
    for file in files:
        current_file = file
        try:
            tree = ET.parse(file)
        except ET.ParseError as e:
            parse_error(e)
            exit(1)
        xml_root = tree.getroot()
        if "name" not in xml_root.attrib:
            parse_error("Name is not a root attribute")
            continue
        if "version" not in xml_root.attrib:
            parse_error("Version missing")
            continue
        class_name = xml_root.get("name")
        if class_name in class_defs:
            parse_error("Duplicate class '{}'".format(class_name))
            continue
        class_def = parse_class(xml_root)
        class_defs[class_def.name] = class_def
    class_defs = OrderedDict(sorted(class_defs.items(), key=lambda t: t[0]))
    return class_defs


def parse_class(xml_root):
    class_name = xml_root.get("name")
    class_def = ClassDef(class_name)
    inherits = xml_root.get("inherits")
    if inherits is not None:
        class_def.inherits = inherits
    brief_description = xml_root.find("brief_description")
    if brief_description is not None and brief_description.text:
        class_def.brief_description = brief_description.text
    description = xml_root.find("description")
    if description is not None and description.text:
        class_def.description = description.text
    parse_tutorials(xml_root.find("tutorials"), class_def)
    parse_methods(xml_root.find("methods"), class_def)
    parse_properties(xml_root.find("members"), class_def)
    parse_signals(xml_root.find("signals"), class_def)
    parse_constants(xml_root.find("constants"), class_def)
    parse_theme_items(xml_root.find("theme_items"), class_def)
    return class_def


def parse_tutorials(tutorials, class_def):
    if tutorials is None:
        return
    for link in tutorials:
        assert link.tag == "link"
        if link.text is not None:
            class_def.tutorials.append((link.text.strip(), link.get("title", "")))


def parse_methods(methods, class_def):
    if methods is None:
        return
    for method in methods:
        assert method.tag == "method"
        name = method.get("name")
        qualifiers = method.get("qualifiers")
        return_type = TypeDef(method.find("return"))
        parameters = get_parameters(method)
        description_element = method.find("description")
        description = None
        if description_element is not None:
            description = description_element.text
        method_def = MethodDef(name, qualifiers, return_type, parameters, description)
        if name not in class_def.methods:
            class_def.methods[name] = []
        class_def.methods[name].append(method_def)


def parse_properties(properties, class_def):
    if properties is None:
        return
    for property in properties:
        assert property.tag == "member"
        name = property.get("name")
        if name in class_def.properties:
            parse_error("Duplicate member '{}'".format(name))
            continue
        type_def = TypeDef(property)
        setter = property.get("setter")
        getter = property.get("getter")
        default = property.get("default")
        overridden = property.get("override") or False
        property_def = PropertyDef(
            name, type_def, setter, getter, default, overridden, property.text
        )
        class_def.properties[name] = property_def


def parse_signals(signals, class_def):
    if signals is None:
        return
    for signal in signals:
        assert signal.tag == "signal"
        name = signal.get("name")
        if name in class_def.signals:
            parse_error("Duplicate signal '{}'".format(name))
            continue
        parameters = get_parameters(signal)
        description_element = signal.find("description")
        description = None
        if description_element is not None:
            description = description_element.text
        signal_def = SignalDef(name, parameters, description)
        class_def.signals[name] = signal_def


def parse_constants(constants, class_def):
    if constants is None:
        return
    for constant in constants:
        assert constant.tag == "constant"
        name = constant.get("name")
        value = constant.get("value")
        enum = constant.get("enum")
        constant_def = ConstantDef(name, value, constant.text)
        if enum:
            if enum in class_def.enums:
                enum_def = class_def.enums[enum]
            else:
                enum_def = EnumDef(enum)
                class_def.enums[enum] = enum_def
            enum_def.values[name] = constant_def
        elif name in class_def.constants:
            parse_error("Duplicate constant '{}'".format(name))
            continue
        else:
            class_def.constants[name] = constant_def


def parse_theme_items(theme_items, class_def):
    if theme_items is None:
        return
    for theme_item in theme_items:
        assert theme_item.tag == "theme_item"
        name = theme_item.get("name")
        if name in class_def.theme_items:
            parse_error("Duplicate theme item '{}'".format(name))
            continue
        data_type = theme_item.get("data_type")
        type_def = TypeDef(theme_item)
        default = theme_item.get("default")
        theme_item_def = ThemeItemDef(
            name, data_type, type_def, default, theme_item.text
        )
        class_def.theme_items[name] = theme_item_def


def get_parameters(element):
    parameter_elements = element.findall("argument")
    parameters = [None] * len(parameter_elements)
    for parameter_element in parameter_elements:
        index = int(parameter_element.get("index"))
        name = parameter_element.get("name")
        type_def = TypeDef(parameter_element)
        default = parameter_element.get("default")
        parameters[index] = ParameterDef(name, type_def, default)
    return parameters


def write_rst_file(rst_file, class_def):
    write_header(rst_file, class_def)
    write_inherits_classes(rst_file, class_def)
    write_inherited_by_classes(rst_file, class_def)
    write_brief_description(rst_file, class_def)
    write_description(rst_file, class_def)
    write_tutorials(rst_file, class_def)
    write_properties_list(rst_file, class_def)
    write_methods_list(rst_file, class_def)
    write_theme_properties_list(rst_file, class_def)
    write_signals(rst_file, class_def)
    write_enums(rst_file, class_def)
    write_contants(rst_file, class_def)
    write_property_descriptions(rst_file, class_def)
    write_method_descriptions(rst_file, class_def)
    write_theme_property_descriptions(rst_file, class_def)
    write_footer(rst_file, class_def)


def write_header(rst_file, class_def):
    class_name = class_def.name
    # Warn contributors not to edit this file directly.
    rst_file.write(":github_url: hide\n\n")
    rst_file.write(
        ".. Generated automatically by RebelEngine/tools/scripts/rst_from_xml.py"
    )
    rst_file.write(
        ".. DO NOT EDIT THIS FILE, but the {}.xml source instead.\n".format(class_name)
    )
    rst_file.write(".. The source is found in docs or modules/<name>/docs.")
    rst_file.write("\n\n")
    rst_file.write(".. _class_" + class_name + ":")
    rst_file.write("\n\n")
    rst_file.write(rst_heading(class_name, "="))


def write_inherits_classes(rst_file, class_def):
    if not class_def.inherits:
        return
    rst_file.write("**Inherits:** ")
    inherits = class_def.inherits
    first = True
    while inherits in class_defs:
        if first:
            first = False
        else:
            rst_file.write(" **<** ")
        rst_file.write(rst_class_reference_link(inherits))
        inherits = class_defs[inherits].inherits
    rst_file.write("\n\n")


def write_inherited_by_classes(rst_file, class_def):
    class_name = class_def.name
    inherited_by = []
    for class_def in class_defs.values():
        if class_def.inherits and class_def.inherits == class_name:
            inherited_by.append(class_def.name)
    if not inherited_by:
        return
    rst_file.write("**Inherited By:** ")
    first = True
    for child in inherited_by:
        if first:
            first = False
        else:
            rst_file.write(", ")
        rst_file.write(rst_class_reference_link(child))
    rst_file.write("\n\n")


def write_brief_description(rst_file, class_def):
    if class_def.brief_description is None:
        return
    rst_file.write(rst_text(class_def.brief_description))
    rst_file.write("\n\n")


def write_description(rst_file, class_def):
    if class_def.description.strip() == "":
        return
    rst_file.write(rst_heading("Description", "-"))
    rst_file.write(rst_text(class_def.description))
    rst_file.write("\n\n")


def write_tutorials(rst_file, class_def):
    if not class_def.tutorials:
        return
    rst_file.write(rst_heading("Tutorials", "-"))
    for url, title in class_def.tutorials:
        rst_file.write("- " + rst_link_label(url, title))
        rst_file.write("\n\n")


def write_properties_list(rst_file, class_def):
    if not class_def.properties:
        return
    rst_file.write(rst_heading("Properties", "-"))
    property_data = []
    for property_def in class_def.properties.values():
        type_rst = rst_type(property_def.type_def)
        default = property_def.default
        if default is not None:
            default = rst_inline_code(default)
        if default is not None and property_def.overridden:
            property_data.append(
                (type_rst, property_def.name, "{} *(parent override)*".format(default))
            )
        else:
            reference = ":ref:`{0}<class_{1}_property_{0}>`".format(
                property_def.name, class_def.name
            )
            property_data.append([type_rst, reference, default])
    write_table(rst_file, property_data, True)


def write_methods_list(rst_file, class_def):
    if not class_def.methods:
        return
    rst_file.write(rst_heading("Methods", "-"))
    method_data = []
    for methods in class_def.methods.values():
        for method_def in methods:
            method_name = rst_reference_link("method", method_def.name)
            method_parameters = rst_method_parameters(method_def)
            method_signature = [
                rst_type(method_def.return_type),
                method_name + method_parameters,
            ]
            method_data.append(method_signature)
    write_table(rst_file, method_data)


def write_theme_properties_list(rst_file, class_def):
    if not class_def.theme_items:
        return
    rst_file.write(rst_heading("Theme Properties", "-"))
    theme_properties_data = []
    for theme_item_def in class_def.theme_items.values():
        default = theme_item_def.default
        if default is not None:
            default = rst_inline_code(default)
        reference = ":ref:`{1}<class_{0}_theme_{2}_{1}>`".format(
            class_def.name, theme_item_def.name, theme_item_def.data_type
        )
        theme_properties_data.append(
            [rst_type(theme_item_def.type_def), reference, default]
        )
    write_table(rst_file, theme_properties_data, True)


def write_signals(rst_file, class_def):
    if not class_def.signals:
        return
    rst_file.write(rst_heading("Signals", "-"))
    first = True
    for signal in class_def.signals.values():
        if first:
            first = False
        else:
            rst_file.write(rst_line_separator())
        rst_file.write(
            ".. _class_{}_signal_{}:\n\n".format(class_def.name, signal.name)
        )
        rst_file.write("- {}\n\n".format(rst_signal(signal)))
        if signal.description.strip():
            rst_file.write(rst_text(signal.description))
            rst_file.write("\n\n")


def write_enums(rst_file, class_def):
    if not class_def.enums:
        return
    rst_file.write(rst_heading("Enumerations", "-"))
    first = True
    for enum in class_def.enums.values():
        if first:
            first = False
        else:
            rst_file.write(rst_line_separator())
        rst_file.write(".. _enum_{}_{}:\n\n".format(class_def.name, enum.name))
        # Sphinx seems to divide the bullet list into individual <ul> tags
        # if we weave the labels into it. As such I'll put them all above the list.
        # Won't be perfect but better than making the list visually broken.
        # As to why I'm not modifying the reference parser to directly link to the _enum label:
        # If somebody gets annoyed enough to fix it, all existing references will magically improve.
        for value in enum.values.values():
            rst_file.write(
                ".. _class_{}_constant_{}:\n\n".format(class_def.name, value.name)
            )
        rst_file.write("enum **{}**:\n\n".format(enum.name))
        for value in enum.values.values():
            rst_file.write("- **{}** = **{}**".format(value.name, value.value))
            if value.text.strip():
                rst_file.write(" --- " + rst_text(value.text))
            rst_file.write("\n\n")


def write_contants(rst_file, class_def):
    if not class_def.constants:
        return
    rst_file.write(rst_heading("Constants", "-"))
    # Place all constant labels above the list to prevent the list being visually broken,
    # because Sphinx appears to divide the bullet list into individual <ul> tags
    # if we weave the labels into it.
    # TODO: Move label to each constant.
    for constant in class_def.constants.values():
        rst_file.write(
            ".. _class_{}_constant_{}:\n\n".format(class_def.name, constant.name)
        )
    for constant in class_def.constants.values():
        rst_file.write("- **{}** = **{}**".format(constant.name, constant.value))
        if constant.text.strip():
            rst_file.write(" --- " + rst_text(constant.text))
        rst_file.write("\n\n")


def write_property_descriptions(rst_file, class_def):
    if all(p.overridden for p in class_def.properties.values()):
        return
    rst_file.write(rst_heading("Property Descriptions", "-"))
    first = True
    for property_def in class_def.properties.values():
        if property_def.overridden:
            continue
        if first:
            first = False
        else:
            rst_file.write(rst_line_separator())
        rst_file.write(
            ".. _class_{}_property_{}:\n\n".format(class_def.name, property_def.name)
        )
        rst_file.write(
            "- {} **{}**\n\n".format(rst_type(property_def.type_def), property_def.name)
        )
        write_property_data(rst_file, property_def)
        if property_def.text.strip():
            rst_file.write(rst_text(property_def.text))
            rst_file.write("\n\n")


def write_method_descriptions(rst_file, class_def):
    if not class_def.methods:
        return
    rst_file.write(rst_heading("Method Descriptions", "-"))
    first_name = True
    for methods in class_def.methods.values():
        first_method = True
        for method_def in methods:
            if first_name:
                first_name = False
            else:
                rst_file.write(rst_line_separator())
            if first_method:
                first_method = False
                rst_file.write(
                    ".. _class_{}_method_{}:\n\n".format(
                        class_def.name, method_def.name
                    )
                )
            rst_file.write(
                "- {} {}\n\n".format(
                    rst_type(method_def.return_type),
                    rst_bold(method_def.name) + rst_method_parameters(method_def),
                )
            )
            if method_def.description.strip():
                rst_file.write(rst_text(method_def.description))
                rst_file.write("\n\n")


def write_theme_property_descriptions(rst_file, class_def):
    if not class_def.theme_items:
        return
    rst_file.write(rst_heading("Theme Property Descriptions", "-"))
    first = True
    for theme_item_def in class_def.theme_items.values():
        if first:
            first = False
        else:
            rst_file.write(rst_line_separator())
        rst_file.write(
            ".. _class_{}_theme_{}_{}:\n\n".format(
                class_def.name, theme_item_def.data_type, theme_item_def.name
            )
        )
        rst_file.write(
            "- {} **{}**\n\n".format(
                rst_type(theme_item_def.type_def), theme_item_def.name
            )
        )
        theme_item_data = []
        if theme_item_def.default is not None:
            theme_item_data.append(
                ("*Default*", rst_inline_code(theme_item_def.default))
            )
        if theme_item_data:
            write_table(rst_file, theme_item_data)
        if theme_item_def.text.strip():
            rst_file.write(rst_text(theme_item_def.text) + "\n\n")


def write_footer(rst_file, class_def):
    # Abbreviations are used to display tooltips on hover.
    # Substitutions are used to add abbreviations.
    # Method qualifiers.
    rst_file.write(
        ".. |virtual| replace:: :abbr:`virtual (This method should typically be overridden by the user to have any effect.)`\n"
    )
    rst_file.write(
        ".. |const| replace:: :abbr:`const (This method has no side effects. It doesn't modify any of the instance's member variables.)`\n"
    )
    rst_file.write(
        ".. |vararg| replace:: :abbr:`vararg (This method accepts any number of arguments after the ones described here.)`\n"
    )


def write_property_data(rst_file, property_def):
    property_data = []
    if property_def.default:
        property_data.append(("*Default*", rst_inline_code(property_def.default)))
    if property_def.setter and not property_def.setter.startswith("_"):
        property_data.append(("*Setter*", property_def.setter + "(value)"))
    if property_def.getter and not property_def.getter.startswith("_"):
        property_data.append(("*Getter*", property_def.getter + "()"))
    if property_data:
        write_table(rst_file, property_data)


def write_table(rst_file, data, remove_empty_columns=False):
    if not data:
        return
    column_widths = calculate_column_widths(data)
    row_separator = get_row_separator(column_widths, remove_empty_columns)
    rst_file.write(row_separator)
    for row in data:
        row_text = "|"
        for column, text in enumerate(row):
            if column_widths[column] == 0 and remove_empty_columns:
                continue
            row_text += " " + (text or "").ljust(column_widths[column]) + " |"
        row_text += "\n"
        rst_file.write(row_text)
        rst_file.write(row_separator)
    rst_file.write("\n")


def calculate_column_widths(data):
    column_widths = [0] * len(data[0])
    for row in data:
        for column, text in enumerate(row):
            text_length = len(text or "")
            if text_length > column_widths[column]:
                column_widths[column] = text_length
    return column_widths


def get_row_separator(column_widths, remove_empty_columns):
    row_separator = ""
    for column_width in column_widths:
        if remove_empty_columns and column_width == 0:
            continue
        row_separator += "+" + "-" * (column_width + 2)
    row_separator += "+\n"
    return row_separator


def rst_heading(heading, underline):
    return heading + "\n" + (underline * len(heading)) + "\n\n"


def rst_line_separator():
    return "----\n\n"


def rst_bold(text):
    return "**{}**".format(text)


def rst_italics(text):
    return "*{}*".format(text)


def rst_inline_code(code):
    return "``{}``".format(code)


def rst_class_reference_link(class_name):
    if class_name not in class_defs:
        rst_error("Unknown class '{}'".format(class_name))
    return ":ref:`{0}<class_{0}>`".format(class_name)


def rst_reference_link(component, name):
    name_components = name.split(".")
    if len(name_components) == 1:
        class_name = current_class
        component_name = name
    elif len(name_components) == 2:
        class_name, component_name = name_components
    else:
        rst_error("Bad reference: '{}'".format(name))
        return name
    class_name = find_reference_link(component, class_name, component_name)
    if component == "member":
        component = "property"
    if class_name == current_class:
        return ":ref:`{2}<class_{1}_{0}_{2}>`".format(
            component, class_name, component_name
        )
    else:
        return ":ref:`{1}.{2}<class_{1}_{0}_{2}>`".format(
            component, class_name, component_name
        )


def find_reference_link(component, class_name, component_name):
    if class_name not in class_defs:
        rst_error("Unknown class '{}'".format(class_name))
        return class_name
    class_def = class_defs[class_name]
    found = False
    match component:
        case "constant":
            found = search_constants(class_def, component_name)
            if not found:
                class_def = class_defs["@GlobalScope"]
                found = search_constants(class_def, component_name)
        case "method":
            found = component_name in class_def.methods
        case "member":
            found = component_name in class_def.properties
        case "signal":
            found = component_name in class_def.signals
        case _:
            rst_error("Unrecognised component: {}".format(component))
    if not found:
        rst_error("Unresolved {} '{}.{}'".format(component, class_name, component_name))
        return class_name
    return class_def.name


def search_constants(class_def, constant_name):
    if constant_name in class_def.constants:
        return True
    else:
        for enum in class_def.enums.values():
            if constant_name in enum.values:
                return True
    return False


def rst_method_parameters(method_def):
    vararg = method_def.qualifiers is not None and "vararg" in method_def.qualifiers
    parameters = rst_parameters(method_def.parameters, vararg)
    qualifiers = rst_qualifiers(method_def.qualifiers)
    return " {}{}".format(parameters, qualifiers)


def rst_qualifiers(qualifiers):
    if qualifiers is None:
        return ""
    qualifier_text = ""
    # Abbreviations are used to display tooltips on hover.
    # Substitutions are used to add abbreviations.
    # See `write_footer()` for method qualifier descriptions.
    for qualifier in qualifiers.split():
        qualifier_text += " |" + qualifier + "|"
    return qualifier_text


def rst_signal(signal_def):
    return "**{}** {}".format(signal_def.name, rst_parameters(signal_def.parameters))


def rst_parameters(parameters, vararg=False):
    parameter_string = ""
    first = True
    for parameter in parameters:
        if first:
            first = False
            parameter_string += " "
        else:
            parameter_string += ", "
        parameter_string += rst_type(parameter.type_def)
        parameter_string += " "
        parameter_string += parameter.name
        if parameter.default is not None:
            parameter_string += "={}".format(parameter.default)
    if vararg:
        if parameters:
            parameter_string += ", ..."
        else:
            parameter_string += " ..."
    parameter_string += " "
    return "**(**" + parameter_string + "**)**"


def rst_link_label(url, title):
    # http(s)://docs.rebeltoolbox.com/<langcode>/<tag>/path/to/page.html(#fragment-tag)
    docs_rebeltoolbox_com = re.compile(
        r"^http(?:s)?://docs\.rebeltoolbox\.com/(?:[a-zA-Z0-9.\-_]*)/(?:[a-zA-Z0-9.\-_]*)/(.*)\.html(#.*)?$"
    )
    docs_match = docs_rebeltoolbox_com.search(url)
    if docs_match:
        return rst_internal_link(docs_match)
    else:
        return rst_external_link(url, title)


def rst_internal_link(docs_match):
    doc = docs_match.group(1)
    tag = docs_match.group(2)
    if tag is not None:
        return "`{1} <../{0}.html{1}>`_ in :doc:`../{0}`".format(doc, tag)
    else:
        return ":doc:`../{}`".format(doc)


def rst_external_link(url, title):
    if title:
        return "`" + title + " <" + url + ">`__"
    else:
        return "`" + url + " <" + url + ">`__"


def rst_type(type_def):
    if type_def.enum is not None:
        return rst_enum(type_def.enum)
    elif type_def.type == "void":
        return "void"
    else:
        return rst_class_reference_link(type_def.type)


def rst_text(text):
    text = textwrap.dedent(text).strip()
    text = add_line_breaks(text)
    result = ""
    remaining_text = text
    while remaining_text:
        (plain_text, tag, remaining_text) = extract_next_tag(remaining_text)
        result += escape_special_characters(plain_text)
        result += rst_tag(tag)
        if (
            result.endswith("`")
            and remaining_text
            and (remaining_text[0].isalnum() or remaining_text[0] == "(")
        ):
            result += "\ "
    return result


def add_line_breaks(text):
    # Linebreak in XML becomes two line breaks in RST; unless inside a [codeblock].
    lines = text.splitlines()
    result = []
    inside_codeblock = False
    for line in lines:
        if line.startswith("[codeblock]"):
            inside_codeblock = True
        elif line.startswith("[/codeblock]"):
            inside_codeblock = False
        result.append(line)
        if not inside_codeblock:
            result.append("")
    if inside_codeblock:
        rst_error("[codeblock] without a closing tag")
        return ""
    return "\n".join(result[:-1])


def escape_special_characters(text):
    escaped_text = ""
    for character in text:
        if character == "*":
            escaped_text += "\*"
        elif character == "\\":
            escaped_text += "\\\\"
        else:
            escaped_text += character
    position = escaped_text.find("_")
    while position != -1:
        if not text[position + 1].isalnum():
            escaped_text = escaped_text[:position] + "\_" + escaped_text[position + 1 :]
        position = escaped_text.find("_", position + 2)
    return escaped_text


def extract_next_tag(text):
    (before_text, tag_name, after_text) = extract_next_tag_name(text)
    if tag_name == "":
        return (before_text, None, after_text)
    tag = TagDef(tag_name)
    (tag.name, tag.value) = extract_tag_value(tag_name)
    block_tags = [
        "b",
        "code",
        "codeblock",
        "i",
        "img",
        "url",
    ]
    if tag.name in block_tags:
        (tag.contents, after_text) = extract_tag_contents(tag.name, after_text)
    return (before_text, tag, after_text)


def extract_next_tag_name(text):
    tag_open = text.find("[")
    tag_close = text.find("]", tag_open)
    if tag_open == -1 or tag_close == -1:
        return (text, "", "")
    before_text = text[:tag_open]
    tag_name = text[tag_open + 1 : tag_close]
    after_text = text[tag_close + 1 :]
    return (before_text, tag_name, after_text)


def extract_tag_value(tag):
    components = tag.split(" ", 1)
    if len(components) == 1:
        # Look for tag name with shorthand "=" option.
        components = tag.split("=")
        if len(components) == 1:
            return (tag, "")
        if len(components) == 2:
            return components
        rst_error("Failed to extract tag value from {}".format(tag))
        exit(1)
    return components


def extract_tag_contents(tag_name, text):
    tag_contents = ""
    next_tag_name = ""
    remaining_text = text
    while next_tag_name != ("/" + tag_name) and remaining_text:
        (before_text, next_tag_name, remaining_text) = extract_next_tag_name(
            remaining_text
        )
        tag_contents += before_text
        if next_tag_name != ("/" + tag_name):
            tag_contents += "[" + next_tag_name + "]"
    if next_tag_name != ("/" + tag_name):
        rst_error("Failed to find closing tag [\{}] in {}".format(tag_name, text))
        exit(1)
    return (tag_contents, remaining_text)


def rst_tag(tag):
    if tag is None:
        return ""
    if tag.name == current_class:
        return rst_inline_code(tag.name)
    match tag.name:
        case "b":
            return rst_bold(rst_text(tag.contents))
        case "code":
            return rst_inline_code(tag.contents)
        case "codeblock":
            return rst_codeblock(tag.contents)
        case "constant" | "member" | "method" | "signal":
            return rst_reference_link(tag.name, tag.value)
        case "enum":
            return rst_enum(tag.value)
        case "i":
            return rst_italics(rst_text(tag.contents))
        case "img":
            return rst_image(tag.contents, tag.value)
        case "url":
            return rst_url(tag.value, rst_text(tag.contents))
        case _:
            return rst_class_reference_link(tag.name)


def rst_codeblock(contents):
    if contents.startswith("\n"):
        contents = contents[1:]
    if contents.endswith("\n"):
        contents = contents[:-1]
    indent = "    "
    codeblock = "::\n\n" + textwrap.indent(contents, indent, lambda line: True)
    if contents.endswith("\n"):
        codeblock += indent
    return codeblock


def rst_enum(enum):
    enum_components = enum.split(".")
    if len(enum_components) == 1:
        class_name = current_class
        enum_name = enum
    elif len(enum_components) == 2:
        class_name, enum_name = enum_components
    else:
        rst_error("Bad enum reference: '{}'".format(enum))
        return enum
    if class_name.startswith("_"):
        class_name = class_name[1:]
    elif class_name == "Variant":
        # Variant enums live in GlobalScope but still use periods.
        class_name = "@GlobalScope"
        enum_name = "Variant." + enum_name
    if enum_name not in class_defs[class_name].enums:
        class_name = "@GlobalScope"
        if enum_name not in class_defs[class_name].enums:
            # Don't fail for `Vector3.Axis`
            # TODO: Fix resolution failure.
            if enum != "Vector3.Axis":
                rst_error("Unresolved enum '{}'".format(enum))
            return enum
    return ":ref:`{1}<enum_{0}_{1}>`".format(class_name, enum_name)


def rst_image(image_file, image_options):
    image = ".. figure:: {}".format(image_file)
    if not image_options:
        return image
    options = parse_options(image_options)
    for option, value in options.items():
        match option:
            case "alt" | "height" | "width" | "scale" | "align" | "target":
                image += rst_image_option(option, value)
            case "caption":
                # Added at the end
                pass
            case _:
                rst_error("Unrecognised image option {}={}".format(option, value))
    if "caption" in options:
        image += "\n\n    {}".format(options["caption"])
    return image


def rst_image_option(option, value):
    return "\n    :{}: {}".format(option, value)


def parse_options(options_string):
    if "=" not in options_string:
        # Shorthand option
        return get_dimensions(options_string)
    options = OrderedDict()
    while options_string:
        option, value = options_string.split("=", 1)
        if value.startswith('"'):
            end_value = value.find('"', 1)
            options_string = value[end_value + 1 :].strip()
            value = value[1:end_value]
        else:
            value, options_string = value.split(" ", 1)
        options[option] = value
    return options


def get_dimensions(value):
    dimensions = value.split("x")
    if len(dimensions) > 2:
        rst_error("Unrecognised dimensions string: {}").format(value)
        return {}
    if len(dimensions) == 1:
        return {"width": value}
    return {"width": dimensions[0], "height": dimensions[1]}


def rst_url(link, label):
    if not label:
        label = link
    return "`" + label + " <" + link + ">`__"


class ClassDef:
    def __init__(self, name):
        self.name = name
        self.inherits = None
        self.brief_description = None
        self.description = None
        self.tutorials = []
        self.methods = OrderedDict()
        self.properties = OrderedDict()
        self.signals = OrderedDict()
        self.constants = OrderedDict()
        self.enums = OrderedDict()
        self.theme_items = OrderedDict()


class MethodDef:
    def __init__(self, name, qualifiers, return_type, parameters, description):
        self.name = name
        self.qualifiers = qualifiers
        self.return_type = return_type
        self.parameters = parameters
        self.description = description


class ParameterDef:
    def __init__(self, name, type_def, default):
        self.name = name
        self.type_def = type_def
        self.default = default


class PropertyDef:
    def __init__(self, name, type_def, setter, getter, default, overridden, text):
        self.name = name
        self.type_def = type_def
        self.setter = setter
        self.getter = getter
        self.default = default
        self.overridden = overridden
        self.text = text


class SignalDef:
    def __init__(self, name, parameters, description):
        self.name = name
        self.parameters = parameters
        self.description = description


class ConstantDef:
    def __init__(self, name, value, text):
        self.name = name
        self.value = value
        self.text = text


class EnumDef:
    def __init__(self, name):
        self.name = name
        self.values = OrderedDict()


class ThemeItemDef:
    def __init__(self, name, data_type, type_def, default, text):
        self.name = name
        self.data_type = data_type
        self.type_def = type_def
        self.default = default
        self.text = text


class TypeDef:
    def __init__(self, element):
        self.type = "void"
        self.enum = enum = None
        if element is not None:
            self.type = element.get("type")
            self.enum = element.get("enum")


class TagDef:
    def __init__(self, name):
        self.name = name
        self.value = ""
        self.contents = ""


if __name__ == "__main__":
    main()
