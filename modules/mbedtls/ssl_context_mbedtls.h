// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SSL_CONTEXT_MBED_TLS_H
#define SSL_CONTEXT_MBED_TLS_H

#include "core/os/file_access.h"
#include "core/pool_vector.h"
#include "core/reference.h"
#include "crypto_mbedtls.h"

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/debug.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_cookie.h>

class SSLContextMbedTLS;

class CookieContextMbedTLS : public Reference {
    friend class SSLContextMbedTLS;

protected:
    bool inited;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_cookie_ctx cookie_ctx;

public:
    Error setup();
    void clear();

    CookieContextMbedTLS();
    ~CookieContextMbedTLS();
};

class SSLContextMbedTLS : public Reference {
protected:
    bool inited;

    static PoolByteArray _read_file(String p_path);

public:
    static void print_mbedtls_error(int p_ret);

    Ref<X509CertificateMbedTLS> certs;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;

    Ref<CookieContextMbedTLS> cookies;
    Ref<CryptoKeyMbedTLS> pkey;

    Error _setup(int p_endpoint, int p_transport, int p_authmode);
    Error init_server(
        int p_transport,
        int p_authmode,
        Ref<CryptoKeyMbedTLS> p_pkey,
        Ref<X509CertificateMbedTLS> p_cert,
        Ref<CookieContextMbedTLS> p_cookies = Ref<CookieContextMbedTLS>()
    );
    Error init_client(
        int p_transport,
        int p_authmode,
        Ref<X509CertificateMbedTLS> p_valid_cas
    );
    void clear();

    mbedtls_ssl_context* get_context();

    SSLContextMbedTLS();
    ~SSLContextMbedTLS();
};

#endif // SSL_CONTEXT_MBED_TLS_H
