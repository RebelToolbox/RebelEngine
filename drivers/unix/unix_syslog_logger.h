// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef UNIX_SYSLOG_LOGGER_H
#define UNIX_SYSLOG_LOGGER_H

#ifdef UNIX_ENABLED

#include "core/io/logger.h"

class UnixSyslogLogger : public Logger {
public:
    virtual void logv(const char* p_format, va_list p_list, bool p_err)
        _PRINTF_FORMAT_ATTRIBUTE_2_0;
    virtual void print_error(
        const char* p_function,
        const char* p_file,
        int p_line,
        const char* p_code,
        const char* p_rationale,
        ErrorType p_type
    );

    virtual ~UnixSyslogLogger();
};

#endif // UNIX_ENABLED

#endif // UNIX_SYSLOG_LOGGER_H
