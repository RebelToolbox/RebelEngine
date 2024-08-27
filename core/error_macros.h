// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#include "core/safe_refcount.h"
#include "core/typedefs.h"

class String;

enum ErrorHandlerType {
    ERR_HANDLER_ERROR,
    ERR_HANDLER_WARNING,
    ERR_HANDLER_SCRIPT,
    ERR_HANDLER_SHADER,
};

/**
 * Pointer to the error handler printing function.
 * Reassign to any function to have errors printed.
 */
typedef void (*ErrorHandlerFunc)(
    void* p_user_data,
    const char* p_function,
    const char* p_file,
    int p_line,
    const char* p_error,
    const char* p_message,
    ErrorHandlerType p_type
);

struct ErrorHandlerList {
    ErrorHandlerFunc errfunc = nullptr;
    void* userdata           = nullptr;
    ErrorHandlerList* next   = nullptr;
};

void add_error_handler(ErrorHandlerList* p_handler);
void remove_error_handler(ErrorHandlerList* p_handler);

// Functions used by the error macros.
void _err_print_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    const char* p_error,
    ErrorHandlerType p_type = ERR_HANDLER_ERROR
);

void _err_print_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    const String& p_error,
    ErrorHandlerType p_type = ERR_HANDLER_ERROR
);

void _err_print_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    const char* p_error,
    const char* p_message,
    ErrorHandlerType p_type = ERR_HANDLER_ERROR
);

void _err_print_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    const String& p_error,
    const char* p_message,
    ErrorHandlerType p_type = ERR_HANDLER_ERROR
);

void _err_print_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    const char* p_error,
    const String& p_message,
    ErrorHandlerType p_type = ERR_HANDLER_ERROR
);

void _err_print_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    const String& p_error,
    const String& p_message,
    ErrorHandlerType p_type = ERR_HANDLER_ERROR
);

void _err_print_index_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    int64_t p_index,
    int64_t p_size,
    const char* p_index_str,
    const char* p_size_str,
    const char* p_message = "",
    bool fatal            = false
);

void _err_print_index_error(
    const char* p_function,
    const char* p_file,
    int p_line,
    int64_t p_index,
    int64_t p_size,
    const char* p_index_str,
    const char* p_size_str,
    const String& p_message,
    bool fatal = false
);

void _err_flush_stdout();

// Don't use this directly; instead, use any of the CRASH_* macros.
#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

/**
 * ERROR MACROS
 *
 * These macros work in the opposite way to assert(). Unlike exceptions and
 * asserts, these macros try to maintain consistency and stability. In most
 * cases, bugs and/or invalid data are not fatal. They should never allow a
 * perfectly running application to fail or crash. We always try to return
 * processable data, so the engine can keep running well.
 *
 * The `((void)0)` no-op statement is used as a trick to force us to put a
 * semicolon after these macros, making them look like proper statements.
 * The if wrappers are used to ensure that the macro replacement does not
 * trigger unexpected issues when expanded e.g. after an `if (cond) ERR_FAIL();`
 * without braces.
 *
 * Use the _MSG versions to print a meaningful message to help with debugging.
 */

/**
 * 1. Index out of bounds error macros
 * These macros should be used instead of `ERR_FAIL_COND` for bounds checking.
 */

/**
 * 1.1. Integer index out of bounds error macros
 */

/**
 * Ensures `m_index` is less than `m_size` and greater than or equal to 0.
 * On failure, it prints `m_msg` and the current function returns.
 */
#define ERR_FAIL_INDEX_MSG(m_index, m_size, m_msg)                             \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                    \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            m_msg                                                              \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_INDEX_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_INDEX(m_index, m_size)                                        \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                    \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size)                                                       \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Ensures `m_index` is less than `m_size` and greater than or equal to 0.
 * On failure, it prints `m_msg` and the current function returns `m_retval`.
 */
#define ERR_FAIL_INDEX_V_MSG(m_index, m_size, m_retval, m_msg)                 \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                    \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            m_msg                                                              \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_INDEX_V_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_INDEX_V(m_index, m_size, m_retval)                            \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                    \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size)                                                       \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_INDEX_MSG` or `ERR_FAIL_INDEX_V_MSG`.
 * Only use this macro if the error is unrecoverable.
 * Ensures `m_index` is less than `m_size` and greater than or equal to 0.
 * On failure, it prints `m_msg` and the application crashes.
 */
#define CRASH_BAD_INDEX_MSG(m_index, m_size, m_msg)                            \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                    \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            m_msg,                                                             \
            true                                                               \
        );                                                                     \
        GENERATE_TRAP();                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_INDEX_MSG`, `ERR_FAIL_INDEX_V_MSG`.
 * If the error is unrecoverable, try using `CRASH_BAD_INDEX_MSG`.
 */
#define CRASH_BAD_INDEX(m_index, m_size)                                       \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                    \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            "",                                                                \
            true                                                               \
        );                                                                     \
        GENERATE_TRAP();                                                       \
    } else                                                                     \
        ((void)0)

/**
 * 1.2. Unsigned integer index out of bounds error macros
 */

/**
 * Ensures `m_index` is less than `m_size`.
 * On failure, it prints `m_msg` and the current function returns.
 */
#define ERR_FAIL_UNSIGNED_INDEX_MSG(m_index, m_size, m_msg)                    \
    if (unlikely((m_index) >= (m_size))) {                                     \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            m_msg                                                              \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_INDEX_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_UNSIGNED_INDEX(m_index, m_size)                               \
    if (unlikely((m_index) >= (m_size))) {                                     \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size)                                                       \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Ensures `m_index` is less than `m_size`.
 * On failure, it prints `m_msg` and the current function returns `m_retval`.
 */
#define ERR_FAIL_UNSIGNED_INDEX_V_MSG(m_index, m_size, m_retval, m_msg)        \
    if (unlikely((m_index) >= (m_size))) {                                     \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            m_msg                                                              \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_INDEX_V_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_UNSIGNED_INDEX_V(m_index, m_size, m_retval)                   \
    if (unlikely((m_index) >= (m_size))) {                                     \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size)                                                       \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_INDEX_MSG` or `ERR_FAIL_INDEX_V_MSG`.
 * Only use this macro if the error is unrecoverable.
 * Ensures `m_index` is less than `m_size`.
 * On failure, it prints `m_msg` and the application crashes.
 */
#define CRASH_BAD_UNSIGNED_INDEX_MSG(m_index, m_size, m_msg)                   \
    if (unlikely((m_index) >= (m_size))) {                                     \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            m_msg,                                                             \
            true                                                               \
        );                                                                     \
        GENERATE_TRAP();                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_INDEX_MSG`, `ERR_FAIL_INDEX_V_MSG`.
 * If the error is unrecoverable, try using `CRASH_BAD_INDEX_MSG`.
 */
#define CRASH_BAD_UNSIGNED_INDEX(m_index, m_size)                              \
    if (unlikely((m_index) >= (m_size))) {                                     \
        _err_print_index_error(                                                \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            m_index,                                                           \
            m_size,                                                            \
            _STR(m_index),                                                     \
            _STR(m_size),                                                      \
            "",                                                                \
            true                                                               \
        );                                                                     \
        GENERATE_TRAP();                                                       \
    } else                                                                     \
        ((void)0)

/**
 * 2. Null reference error macros
 * These macros should be used instead of `ERR_FAIL_COND` for null references.
 */

/**
 * Ensures a pointer `m_param` is not null.
 * On failure, it prints `m_msg` and the current function returns.
 */
#define ERR_FAIL_NULL_MSG(m_param, m_msg)                                      \
    if (unlikely(m_param == nullptr)) {                                        \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Parameter \"" _STR(m_param) "\" is null.",                        \
            m_msg                                                              \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_NULL_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_NULL(m_param)                                                 \
    if (unlikely(m_param == nullptr)) {                                        \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Parameter \"" _STR(m_param) "\" is null."                         \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Ensures a pointer `m_param` is not null.
 * On failure, it prints `m_msg` and the current function returns `m_retval`.
 */
#define ERR_FAIL_NULL_V_MSG(m_param, m_retval, m_msg)                          \
    if (unlikely(m_param == nullptr)) {                                        \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Parameter \"" _STR(m_param) "\" is null.",                        \
            m_msg                                                              \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_NULL_V_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_NULL_V(m_param, m_retval)                                     \
    if (unlikely(m_param == nullptr)) {                                        \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Parameter \"" _STR(m_param) "\" is null."                         \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * 3. Conditional error macros
 */

/**
 * Ensures `m_cond` is false.
 * On failure, it prints `m_msg` and the current function returns.
 * If checking for index bounds use `ERR_FAIL_INDEX_MSG` or
 * ERR_FAIL_UNSIGNED_INDEX_MSG instead.
 * If checking for null references, use `ERR_FAIL_NULL_MSG` instead.
 */
#define ERR_FAIL_COND_MSG(m_cond, m_msg)                                       \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond) "\" is true.",                         \
            m_msg                                                              \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/*
 * Always try using `ERR_FAIL_COND_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_COND(m_cond)                                                  \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond) "\" is true."                          \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Ensures `m_cond` is false.
 * On failure, it prints `m_msg` and the current function returns `m_retval`.
 * If checking for index bounds use `ERR_FAIL_INDEX_V_MSG` or
 * ERR_FAIL_UNSIGNED_INDEX_MSG instead.
 * If checking for null references, use `ERR_FAIL_NULL_V_MSG` instead.
 */
#define ERR_FAIL_COND_V_MSG(m_cond, m_retval, m_msg)                           \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond                                         \
            ) "\" is true. Returning: " _STR(m_retval),                        \
            m_msg                                                              \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_COND_V_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_COND_V(m_cond, m_retval)                                      \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond                                         \
            ) "\" is true. Returning: " _STR(m_retval)                         \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Ensures `m_cond` is false.
 * On failure, it prints `m_msg` and the current loop continues.
 */
#define ERR_CONTINUE_MSG(m_cond, m_msg)                                        \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond) "\" is true. Continuing.",             \
            m_msg                                                              \
        );                                                                     \
        continue;                                                              \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_CONTINUE_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_CONTINUE(m_cond)                                                   \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond) "\" is true. Continuing."              \
        );                                                                     \
        continue;                                                              \
    } else                                                                     \
        ((void)0)

/**
 * Ensures `m_cond` is false.
 * On failure, it prints `m_msg` and the current loop breaks.
 */
#define ERR_BREAK_MSG(m_cond, m_msg)                                           \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond) "\" is true. Breaking.",               \
            m_msg                                                              \
        );                                                                     \
        break;                                                                 \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_BREAK_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_BREAK(m_cond)                                                      \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Condition \"" _STR(m_cond) "\" is true. Breaking."                \
        );                                                                     \
        break;                                                                 \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_COND_MSG`, `ERR_FAIL_COND_V_MSG`, `ERR_CONTINUE_MSG` or
 * `ERR_BREAK_MSG`.
 * Only use this macro if the error is unrecoverable.
 * Ensures `m_cond` is false.
 * On failure, it prints `m_msg` and the application crashes.
 */
#define CRASH_COND_MSG(m_cond, m_msg)                                          \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "FATAL: Condition \"" _STR(m_cond) "\" is true.",                  \
            m_msg                                                              \
        );                                                                     \
        GENERATE_TRAP();                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_COND_MSG`, `ERR_FAIL_COND_V_MSG`, `ERR_CONTINUE_MSG` or
 * `ERR_BREAK_MSG`.
 * If the error is unrecoverable, try using `CRASH_COND_MSG`.
 */
#define CRASH_COND(m_cond)                                                     \
    if (unlikely(m_cond)) {                                                    \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "FATAL: Condition \"" _STR(m_cond) "\" is true."                   \
        );                                                                     \
        GENERATE_TRAP();                                                       \
    } else                                                                     \
        ((void)0)

/**
 * 4. Generic error macros
 */

/**
 * Try using `ERR_FAIL_COND_MSG`.
 * Only use this macro if more complex error detection or recovery is required.
 * Prints `m_msg`, and the current function returns.
 */
#define ERR_FAIL_MSG(m_msg)                                                    \
    if (true) {                                                                \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Method/function failed.",                                         \
            m_msg                                                              \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL()                                                             \
    if (true) {                                                                \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Method/function failed."                                          \
        );                                                                     \
        return;                                                                \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_COND_V_MSG`.
 * Only use this macro if more complex error detection or recovery is required.
 * Prints `m_msg`, and the current function returns `m_retval`.
 */
#define ERR_FAIL_V_MSG(m_retval, m_msg)                                        \
    if (true) {                                                                \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Method/function failed. Returning: " _STR(m_retval),              \
            m_msg                                                              \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `ERR_FAIL_V_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define ERR_FAIL_V(m_retval)                                                   \
    if (true) {                                                                \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "Method/function failed. Returning: " _STR(m_retval)               \
        );                                                                     \
        return m_retval;                                                       \
    } else                                                                     \
        ((void)0)

/**
 * Try using `ERR_FAIL_COND_MSG`, `ERR_FAIL_COND_V_MSG`, `ERR_CONTINUE_MSG` or
 * `ERR_BREAK_MSG`.
 * Only use this macro at the start of a function that has not been implemented
 * yet, or if more complex error detection or recovery is required.
 * Prints `m_msg`.
 */
#define ERR_PRINT(m_msg)                                                       \
    _err_print_error(__FUNCTION__, __FILE__, __LINE__, m_msg)

/**
 * Prints `m_msg` once during the application lifetime.
 */
#define ERR_PRINT_ONCE(m_msg)                                                  \
    if (true) {                                                                \
        static bool first_print = true;                                        \
        if (first_print) {                                                     \
            _err_print_error(__FUNCTION__, __FILE__, __LINE__, m_msg);         \
            first_print = false;                                               \
        }                                                                      \
    } else                                                                     \
        ((void)0)

/**
 * 5. Warning message macros
 */

/**
 * Warns that the current function is deprecated and prints `m_msg`.
 */
#define WARN_DEPRECATED_MSG(m_msg)                                             \
    if (true) {                                                                \
        static SafeFlag warning_shown;                                         \
        if (!warning_shown.is_set()) {                                         \
            _err_print_error(                                                  \
                __FUNCTION__,                                                  \
                __FILE__,                                                      \
                __LINE__,                                                      \
                "This method has been deprecated and will be removed in the "  \
                "future.",                                                     \
                m_msg,                                                         \
                ERR_HANDLER_WARNING                                            \
            );                                                                 \
            warning_shown.set();                                               \
        }                                                                      \
    } else                                                                     \
        ((void)0)

/**
 * Always try using `WARN_DEPRECATED_MSG` instead.
 * Only use this macro if there is no sensible error message.
 */
#define WARN_DEPRECATED                                                        \
    if (true) {                                                                \
        static SafeFlag warning_shown;                                         \
        if (!warning_shown.is_set()) {                                         \
            _err_print_error(                                                  \
                __FUNCTION__,                                                  \
                __FILE__,                                                      \
                __LINE__,                                                      \
                "This method has been deprecated and will be removed in the "  \
                "future.",                                                     \
                ERR_HANDLER_WARNING                                            \
            );                                                                 \
            warning_shown.set();                                               \
        }                                                                      \
    } else                                                                     \
        ((void)0)

/**
 * If warning about deprecated usage, use `WARN_DEPRECATED_MSG` instead.
 * Prints the warning message `m_msg`.
 */
#define WARN_PRINT(m_msg)                                                      \
    _err_print_error(                                                          \
        __FUNCTION__,                                                          \
        __FILE__,                                                              \
        __LINE__,                                                              \
        m_msg,                                                                 \
        ERR_HANDLER_WARNING                                                    \
    )

/**
 * Prints the warning message `m_msg` once during the application lifetime.
 */
#define WARN_PRINT_ONCE(m_msg)                                                 \
    if (true) {                                                                \
        static bool first_print = true;                                        \
        if (first_print) {                                                     \
            _err_print_error(                                                  \
                __FUNCTION__,                                                  \
                __FILE__,                                                      \
                __LINE__,                                                      \
                m_msg,                                                         \
                ERR_HANDLER_WARNING                                            \
            );                                                                 \
            first_print = false;                                               \
        }                                                                      \
    } else                                                                     \
        ((void)0)

/**
 * 6. Crash macros
 */

/**
 * Only use if the application should never reach this point.
 * Prints `m_msg`, and then the application crashes.
 */
#define CRASH_NOW_MSG(m_msg)                                                   \
    _err_print_error(                                                          \
        __FUNCTION__,                                                          \
        __FILE__,                                                              \
        __LINE__,                                                              \
        "FATAL: Method/function failed.",                                      \
        m_msg                                                                  \
    );                                                                         \
    GENERATE_TRAP()

/**
 * Do not use.
 * If the application should never reach this point, use CRASH_NOW_MSG(m_msg) to
 * explain why.
 */
#define CRASH_NOW()                                                            \
    _err_print_error(                                                          \
        __FUNCTION__,                                                          \
        __FILE__,                                                              \
        __LINE__,                                                              \
        "FATAL: Method/function failed."                                       \
    );                                                                         \
    GENERATE_TRAP()

/**
 * 7. Develpment macros
 * Only applied in development builds.
 */

#ifdef DEV_ENABLED
#define DEV_ASSERT(m_cond)                                                     \
    if (unlikely(!(m_cond))) {                                                 \
        _err_print_error(                                                      \
            __FUNCTION__,                                                      \
            __FILE__,                                                          \
            __LINE__,                                                          \
            "FATAL: DEV_ASSERT failed  \"" _STR(m_cond) "\" is false."         \
        );                                                                     \
        void _err_flush_stdout();                                              \
        GENERATE_TRAP();                                                       \
    } else                                                                     \
        ((void)0)
#else
#define DEV_ASSERT(m_cond)
#endif

#ifdef DEV_ENABLED
#define DEV_CHECK(m_cond)                                                      \
    if (unlikely(!(m_cond))) {                                                 \
        ERR_PRINT("DEV_CHECK failed  \"" _STR(m_cond) "\" is false.");         \
    } else                                                                     \
        ((void)0)
#else
#define DEV_CHECK(m_cond)
#endif

#ifdef DEV_ENABLED
#define DEV_CHECK_ONCE(m_cond)                                                 \
    if (unlikely(!(m_cond))) {                                                 \
        ERR_PRINT_ONCE("DEV_CHECK_ONCE failed  \"" _STR(m_cond) "\" is false." \
        );                                                                     \
    } else                                                                     \
        ((void)0)
#else
#define DEV_CHECK_ONCE(m_cond)
#endif

#endif // ERROR_MACROS_H
