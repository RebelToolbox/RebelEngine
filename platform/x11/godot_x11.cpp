// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "main/main.h"
#include "os_x11.h"

#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    OS_X11 os;

    setlocale(LC_CTYPE, "");

    char* cwd = (char*)malloc(PATH_MAX);
    ERR_FAIL_COND_V(!cwd, ERR_OUT_OF_MEMORY);
    char* ret = getcwd(cwd, PATH_MAX);

    Error err = Main::setup(argv[0], argc - 1, &argv[1]);
    if (err != OK) {
        free(cwd);
        return 255;
    }

    if (Main::start()) {
        os.run(); // it is actually the OS that decides how to run
    }
    Main::cleanup();

    if (ret) { // Previous getcwd was successful
        if (chdir(cwd) != 0) {
            ERR_PRINT("Couldn't return to previous working directory.");
        }
    }
    free(cwd);

    return os.get_exit_code();
}
