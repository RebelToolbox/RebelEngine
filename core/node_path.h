// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef NODE_PATH_H
#define NODE_PATH_H

#include "core/string_name.h"
#include "core/ustring.h"

class NodePath {
    struct Data {
        SafeRefCount refcount;
        Vector<StringName> path;
        Vector<StringName> subpath;
        StringName concatenated_subpath;
        bool absolute;
        bool has_slashes;
        mutable bool hash_cache_valid;
        mutable uint32_t hash_cache;
    };

    mutable Data* data;
    void unref();

    void _update_hash_cache() const;

public:
    _FORCE_INLINE_ StringName get_sname() const {
        if (data && data->path.size() == 1 && data->subpath.empty()) {
            return data->path[0];
        } else {
            return operator String();
        }
    }

    bool is_absolute() const;
    int get_name_count() const;
    StringName get_name(int p_idx) const;
    int get_subname_count() const;
    StringName get_subname(int p_idx) const;
    Vector<StringName> get_names() const;
    Vector<StringName> get_subnames() const;
    StringName get_concatenated_subnames() const;

    NodePath rel_path_to(const NodePath& p_np) const;
    NodePath get_as_property_path() const;

    void prepend_period();

    _FORCE_INLINE_ uint32_t hash() const {
        if (!data) {
            return 0;
        }
        if (!data->hash_cache_valid) {
            _update_hash_cache();
        }
        return data->hash_cache;
    }

    operator String() const;
    bool is_empty() const;

    bool operator==(const NodePath& p_path) const;
    bool operator!=(const NodePath& p_path) const;
    void operator=(const NodePath& p_path);

    void simplify();
    NodePath simplified() const;

    NodePath(const Vector<StringName>& p_path, bool p_absolute);
    NodePath(
        const Vector<StringName>& p_path,
        const Vector<StringName>& p_subpath,
        bool p_absolute
    );
    NodePath(const NodePath& p_path);
    NodePath(const String& p_path);
    NodePath();
    ~NodePath();
};

#endif
