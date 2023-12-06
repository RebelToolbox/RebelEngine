// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include "core/list.h"
#include "core/os/main_loop.h"
#include "core/ustring.h"

const char** tests_get_names();
MainLoop* test_main(String p_test, const List<String>& p_args);

#endif // TEST_MAIN_H
