// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef JSON_RPC_H
#define JSON_RPC_H

#include "core/map.h"
#include "core/object.h"

class Array;
class Dictionary;
class String;
class Variant;

class JSONRPC : public Object {
    GDCLASS(JSONRPC, Object)

public:
    enum ErrorCode {
        PARSE_ERROR      = -32700,
        INVALID_REQUEST  = -32600,
        METHOD_NOT_FOUND = -32601,
        INVALID_PARAMS   = -32602,
        INTERNAL_ERROR   = -32603,
    };

    Dictionary make_notification(const String& method, const Variant& params)
        const;
    Dictionary make_request(
        const String& method,
        const Variant& params,
        const Variant& id
    ) const;
    Dictionary make_response(const Variant& value, const Variant& id) const;
    Dictionary make_response_error(
        int code,
        const String& message,
        const Variant& id = Variant()
    ) const;

    Variant process_action(const Variant& request, bool unused = true);
    String process_string(const String& request_string);

    void set_scope(const String& scope, Object* object);

protected:
    static void _bind_methods();

private:
    Map<String, Object*> method_scopes;
};

VARIANT_ENUM_CAST(JSONRPC::ErrorCode);

#endif // JSON_RPC_H
