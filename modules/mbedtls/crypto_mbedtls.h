// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CRYPTO_MBEDTLS_H
#define CRYPTO_MBEDTLS_H

#include "core/crypto/crypto.h"
#include "core/resource.h"

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ssl.h>

class CryptoMbedTLS;
class SSLContextMbedTLS;

class CryptoKeyMbedTLS : public CryptoKey {
private:
    mbedtls_entropy_context entropy_context;
    mbedtls_ctr_drbg_context ctr_drbg_context;
    mbedtls_pk_context pkey;
    int locks        = 0;
    bool public_only = true;

public:
    static CryptoKey* create();

    static void make_default() {
        CryptoKey::_create = create;
    }

    static void finalize() {
        CryptoKey::_create = nullptr;
    }

    virtual Error load(String p_path, bool p_public_only);
    virtual Error save(String p_path, bool p_public_only);
    virtual String save_to_string(bool p_public_only);
    virtual Error load_from_string(String p_string_key, bool p_public_only);

    virtual bool is_public_only() const {
        return public_only;
    };

    CryptoKeyMbedTLS();
    ~CryptoKeyMbedTLS();

    _FORCE_INLINE_ void lock() {
        locks++;
    }

    _FORCE_INLINE_ void unlock() {
        locks--;
    }

    friend class CryptoMbedTLS;
    friend class SSLContextMbedTLS;
};

class X509CertificateMbedTLS : public X509Certificate {
private:
    mbedtls_x509_crt cert;
    int locks;

public:
    static X509Certificate* create();

    static void make_default() {
        X509Certificate::_create = create;
    }

    static void finalize() {
        X509Certificate::_create = nullptr;
    }

    virtual Error load(String p_path);
    virtual Error load_from_memory(const uint8_t* p_buffer, int p_len);
    virtual Error save(String p_path);

    X509CertificateMbedTLS() {
        mbedtls_x509_crt_init(&cert);
        locks = 0;
    }

    ~X509CertificateMbedTLS() {
        mbedtls_x509_crt_free(&cert);
    }

    _FORCE_INLINE_ void lock() {
        locks++;
    }

    _FORCE_INLINE_ void unlock() {
        locks--;
    }

    friend class CryptoMbedTLS;
    friend class SSLContextMbedTLS;
};

class HMACContextMbedTLS : public HMACContext {
private:
    HashingContext::HashType hash_type;
    int hash_len = 0;
    void* ctx    = nullptr;

public:
    static HMACContext* create();

    static void make_default() {
        HMACContext::_create = create;
    }

    static void finalize() {
        HMACContext::_create = nullptr;
    }

    static bool is_md_type_allowed(mbedtls_md_type_t p_md_type);

    virtual Error start(
        HashingContext::HashType p_hash_type,
        PoolByteArray p_key
    );
    virtual Error update(PoolByteArray p_data);
    virtual PoolByteArray finish();

    HMACContextMbedTLS() {}

    ~HMACContextMbedTLS();
};

class CryptoMbedTLS : public Crypto {
private:
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    static X509CertificateMbedTLS* default_certs;

public:
    static Crypto* create();
    static void initialize_crypto();
    static void finalize_crypto();
    static X509CertificateMbedTLS* get_default_certificates();
    static void load_default_certificates(String p_path);
    static mbedtls_md_type_t md_type_from_hashtype(
        HashingContext::HashType p_hash_type,
        int& r_size
    );

    virtual PoolByteArray generate_random_bytes(int p_bytes);
    virtual Ref<CryptoKey> generate_rsa(int p_bytes);
    virtual Ref<X509Certificate> generate_self_signed_certificate(
        Ref<CryptoKey> p_key,
        String p_issuer_name,
        String p_not_before,
        String p_not_after
    );
    virtual Vector<uint8_t> sign(
        HashingContext::HashType p_hash_type,
        Vector<uint8_t> p_hash,
        Ref<CryptoKey> p_key
    );
    virtual bool verify(
        HashingContext::HashType p_hash_type,
        Vector<uint8_t> p_hash,
        Vector<uint8_t> p_signature,
        Ref<CryptoKey> p_key
    );
    virtual Vector<uint8_t> encrypt(
        Ref<CryptoKey> p_key,
        Vector<uint8_t> p_plaintext
    );
    virtual Vector<uint8_t> decrypt(
        Ref<CryptoKey> p_key,
        Vector<uint8_t> p_ciphertext
    );

    CryptoMbedTLS();
    ~CryptoMbedTLS();
};

#endif // CRYPTO_MBEDTLS_H
