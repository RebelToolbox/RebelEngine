// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PRINT_STRING_H
#define PRINT_STRING_H

#include "core/ustring.h"

extern void (*_print_func)(String);

typedef void (*PrintHandlerFunc)(void*, const String& p_string, bool p_error);

struct PrintHandlerList {
    PrintHandlerFunc printfunc;
    void* userdata;

    PrintHandlerList* next;

    PrintHandlerList() {
        printfunc = nullptr;
        next      = nullptr;
        userdata  = nullptr;
    }
};

void add_print_handler(PrintHandlerList* p_handler);
void remove_print_handler(PrintHandlerList* p_handler);

extern bool _print_line_enabled;
extern bool _print_error_enabled;
extern void print_line(String p_string);
extern void print_error(String p_string);
extern void print_verbose(String p_string);

#endif
