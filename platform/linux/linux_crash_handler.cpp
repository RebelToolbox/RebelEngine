// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "linux_crash_handler.h"

#include "core/os/os.h"
#include "core/project_settings.h"
#include "core/version.h"
#include "core/version_hash.gen.h"
#include "main/main.h"

#ifdef DEBUG_ENABLED
#define CRASH_HANDLER_ENABLED 1
#endif

#ifdef CRASH_HANDLER_ENABLED
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>

static void handle_crash(int sig) {
    if (OS::get_singleton() == nullptr) {
        abort();
    }

    void* bt_buffer[256];
    size_t size      = backtrace(bt_buffer, 256);
    String _execpath = OS::get_singleton()->get_executable_path();

    String msg;
    const ProjectSettings* proj_settings = ProjectSettings::get_singleton();
    if (proj_settings) {
        msg = proj_settings->get("debug/settings/crash_handler/message");
    }

    // Dump the backtrace to stderr with a message to the user
    fprintf(
        stderr,
        "\n================================================================\n"
    );
    fprintf(stderr, "%s: Program crashed with signal %d\n", __FUNCTION__, sig);

    if (OS::get_singleton()->get_main_loop()) {
        OS::get_singleton()->get_main_loop()->notification(
            MainLoop::NOTIFICATION_CRASH
        );
    }

    // Print the engine version just before, so that people are reminded to
    // include the version in backtrace reports.
    if (String(VERSION_HASH).length() != 0) {
        fprintf(
            stderr,
            "Engine version: " VERSION_FULL_NAME " (" VERSION_HASH ")\n"
        );
    } else {
        fprintf(stderr, "Engine version: " VERSION_FULL_NAME "\n");
    }
    fprintf(stderr, "Dumping the backtrace. %ls\n", msg.c_str());
    char** strings = backtrace_symbols(bt_buffer, size);
    if (strings) {
        for (size_t i = 1; i < size; i++) {
            char fname[1024];
            Dl_info info;

            snprintf(fname, 1024, "%s", strings[i]);

            // Try to demangle the function name to provide a more readable one
            if (dladdr(bt_buffer[i], &info) && info.dli_sname) {
                if (info.dli_sname[0] == '_') {
                    int status;
                    char* demangled = abi::__cxa_demangle(
                        info.dli_sname,
                        nullptr,
                        nullptr,
                        &status
                    );

                    if (status == 0 && demangled) {
                        snprintf(fname, 1024, "%s", demangled);
                    }

                    if (demangled) {
                        free(demangled);
                    }
                }
            }

            List<String> args;

            char str[1024];
            snprintf(str, 1024, "%p", bt_buffer[i]);
            args.push_back(str);
            args.push_back("-e");
            args.push_back(_execpath);

            String output = "";

            // Try to get the file/line number using addr2line
            int ret;
            Error err = OS::get_singleton()->execute(
                String("addr2line"),
                args,
                true,
                nullptr,
                &output,
                &ret
            );
            if (err == OK) {
                output.erase(output.length() - 1, 1);
            }

            fprintf(
                stderr,
                "[%ld] %s (%ls)\n",
                (long int)i,
                fname,
                output.c_str()
            );
        }

        free(strings);
    }
    fprintf(stderr, "-- END OF BACKTRACE --\n");
    fprintf(
        stderr,
        "================================================================\n"
    );

    // Abort to pass the error to the OS
    abort();
}
#endif

CrashHandler::CrashHandler() {
    disabled = false;
}

CrashHandler::~CrashHandler() {
    disable();
}

void CrashHandler::disable() {
    if (disabled) {
        return;
    }

#ifdef CRASH_HANDLER_ENABLED
    signal(SIGSEGV, nullptr);
    signal(SIGFPE, nullptr);
    signal(SIGILL, nullptr);
#endif

    disabled = true;
}

void CrashHandler::initialize() {
#ifdef CRASH_HANDLER_ENABLED
    signal(SIGSEGV, handle_crash);
    signal(SIGFPE, handle_crash);
    signal(SIGILL, handle_crash);
#endif
}
