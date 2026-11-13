// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef UNIX_OS_H
#define UNIX_OS_H

#ifdef UNIX_ENABLED

#include "core/os/os.h"
#include "drivers/network/default_ip.h"

class UnixOS : public OS {
protected:
    // Unix only handles the core functions.
    // Platforms the inherit Unix should handle the rest.

    void initialize_core() override;
    virtual int unix_initialize_audio(int p_audio_driver);

    void finalize_core() override;

    String stdin_buf;

public:
    UnixOS();

    void alert(
        const String& p_alert,
        const String& p_title = "ALERT!"
    ) override;
    String get_stdin_string(bool p_block) override;

    Error open_dynamic_library(
        const String p_path,
        void*& p_library_handle,
        bool p_also_set_library_path = false
    ) override;
    Error close_dynamic_library(void* p_library_handle) override;
    Error get_dynamic_library_symbol_handle(
        void* p_library_handle,
        const String p_name,
        void*& p_symbol_handle,
        bool p_optional = false
    ) override;

    Error set_cwd(const String& p_cwd) override;

    String get_name() const override;

    Date get_date(bool utc) const override;
    Time get_time(bool utc) const override;
    TimeZoneInfo get_time_zone_info() const override;

    uint64_t get_unix_time() const override;
    uint64_t get_system_time_secs() const override;
    uint64_t get_system_time_msecs() const override;

    void delay_usec(uint32_t p_usec) const override;
    uint64_t get_ticks_usec() const override;

    Error execute(
        const String& p_path,
        const List<String>& p_arguments,
        bool p_blocking       = true,
        ProcessID* r_child_id = nullptr,
        String* r_pipe        = nullptr,
        int* r_exitcode       = nullptr,
        bool read_stderr      = false,
        Mutex* p_pipe_mutex   = nullptr
    ) override;
    Error kill(const ProcessID& p_pid) override;
    int get_process_id() const override;

    bool has_environment(const String& p_var) const override;
    String get_environment(const String& p_var) const override;
    bool set_environment(
        const String& p_var,
        const String& p_value
    ) const override;
    String get_locale() const override;

    int get_processor_count() const override;

    void debug_break() override;
    void initialize_debugging() override;

    String get_executable_path() const override;
    String get_user_data_dir() const override;
};

class UnixTerminalLogger : public StdLogger {
public:
    void log_error(
        const char* p_function,
        const char* p_file,
        int p_line,
        const char* p_code,
        const char* p_rationale,
        ErrorType p_type = ERR_ERROR
    ) override;
    ~UnixTerminalLogger() override;
};

#endif // UNIX_ENABLED

#endif // UNIX_OS_H
