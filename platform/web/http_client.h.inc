// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

// HTTPClient's additional private members in the Web platform

Error make_request(
    Method p_method,
    const String& p_url,
    const Vector<String>& p_headers,
    const uint8_t* p_body,
    int p_body_len
);
static void _parse_headers(int p_len, const char** p_headers, void* p_ref);

int js_id      = 0;
// 64 KiB by default (favors fast download speeds at the cost of memory usage).
int read_limit = 65536;
Status status  = STATUS_DISCONNECTED;

String host;
int port     = -1;
bool use_tls = false;

int polled_response_code = 0;
Vector<String> response_headers;
Vector<uint8_t> response_buffer;

#ifdef DEBUG_ENABLED
uint64_t last_polling_frame = 0;
#endif
