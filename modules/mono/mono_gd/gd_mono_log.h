// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GD_MONO_LOG_H
#define GD_MONO_LOG_H

#include "core/typedefs.h"

#include <mono/utils/mono-logger.h>

#if !defined(JAVASCRIPT_ENABLED) && !defined(IPHONE_ENABLED)
// We have custom mono log callbacks for WASM and iOS
#define GD_MONO_LOG_ENABLED
#endif

#ifdef GD_MONO_LOG_ENABLED
#include "core/os/file_access.h"
#endif

class GDMonoLog {
#ifdef GD_MONO_LOG_ENABLED
    int log_level_id;

    FileAccess* log_file;
    String log_file_path;

    bool _try_create_logs_dir(const String& p_logs_dir);
    void _delete_old_log_files(const String& p_logs_dir);

    static void mono_log_callback(
        const char* log_domain,
        const char* log_level,
        const char* message,
        mono_bool fatal,
        void* user_data
    );
#endif

    static GDMonoLog* singleton;

public:
    _FORCE_INLINE_ static GDMonoLog* get_singleton() {
        return singleton;
    }

    void initialize();

    GDMonoLog();
    ~GDMonoLog();
};

#endif // GD_MONO_LOG_H
