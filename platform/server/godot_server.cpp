// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "main/main.h"
#include "os_server.h"

int main(int argc, char* argv[]) {
    OS_Server os;

    Error err = Main::setup(argv[0], argc - 1, &argv[1]);
    if (err != OK) {
        return 255;
    }

    if (Main::start()) {
        os.run(); // it is actually the OS that decides how to run
    }
    Main::cleanup();

    return os.get_exit_code();
}
