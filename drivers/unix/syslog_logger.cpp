// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef UNIX_ENABLED

#include "syslog_logger.h"

#include "core/print_string.h"

#include <syslog.h>

void SyslogLogger::logv(const char* p_format, va_list p_list, bool p_err) {
    if (!should_log(p_err)) {
        return;
    }

    vsyslog(p_err ? LOG_ERR : LOG_INFO, p_format, p_list);
}

void SyslogLogger::print_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    const char* p_code,
    const char* p_rationale,
    ErrorType p_type
) {
    if (!should_log(true)) {
        return;
    }

    const char* err_type = "**ERROR**";
    switch (p_type) {
        case ERR_ERROR:
            err_type = "**ERROR**";
            break;
        case ERR_WARNING:
            err_type = "**WARNING**";
            break;
        case ERR_SCRIPT:
            err_type = "**SCRIPT ERROR**";
            break;
        case ERR_SHADER:
            err_type = "**SHADER ERROR**";
            break;
        default:
            ERR_PRINT("Unknown error type");
            break;
    }

    const char* err_details;
    if (p_rationale && *p_rationale) {
        err_details = p_rationale;
    } else {
        err_details = p_code;
    }

    syslog(
        p_type == ERR_WARNING ? LOG_WARNING : LOG_ERR,
        "%s: %s\n   At: %s:%i:%s() - %s",
        err_type,
        err_details,
        p_file,
        p_line,
        p_function,
        p_code
    );
}

SyslogLogger::~SyslogLogger() {}

#endif
