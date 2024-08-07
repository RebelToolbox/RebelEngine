// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "macos_crash_handler.h"

#include "core/os/os.h"
#include "core/project_settings.h"
#include "core/version.h"
#include "core/version_hash.gen.h"
#include "main/main.h"

#include <string.h>
#include <unistd.h>

#if defined(DEBUG_ENABLED)
#define CRASH_HANDLER_ENABLED 1
#endif

#ifdef CRASH_HANDLER_ENABLED
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <mach-o/dyld.h>
#include <mach-o/getsect.h>
#include <signal.h>
#include <stdlib.h>

static uint64_t load_address() {
    const struct segment_command_64* cmd = getsegbyname("__TEXT");
    char full_path[1024];
    uint32_t size = sizeof(full_path);

    if (cmd && !_NSGetExecutablePath(full_path, &size)) {
        uint32_t dyld_count = _dyld_image_count();
        for (uint32_t i = 0; i < dyld_count; i++) {
            const char* image_name = _dyld_get_image_name(i);
            if (image_name && strncmp(image_name, full_path, 1024) == 0) {
                return cmd->vmaddr + _dyld_get_image_vmaddr_slide(i);
            }
        }
    }

    return 0;
}

static void handle_crash(int sig) {
    if (OS::get_singleton() == NULL) {
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
        void* load_addr = (void*)load_address();

        for (size_t i = 1; i < size; i++) {
            char fname[1024];
            Dl_info info;

            snprintf(fname, 1024, "%s", strings[i]);

            // Try to demangle the function name to provide a more readable one
            if (dladdr(bt_buffer[i], &info) && info.dli_sname) {
                if (info.dli_sname[0] == '_') {
                    int status;
                    char* demangled =
                        abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);

                    if (status == 0 && demangled) {
                        snprintf(fname, 1024, "%s", demangled);
                    }

                    if (demangled) {
                        free(demangled);
                    }
                }
            }

            String output = fname;

            // Try to get the file/line number using atos
            if (bt_buffer[i] > (void*)0x0 && OS::get_singleton()) {
                List<String> args;
                char str[1024];

                args.push_back("-o");
                args.push_back(_execpath);
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__)
                args.push_back("-arch");
                args.push_back("x86_64");
#elif defined(__aarch64__)
                args.push_back("-arch");
                args.push_back("arm64");
#endif
                args.push_back("-l");
                snprintf(str, 1024, "%p", load_addr);
                args.push_back(str);
                snprintf(str, 1024, "%p", bt_buffer[i]);
                args.push_back(str);

                int ret;
                String out = "";
                Error err =
                    OS::get_singleton()
                        ->execute(String("atos"), args, true, NULL, &out, &ret);
                if (err == OK && out.substr(0, 2) != "0x") {
                    out.erase(out.length() - 1, 1);
                    output = out;
                }
            }

            fprintf(stderr, "[%zu] %ls\n", i, output.c_str());
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
    signal(SIGSEGV, NULL);
    signal(SIGFPE, NULL);
    signal(SIGILL, NULL);
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
