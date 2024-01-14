#!/usr/bin/env python3

import argparse
import enum
import fnmatch
import os
import shutil
import subprocess
import sys


HEADER = """
# SPDX-FileCopyrightText: 2023 Rebel Engine contributors
# SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
# SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
#
# SPDX-License-Identifier: MIT
#
# LANGUAGE translation of the Rebel Editor.
#
# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.
#
#, fuzzy
msgid ""
msgstr ""
"Project-Id-Version: Rebel Editor\\n"
"Report-Msgid-Bugs-To: https://github.com/RebelToolbox/RebelEngine\\n"
"MIME-Version: 1.0\\n"
"Content-Type: text/plain; charset=UTF-8\\n"
"Content-Transfer-Encoding: 8-bit\\n"
"""

EXTRACT_TAGS = [
    "RTR(",
    "TTR(",
    "TTRC(",
]


class UniqueStringData:
    def __init__(self, string, filename, comment):
        self.string = string
        self.filenames = [filename]
        self.comments = [comment]


class StringData:
    def __init__(self, string, filename, comment):
        self.string = string
        self.filename = filename
        self.comment = comment


class ParseData:
    state = "SEARCHING"
    previous_line = ""
    line_remainder = ""
    comment = ""
    string = ""


def get_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--output",
        "-o",
        default="editor/translations/editor.pot",
        help="The translation template .pot file.",
    )
    return parser.parse_args()


def print_error(error):
    print("ERROR: {}".format(error))


def get_files():
    filepaths = []
    for root, dirs, files in os.walk("."):
        dirs[:] = [d for d in dirs if d not in ["thirdparty"]]
        files[:] = [f for f in files if not f.endswith(".gen.h")]
        for file in fnmatch.filter(files, "*.cpp"):
            filepaths.append(os.path.join(root, file))
        for file in fnmatch.filter(files, "*.h"):
            filepaths.append(os.path.join(root, file))
    filepaths.sort()
    return filepaths


def search_line(line, parse_data):
    if line.find("TRANSLATORS:") != -1:
        start_comment(line, parse_data)
    else:
        for tag in EXTRACT_TAGS:
            if line.find(tag) != -1:
                start_string(line, tag, parse_data)


def start_comment(line, parse_data):
    if line.startswith("//"):
        line = line[2:].strip()
        parse_data.state = "GETTING_COMMENT"
    elif line.startswith("/*"):
        line = line[2:].strip()
        parse_data.state = "GETTING_BLOCK_COMMENT"
    elif parse_data.previous_line.startswith("/*"):
        if line.startswith("*"):
            line = line[1:].strip()
        parse_data.state = "GETTING_BLOCK_COMMENT"
    else:
        print_error("TRANSLATORS keyword found outside of comment")
        return

    if not line.startswith("TRANSLATORS:"):
        print_error("TRANSLATORS keyword not found at the beginning of comment")
        parse_data.state = "SEARCHING"
        return
    if parse_data.state == "GETTING_BLOCK_COMMENT" and line.endswith("*/"):
        line = line[-2:].strip()
        parse_data.state = "SEARCHING"
    parse_data.comment = line[len("TRANSLATORS:") :]


def update_block_comment(line, parse_data):
    if line.endswith("*/"):
        line = line[-2:].strip()
        parse_data.state = "SEARCHING"
    if line.startswith("*"):
        line = line[1:].strip()
    parse_data.comment += " " + line


def update_comment(line, parse_data):
    if line.startswith("//"):
        line = line[2:].strip()
        parse_data.comment += " " + line
    else:
        parse_data.state = "SEARCHING"
        parse_line(line, parse_data)


def find_unescaped_quote(line):
    pos = line.find('"')
    while pos > 0 and line[pos - 1] == "\\":
        pos = line.find('"', pos + 1)
    return pos


def check_for_tag_close(line, end, parse_data):
    close = line.find(")", end)
    if close != -1:
        parse_data.state = "READY"
        parse_data.line_remainder = line[close + 1 :]


def start_string(line, tag, parse_data):
    parse_data.state = "GETTING_STRING"

    start = line.find(tag) + len(tag)
    line = line[start:].strip()
    if not line:
        # String starts on next line.
        return
    if not line.startswith('"'):
        # Not a quoted string.
        parse_data.state = "SEARCHING"
        return
    line = line[1:].strip()

    end = find_unescaped_quote(line)
    if end == -1:
        print_error("Failed to find closing quotes")
        exit(1)
    string = line[:end]
    parse_data.string = string
    check_for_tag_close(line, end, parse_data)


def update_string(line, parse_data):
    if line.startswith(")"):
        # String ended on previous line".
        parse_data.state = "READY"
        return
    if not line.startswith('"'):
        if parse_data.string:
            print_error("Parsing multi-line string failed")
            exit(1)
        else:
            # Not a quoted string.
            parse_data.state = "SEARCHING"
            return

    line = line[1:].strip()

    end = find_unescaped_quote(line)
    if end == -1:
        print_error("Failed to find closing quotes")
        exit(1)
    string = line[:end]
    parse_data.string += string
    check_for_tag_close(line, end + 1, parse_data)


def parse_line(line, parse_data):
    match parse_data.state:
        case "SEARCHING":
            search_line(line, parse_data)
        case "GETTING_BLOCK_COMMENT":
            update_block_comment(line, parse_data)
        case "GETTING_COMMENT":
            update_comment(line, parse_data)
        case "GETTING_STRING":
            update_string(line, parse_data)
        case _:
            print_error("Parsing state not recognised!")
            exit(1)
    parse_data.previous_line = line


def parse_file(filepath):
    print("Parsing file: {}".format(filepath))
    string_data_list = []
    parse_data = ParseData()

    with open(filepath, "r", encoding="utf8") as file:
        for line in file:
            line = line.strip()
            while line:
                parse_line(line, parse_data)
                line = parse_data.line_remainder
                if parse_data.state == "READY":
                    string = parse_data.string
                    filename = os.path.relpath(filepath).replace("\\", "/")
                    comment = parse_data.comment
                    string_data = StringData(string, filename, comment)
                    string_data_list.append(string_data)
                    parse_data = ParseData()

    if parse_data.state != "SEARCHING":
        print_error("Parsing failed!")
        exit(1)

    return string_data_list


def merge_string_data(string_data_list):
    unique_strings = {}
    for string_data in string_data_list:
        string = string_data.string
        filename = string_data.filename
        comment = string_data.comment
        if string in unique_strings:
            unique_string = unique_strings[string]
            if filename not in unique_string.filenames:
                unique_string.filenames.append(filename)
            unique_string.comments.append(comment)
        else:
            unique_strings[string] = UniqueStringData(string, filename, comment)
    return unique_strings


def format_translator_comment(comment):
    if not comment:
        return ""
    comment_lines = comment.split("\n")
    formatted_comment = "#. TRANSLATORS: "
    for i in range(len(comment_lines)):
        if i == 0:
            formatted_comment += comment_lines[i].strip() + "\n"
        else:
            formatted_comment += "#. " + comment_lines[i].strip() + "\n"
    return formatted_comment


def split_lines(string):
    lines = []
    remainder = string
    new_line = remainder.find("\\n")
    while new_line != -1:
        line = remainder[:new_line]
        remainder = remainder[new_line + 2 :]
        new_line = remainder.find("\\n")
        lines.append(line + "\\n")
    lines.append(remainder)
    return lines


def format_msgid_string(string):
    lines = split_lines(string)
    if len(lines) == 1:
        return 'msgid "' + lines[0] + '"'
    multi_line_string = 'msgid ""'
    for line in lines:
        multi_line_string += '\n"' + line + '"'
    return multi_line_string


def create_translation_template(unique_string_data, output):
    with open(output, "w") as file:
        file.write(HEADER)
        file.write("\n")
        for string_data in unique_string_data.values():
            for comment in string_data.comments:
                file.write(format_translator_comment(comment))
            file.write("#:")
            for filename in string_data.filenames:
                file.write(" " + filename)
            file.write("\n")
            file.write(format_msgid_string(string_data.string))
            file.write('\nmsgstr ""\n\n')


def wrap_strings(output):
    print("Wrapping strings at 79 characters for compatibility with Weblate.")
    os.system("msgmerge -w79 {0} {0} > {0}.wrap".format(output))
    shutil.move("{}.wrap".format(output), output)


def print_results(output):
    added = subprocess.check_output(
        "git diff {} | grep \\+msgid | wc -l".format(output),
        shell=True,
    )
    removed = subprocess.check_output(
        "git diff {} | grep \\\\-msgid | wc -l".format(output),
        shell=True,
    )
    print("# Template changes compared to the staged status:")
    print("# Additions: {} msgids.".format(int(added)))
    print("# Deletions: {} msgids.".format(int(removed)))


def main():
    if not shutil.which("msgmerge"):
        print_error("'msgmerge' is required, but it's not installed.")
        exit(1)
    if not os.path.exists("editor"):
        print_error("This script must be run from the root of the git repo.")
        exit(1)
    arguments = get_arguments()
    output = os.path.abspath(arguments.output)
    string_data_list = []
    filepaths = get_files()
    for filepath in filepaths:
        string_data_list += parse_file(filepath)
    unique_string_data = merge_string_data(string_data_list)
    create_translation_template(unique_string_data, output)
    wrap_strings(output)
    print_results(output)


if __name__ == "__main__":
    main()
