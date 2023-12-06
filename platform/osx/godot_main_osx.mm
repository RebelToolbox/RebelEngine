// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "main/main.h"
#include "os_osx.h"

#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int first_arg       = 1;
    const char* dbg_arg = "-NSDocumentRevisionsDebugMode";
    printf("arguments\n");
    for (int i = 0; i < argc; i++) {
        if (strcmp(dbg_arg, argv[i]) == 0) {
            first_arg = i + 2;
        }
        printf("%i: %s\n", i, argv[i]);
    };

#ifdef DEBUG_ENABLED
    // lets report the path we made current after all that
    char cwd[4096];
    getcwd(cwd, 4096);
    printf("Current path: %s\n", cwd);
#endif

    OS_OSX os;
    Error err;

    if (os.open_with_filename != "") {
        char* argv_c = (char*)malloc(os.open_with_filename.utf8().size());
        memcpy(
            argv_c,
            os.open_with_filename.utf8().get_data(),
            os.open_with_filename.utf8().size()
        );
        err = Main::setup(argv[0], 1, &argv_c);
        free(argv_c);
    } else {
        err = Main::setup(argv[0], argc - first_arg, &argv[first_arg]);
    }

    if (err != OK) {
        return 255;
    }

    if (Main::start()) {
        os.run(); // it is actually the OS that decides how to run
    }

    Main::cleanup();

    return os.get_exit_code();
};
