// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CRYPTO_CORE_H
#define CRYPTO_CORE_H

#include "core/reference.h"

class CryptoCore {
public:
    class MD5Context {
    private:
        void* ctx; // To include, or not to include...

    public:
        MD5Context();
        ~MD5Context();

        Error start();
        Error update(const uint8_t* p_src, size_t p_len);
        Error finish(unsigned char r_hash[16]);
    };

    class SHA1Context {
    private:
        void* ctx; // To include, or not to include...

    public:
        SHA1Context();
        ~SHA1Context();

        Error start();
        Error update(const uint8_t* p_src, size_t p_len);
        Error finish(unsigned char r_hash[20]);
    };

    class SHA256Context {
    private:
        void* ctx; // To include, or not to include...

    public:
        SHA256Context();
        ~SHA256Context();

        Error start();
        Error update(const uint8_t* p_src, size_t p_len);
        Error finish(unsigned char r_hash[32]);
    };

    class AESContext {
    private:
        void* ctx; // To include, or not to include...

    public:
        AESContext();
        ~AESContext();

        Error set_encode_key(const uint8_t* p_key, size_t p_bits);
        Error set_decode_key(const uint8_t* p_key, size_t p_bits);
        Error encrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]);
        Error decrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]);
        Error encrypt_cbc(
            size_t p_length,
            uint8_t r_iv[16],
            const uint8_t* p_src,
            uint8_t* r_dst
        );
        Error decrypt_cbc(
            size_t p_length,
            uint8_t r_iv[16],
            const uint8_t* p_src,
            uint8_t* r_dst
        );
    };

    static String b64_encode_str(const uint8_t* p_src, int p_src_len);
    static Error b64_encode(
        uint8_t* r_dst,
        int p_dst_len,
        size_t* r_len,
        const uint8_t* p_src,
        int p_src_len
    );
    static Error b64_decode(
        uint8_t* r_dst,
        int p_dst_len,
        size_t* r_len,
        const uint8_t* p_src,
        int p_src_len
    );

    static Error md5(
        const uint8_t* p_src,
        int p_src_len,
        unsigned char r_hash[16]
    );
    static Error sha1(
        const uint8_t* p_src,
        int p_src_len,
        unsigned char r_hash[20]
    );
    static Error sha256(
        const uint8_t* p_src,
        int p_src_len,
        unsigned char r_hash[32]
    );
};
#endif // CRYPTO_CORE_H
