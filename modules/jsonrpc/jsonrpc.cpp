// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "jsonrpc.h"

#include "core/array.h"
#include "core/dictionary.h"
#include "core/io/json.h"
#include "core/ustring.h"
#include "core/variant.h"

static Array get_arguments(const Dictionary& dictionary) {
    if (!dictionary.has("params")) {
        return {};
    }
    Variant params = dictionary["params"];
    if (params.get_type() == Variant::ARRAY) {
        return params;
    }
    Array result;
    result.push_back(params);
    return result;
}

Dictionary JSONRPC::make_notification(
    const String& method,
    const Variant& params
) const {
    Dictionary notification;
    notification["jsonrpc"] = "2.0";
    notification["method"]  = method;
    notification["params"]  = params;
    return notification;
}

Dictionary JSONRPC::make_request(
    const String& method,
    const Variant& params,
    const Variant& id
) const {
    Dictionary request;
    request["jsonrpc"] = "2.0";
    request["method"]  = method;
    request["params"]  = params;
    request["id"]      = id;
    return request;
}

Dictionary JSONRPC::make_response(const Variant& value, const Variant& id)
    const {
    Dictionary response;
    response["jsonrpc"] = "2.0";
    response["id"]      = id;
    response["result"]  = value;
    return response;
}

Dictionary JSONRPC::make_response_error(
    const int code,
    const String& message,
    const Variant& id
) const {
    Dictionary error;
    error["code"]    = code;
    error["message"] = message;
    Dictionary response_error;
    response_error["jsonrpc"] = "2.0";
    response_error["error"]   = error;
    response_error["id"]      = id;
    return response_error;
}

Variant JSONRPC::process_action(const Variant& request, const bool) {
    if (request.get_type() == Variant::ARRAY) {
        Array requests = request;
        if (requests.empty()) {
            return make_response_error(INVALID_REQUEST, "Invalid Request");
        }
        Array results;
        for (int i = 0; i < requests.size(); i++) {
            const Variant& this_request = requests[i];
            results.push_back(process_action(this_request));
        }
        return results;
    }
    if (request.get_type() != Variant::DICTIONARY) {
        return make_response_error(INVALID_REQUEST, "Invalid Request");
    }

    const Dictionary dictionary = request;
    String method               = dictionary.get("method", "");
    if (method.begins_with("$/")) {
        return {};
    }
    const Array arguments = get_arguments(dictionary);
    const Variant id      = dictionary.get("id", Variant());
    Object* object        = this;
    if (method_scopes.has(method.get_base_dir())) {
        object = method_scopes[method.get_base_dir()];
        method = method.get_file();
    }
    if (object == nullptr || !object->has_method(method)) {
        return make_response_error(
            METHOD_NOT_FOUND,
            "Method not found: " + method,
            id
        );
    }

    const Variant result = object->callv(method, arguments);
    if (id.get_type() == Variant::NIL) {
        return {};
    }
    return make_response(result, id);
}

String JSONRPC::process_string(const String& request_string) {
    if (request_string.empty()) {
        return {};
    }

    Variant result;
    Variant request;
    String error_message;
    int error_line;
    if (JSON::parse(request_string, request, error_message, error_line) != OK) {
        result = make_response_error(
            PARSE_ERROR,
            String("Parse error: ") + error_message + " at line "
                + itos(error_line)
        );
    } else {
        result = process_action(request);
    }

    if (result.get_type() == Variant::NIL) {
        return {};
    }
    return JSON::print(result);
}

void JSONRPC::set_scope(const String& scope, Object* object) {
    method_scopes[scope] = object;
}

void JSONRPC::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("make_notification", "method", "params"),
        &JSONRPC::make_notification
    );
    ClassDB::bind_method(
        D_METHOD("make_request", "method", "params", "id"),
        &JSONRPC::make_request
    );
    ClassDB::bind_method(
        D_METHOD("make_response", "result", "id"),
        &JSONRPC::make_response
    );
    ClassDB::bind_method(
        D_METHOD("make_response_error", "code", "message", "id"),
        &JSONRPC::make_response_error,
        DEFVAL(Variant())
    );
    ClassDB::bind_method(
        D_METHOD("process_action", "request", "unused"),
        &JSONRPC::process_action,
        DEFVAL(true)
    );
    ClassDB::bind_method(
        D_METHOD("process_string", "request_string"),
        &JSONRPC::process_string
    );
    ClassDB::bind_method(
        D_METHOD("set_scope", "scope", "target"),
        &JSONRPC::set_scope
    );

    BIND_ENUM_CONSTANT(PARSE_ERROR);
    BIND_ENUM_CONSTANT(INVALID_REQUEST);
    BIND_ENUM_CONSTANT(METHOD_NOT_FOUND);
    BIND_ENUM_CONSTANT(INVALID_PARAMS);
    BIND_ENUM_CONSTANT(INTERNAL_ERROR);
}
