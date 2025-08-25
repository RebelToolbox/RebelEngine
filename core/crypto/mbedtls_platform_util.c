// SPDX-FileCopyrightText: The Mbed TLS Contributors
//
// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later

// Extracted from Mbed TLS library/platform_util.c

#include <string.h>

static void* (*const volatile memset_func)(void*, int, size_t) = memset;

void mbedtls_platform_zeroize(void* buf, size_t len) {
    memset_func(buf, 0, len);
}
