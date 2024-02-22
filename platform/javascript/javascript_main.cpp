// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "core/io/resource_loader.h"
#include "godot_js.h"
#include "main/main.h"
#include "platform/javascript/os_javascript.h"

#include <emscripten/emscripten.h>
#include <stdlib.h>

static OS_JavaScript* os     = NULL;
static uint64_t target_ticks = 0;

void exit_callback() {
    emscripten_cancel_main_loop(); // After this, we can exit!
    Main::cleanup();
    int exit_code = OS_JavaScript::get_singleton()->get_exit_code();
    memdelete(os);
    os = NULL;
    emscripten_force_exit(exit_code
    ); // No matter that we call cancel_main_loop, regular "exit" will not work,
       // forcing.
}

void cleanup_after_sync() {
    emscripten_set_main_loop(exit_callback, -1, false);
}

void main_loop_callback() {
    uint64_t current_ticks = os->get_ticks_usec();

    bool force_draw = os->check_size_force_redraw();
    if (force_draw) {
        Main::force_redraw();
    } else if (current_ticks < target_ticks && !force_draw) {
        return; // Skip frame.
    }

    int target_fps = Engine::get_singleton()->get_target_fps();
    if (target_fps > 0) {
        if (current_ticks - target_ticks > 1000000) {
            // When the window loses focus, we stop getting updates and
            // accumulate delay. For this reason, if the difference is too big,
            // we reset target ticks to the current ticks.
            target_ticks = current_ticks;
        }
        target_ticks += (uint64_t)(1000000 / target_fps);
    }
    if (os->main_loop_iterate()) {
        emscripten_cancel_main_loop(
        ); // Cancel current loop and wait for finalize_async.
        os->get_main_loop()->finish();
        rebel_js_os_finish_async(cleanup_after_sync);
    }
}

extern EMSCRIPTEN_KEEPALIVE int rebel_js_main(int argc, char* argv[]) {
    // Set locale
    char locale_ptr[16];
    rebel_js_config_locale_get(locale_ptr, sizeof(locale_ptr));
    setenv("LANG", locale_ptr, true);

    os = new OS_JavaScript();

    Main::setup(argv[0], argc - 1, &argv[1]);
    // Ease up compatibility.
    ResourceLoader::set_abort_on_missing_resources(false);
    Main::start();
    os->get_main_loop()->init();
#ifdef TOOLS_ENABLED
    if (Main::is_project_manager() && FileAccess::exists("/tmp/preload.zip")) {
        PoolStringArray ps;
        ps.push_back("/tmp/preload.zip");
        os->get_main_loop()->emit_signal("files_dropped", ps, -1);
    }
#endif
    emscripten_set_main_loop(main_loop_callback, -1, false);
    // Immediately run the first iteration.
    // We are inside an animation frame, we want to immediately draw on the
    // newly setup canvas.
    main_loop_callback();

    return 0;
}
