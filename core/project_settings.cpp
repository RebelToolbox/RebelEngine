// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "project_settings.h"

#include "core/bind/core_bind.h"
#include "core/core_string_names.h"
#include "core/io/file_access_network.h"
#include "core/io/file_access_pack.h"
#include "core/io/marshalls.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/keyboard.h"
#include "core/os/os.h"
#include "core/variant_parser.h"

#include <zlib.h>

bool operator<(const Setting& left, const Setting& right) {
    if (left.position == right.position) {
        return left.name < right.name;
    }
    return left.position < right.position;
}

ProjectSettings* ProjectSettings::singleton = nullptr;

ProjectSettings* ProjectSettings::get_singleton() {
    return singleton;
}

ProjectSettings::ProjectSettings() {
    singleton = this;

    Array events;
    Dictionary action;
    Ref<InputEventKey> key;
    Ref<InputEventJoypadButton> joyb;

    GLOBAL_DEF("application/config/name", "");
    GLOBAL_DEF("application/config/description", "");
    custom_property_info["application/config/description"] = PropertyInfo(
        Variant::STRING,
        "application/config/description",
        PROPERTY_HINT_MULTILINE_TEXT
    );
    GLOBAL_DEF("application/run/main_scene", "");
    custom_property_info["application/run/main_scene"] = PropertyInfo(
        Variant::STRING,
        "application/run/main_scene",
        PROPERTY_HINT_FILE,
        "*.tscn,*.scn,*.res"
    );
    GLOBAL_DEF("application/run/disable_stdout", false);
    GLOBAL_DEF("application/run/disable_stderr", false);
    GLOBAL_DEF_RST(
        "application/config/use_hidden_project_data_directory",
        true
    );
    GLOBAL_DEF("application/config/use_custom_user_dir", false);
    GLOBAL_DEF("application/config/custom_user_dir_name", "");
    GLOBAL_DEF("application/config/project_settings_override", "");
    GLOBAL_DEF("audio/default_bus_layout", "res://default_bus_layout.tres");
    custom_property_info["audio/default_bus_layout"] = PropertyInfo(
        Variant::STRING,
        "audio/default_bus_layout",
        PROPERTY_HINT_FILE,
        "*.tres"
    );

    PoolStringArray extensions = PoolStringArray();
    extensions.push_back("gd");
    if (Engine::get_singleton()->has_singleton("GodotSharp")) {
        extensions.push_back("cs");
    }
    extensions.push_back("gdshader");
    extensions.push_back("shader");

    GLOBAL_DEF("editor/main_run_args", "");

    GLOBAL_DEF("editor/search_in_file_extensions", extensions);
    custom_property_info["editor/search_in_file_extensions"] = PropertyInfo(
        Variant::POOL_STRING_ARRAY,
        "editor/search_in_file_extensions"
    );

    GLOBAL_DEF("editor/script_templates_search_path", "res://script_templates");
    custom_property_info["editor/script_templates_search_path"] = PropertyInfo(
        Variant::STRING,
        "editor/script_templates_search_path",
        PROPERTY_HINT_DIR
    );

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_ENTER);
    events.push_back(key);
    key.instance();
    key->set_scancode(KEY_KP_ENTER);
    events.push_back(key);
    key.instance();
    key->set_scancode(KEY_SPACE);
    events.push_back(key);
    joyb.instance();
    joyb->set_button_index(JOY_BUTTON_0);
    events.push_back(joyb);
    action["events"] = events;
    GLOBAL_DEF("input/ui_accept", action);
    input_presets.push_back("input/ui_accept");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_SPACE);
    events.push_back(key);
    joyb.instance();
    joyb->set_button_index(JOY_BUTTON_3);
    events.push_back(joyb);
    action["events"] = events;
    GLOBAL_DEF("input/ui_select", action);
    input_presets.push_back("input/ui_select");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_ESCAPE);
    events.push_back(key);
    joyb.instance();
    joyb->set_button_index(JOY_BUTTON_1);
    events.push_back(joyb);
    action["events"] = events;
    GLOBAL_DEF("input/ui_cancel", action);
    input_presets.push_back("input/ui_cancel");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_TAB);
    events.push_back(key);
    action["events"] = events;
    GLOBAL_DEF("input/ui_focus_next", action);
    input_presets.push_back("input/ui_focus_next");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_TAB);
    key->set_shift(true);
    events.push_back(key);
    action["events"] = events;
    GLOBAL_DEF("input/ui_focus_prev", action);
    input_presets.push_back("input/ui_focus_prev");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_LEFT);
    events.push_back(key);
    joyb.instance();
    joyb->set_button_index(JOY_DPAD_LEFT);
    events.push_back(joyb);
    action["events"] = events;
    GLOBAL_DEF("input/ui_left", action);
    input_presets.push_back("input/ui_left");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_RIGHT);
    events.push_back(key);
    joyb.instance();
    joyb->set_button_index(JOY_DPAD_RIGHT);
    events.push_back(joyb);
    action["events"] = events;
    GLOBAL_DEF("input/ui_right", action);
    input_presets.push_back("input/ui_right");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_UP);
    events.push_back(key);
    joyb.instance();
    joyb->set_button_index(JOY_DPAD_UP);
    events.push_back(joyb);
    action["events"] = events;
    GLOBAL_DEF("input/ui_up", action);
    input_presets.push_back("input/ui_up");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_DOWN);
    events.push_back(key);
    joyb.instance();
    joyb->set_button_index(JOY_DPAD_DOWN);
    events.push_back(joyb);
    action["events"] = events;
    GLOBAL_DEF("input/ui_down", action);
    input_presets.push_back("input/ui_down");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_PAGEUP);
    events.push_back(key);
    action["events"] = events;
    GLOBAL_DEF("input/ui_page_up", action);
    input_presets.push_back("input/ui_page_up");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_PAGEDOWN);
    events.push_back(key);
    action["events"] = events;
    GLOBAL_DEF("input/ui_page_down", action);
    input_presets.push_back("input/ui_page_down");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_HOME);
    events.push_back(key);
    action["events"] = events;
    GLOBAL_DEF("input/ui_home", action);
    input_presets.push_back("input/ui_home");

    action             = Dictionary();
    action["deadzone"] = Variant(0.5f);
    events             = Array();
    key.instance();
    key->set_scancode(KEY_END);
    events.push_back(key);
    action["events"] = events;
    GLOBAL_DEF("input/ui_end", action);
    input_presets.push_back("input/ui_end");

    custom_property_info["display/window/handheld/orientation"] = PropertyInfo(
        Variant::STRING,
        "display/window/handheld/orientation",
        PROPERTY_HINT_ENUM,
        "landscape,portrait,reverse_landscape,reverse_portrait,sensor_"
        "landscape,sensor_portrait,sensor"
    );
    custom_property_info["rendering/threads/thread_model"] = PropertyInfo(
        Variant::INT,
        "rendering/threads/thread_model",
        PROPERTY_HINT_ENUM,
        "Single-Unsafe,Single-Safe,Multi-Threaded"
    );
    custom_property_info["physics/2d/thread_model"] = PropertyInfo(
        Variant::INT,
        "physics/2d/thread_model",
        PROPERTY_HINT_ENUM,
        "Single-Unsafe,Single-Safe,Multi-Threaded"
    );
    custom_property_info
        ["rendering/quality/intended_usage/framebuffer_allocation"] =
            PropertyInfo(
                Variant::INT,
                "rendering/quality/intended_usage/framebuffer_allocation",
                PROPERTY_HINT_ENUM,
                "2D,2D Without Sampling,3D,3D Without Effects"
            );

    GLOBAL_DEF("rendering/quality/filters/sharpen_intensity", 0.0);
    custom_property_info["rendering/quality/filters/sharpen_intensity"] =
        PropertyInfo(
            Variant::REAL,
            "rendering/quality/filters/sharpen_intensity",
            PROPERTY_HINT_RANGE,
            "0,1"
        );

    GLOBAL_DEF("debug/settings/profiler/max_functions", 16384);
    custom_property_info["debug/settings/profiler/max_functions"] =
        PropertyInfo(
            Variant::INT,
            "debug/settings/profiler/max_functions",
            PROPERTY_HINT_RANGE,
            "128,65535,1"
        );

    GLOBAL_DEF(
        "compression/formats/zstd/long_distance_matching",
        Compression::zstd_long_distance_matching
    );
    custom_property_info["compression/formats/zstd/long_distance_matching"] =
        PropertyInfo(
            Variant::BOOL,
            "compression/formats/zstd/long_distance_matching"
        );
    GLOBAL_DEF(
        "compression/formats/zstd/compression_level",
        Compression::zstd_level
    );
    custom_property_info["compression/formats/zstd/compression_level"] =
        PropertyInfo(
            Variant::INT,
            "compression/formats/zstd/compression_level",
            PROPERTY_HINT_RANGE,
            "1,22,1"
        );
    GLOBAL_DEF(
        "compression/formats/zstd/window_log_size",
        Compression::zstd_window_log_size
    );
    custom_property_info["compression/formats/zstd/window_log_size"] =
        PropertyInfo(
            Variant::INT,
            "compression/formats/zstd/window_log_size",
            PROPERTY_HINT_RANGE,
            "10,30,1"
        );

    GLOBAL_DEF(
        "compression/formats/zlib/compression_level",
        Compression::zlib_level
    );
    custom_property_info["compression/formats/zlib/compression_level"] =
        PropertyInfo(
            Variant::INT,
            "compression/formats/zlib/compression_level",
            PROPERTY_HINT_RANGE,
            "-1,9,1"
        );

    GLOBAL_DEF(
        "compression/formats/gzip/compression_level",
        Compression::gzip_level
    );
    custom_property_info["compression/formats/gzip/compression_level"] =
        PropertyInfo(
            Variant::INT,
            "compression/formats/gzip/compression_level",
            PROPERTY_HINT_RANGE,
            "-1,9,1"
        );

    GLOBAL_DEF("android/modules", "");
}

ProjectSettings::~ProjectSettings() {
    singleton = nullptr;
}

bool ProjectSettings::has_setting(const String& name) const {
    _THREAD_SAFE_METHOD_
    return settings.has(name);
}

Variant ProjectSettings::get_setting(const String& name) const {
    return get(name);
}

void ProjectSettings::set_setting(const String& name, const Variant& value) {
    set(name, value);
}

int ProjectSettings::get_order(const String& name) const {
    ERR_FAIL_COND_V_MSG(
        !settings.has(name),
        -1,
        "Cannot get order of unknown project setting: " + name
    );
    return settings[name].position;
}

void ProjectSettings::set_order(const String& name, int position) {
    ERR_FAIL_COND_MSG(
        !settings.has(name),
        "Cannot set order of unknown project setting: " + name
    );
    settings[name].position = position;
}

void ProjectSettings::set_builtin_order(const String& name) {
    ERR_FAIL_COND_MSG(
        !settings.has(name),
        "Cannot set builtin order of unknown project setting: " + name
    );
    if (settings[name].position >= CUSTOM_SETTINGS_START) {
        settings[name].position = builtin_position++;
    }
}

bool ProjectSettings::get_ignore_value_in_docs(const String& name) const {
    ERR_FAIL_COND_V_MSG(
        !settings.has(name),
        false,
        "Cannot get ignore value in docs of unknown project setting: " + name
    );
#ifdef DEBUG_METHODS_ENABLED
    return settings[name].ignore_value_in_docs;
#else
    return false;
#endif
}

void ProjectSettings::set_ignore_value_in_docs(
    const String& name,
    bool ignore_in_docs
) {
    ERR_FAIL_COND_MSG(
        !settings.has(name),
        "Cannot set ignore value in docs of unknown project setting: " + name
    );
#ifdef DEBUG_METHODS_ENABLED
    settings[name].ignore_value_in_docs = ignore_in_docs;
#endif
}

Variant ProjectSettings::property_get_revert(const String& name) const {
    ERR_FAIL_COND_V_MSG(
        !settings.has(name),
        Variant(),
        "Cannot get revert value of unknown project setting: " + name
    );
    return settings[name].initial_value;
}

bool ProjectSettings::property_can_revert(const String& name) {
    ERR_FAIL_COND_V_MSG(
        !settings.has(name),
        false,
        "Cannot determine if we can revert unknown project setting: " + name
    );
    return settings[name].initial_value != settings[name].current_value;
}

String ProjectSettings::get_project_data_dir_name() const {
    return project_data_dir_name;
}

String ProjectSettings::get_project_data_path() const {
    return "res://" + project_data_dir_name;
}

String ProjectSettings::get_resource_path() const {
    return resource_path;
};

const Map<StringName, PropertyInfo>& ProjectSettings::get_custom_property_info(
) const {
    return custom_property_info;
}

uint64_t ProjectSettings::get_last_saved_time() const {
    return last_save_time;
}

List<String> ProjectSettings::get_input_presets() const {
    return input_presets;
}

Vector<String> ProjectSettings::get_optimizer_presets() const {
    List<PropertyInfo> property_info_list;
    ProjectSettings::get_singleton()->get_property_list(&property_info_list);
    Vector<String> optimizer_presets;
    for (auto E = property_info_list.front(); E; E = E->next()) {
        if (!E->get().name.begins_with("optimizer_presets/")) {
            continue;
        }
        optimizer_presets.push_back(E->get().name.get_slicec('/', 1));
    }
    optimizer_presets.sort();
    return optimizer_presets;
}

bool ProjectSettings::is_using_datapack() const {
    return using_datapack;
}

bool ProjectSettings::has_custom_feature(const String& feature) const {
    return custom_features.has(feature);
}

void ProjectSettings::clear(const String& name) {
    ERR_FAIL_COND_MSG(
        !settings.has(name),
        "Cannot clear unknown project setting: " + name
    );
    settings.erase(name);
}

void ProjectSettings::set_initial_value(
    const String& name,
    const Variant& value
) {
    ERR_FAIL_COND_MSG(
        !settings.has(name),
        "Cannot set initial value of unknown project setting: " + name
    );
    settings[name].initial_value = value;
}

void ProjectSettings::set_restart_if_changed(
    const String& name,
    bool restart_required
) {
    ERR_FAIL_COND_MSG(
        !settings.has(name),
        "Cannot set restart required of unknown project setting: " + name
    );
    settings[name].restart_if_changed = restart_required;
}

void ProjectSettings::set_disable_feature_overrides(bool disable) {
    disable_feature_overrides = disable;
}

void ProjectSettings::set_custom_property_info(
    const String& name,
    const PropertyInfo& property_info
) {
    ERR_FAIL_COND_MSG(
        !settings.has(name),
        "Cannot set custom property info of unknown project setting: " + name
    );
    custom_property_info[name]      = property_info;
    custom_property_info[name].name = name;
}

String ProjectSettings::globalize_path(const String& local_path) const {
    if (local_path.begins_with("res://")) {
        if (resource_path != "") {
            return local_path.replace("res:/", resource_path);
        };
        return local_path.replace("res://", "");
    } else if (local_path.begins_with("user://")) {
        String data_dir = OS::get_singleton()->get_user_data_dir();
        if (data_dir != "") {
            return local_path.replace("user:/", data_dir);
        };
        return local_path.replace("user://", "");
    }
    return local_path;
}

String ProjectSettings::localize_path(const String& global_path) const {
    if (resource_path.empty() || global_path.begins_with("res://")
        || global_path.begins_with("user://")
        || (global_path.is_abs_path() && !global_path.begins_with(resource_path)
        )) {
        return global_path.simplify_path();
    }

    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    String simple_path    = global_path.replace("\\", "/").simplify_path();
    if (dir_access->change_dir(simple_path) == OK) {
        String current_dir = dir_access->get_current_dir();
        current_dir        = current_dir.replace("\\", "/");
        memdelete(dir_access);

        // Ensure paths end with a '/'.
        const String res_path = resource_path.plus_file("");
        current_dir           = current_dir.plus_file("");

        if (current_dir.begins_with(res_path)) {
            return current_dir.replace_first(res_path, "res://");
        };
        return global_path;
    } else {
        memdelete(dir_access);
        // Global path does not exist.
        int last_dir_index = simple_path.find_last("/");
        if (last_dir_index == -1) {
            return "res://" + simple_path;
        };

        String parent_dir           = simple_path.substr(0, last_dir_index);
        String localized_parent_dir = localize_path(parent_dir);
        if (localized_parent_dir == "") {
            return "";
        };
        // Strip first '/' from 'simple_path' if 'parent_dir' ends with '/'.
        if (localized_parent_dir[localized_parent_dir.length() - 1] == '/') {
            last_dir_index += 1;
        }
        return localized_parent_dir + simple_path.substr(last_dir_index);
    }
}

Error ProjectSettings::setup(
    const String& path,
    const String& main_pack,
    bool try_parent_dirs
) {
    Error error = _setup(path, main_pack, try_parent_dirs);
    if (error == OK) {
        String custom_settings =
            GLOBAL_DEF("application/config/project_settings_override", "");
        if (custom_settings != "") {
            _load_text_settings(custom_settings);
        }
    }

    // Assign global variables dependent on project settings.
    if (GLOBAL_GET("application/config/use_hidden_project_data_directory")) {
        project_data_dir_name = "." + project_data_dir_name;
    }

    Compression::zstd_long_distance_matching =
        GLOBAL_GET("compression/formats/zstd/long_distance_matching");
    Compression::zstd_level =
        GLOBAL_GET("compression/formats/zstd/compression_level");
    Compression::zstd_window_log_size =
        GLOBAL_GET("compression/formats/zstd/window_log_size");
    Compression::zlib_level =
        GLOBAL_GET("compression/formats/zlib/compression_level");
    Compression::gzip_level =
        GLOBAL_GET("compression/formats/gzip/compression_level");

    return error;
}

Error ProjectSettings::save() {
    Error error = save_custom(resource_path.plus_file("project.rebel"));
    if (error == OK) {
        last_save_time = FileAccess::get_modified_time(
            resource_path.plus_file("project.rebel")
        );
    }
    return error;
}

Error ProjectSettings::save_custom(
    const String& settings_file,
    const CustomSettings& custom_settings,
    const Vector<String>& custom_features,
    bool merge_with_current
) {
    ERR_FAIL_COND_V_MSG(
        !(settings_file.ends_with(".rebel")
          || settings_file.ends_with(".binary")
          || settings_file.ends_with("override.cfg")),
        ERR_FILE_UNRECOGNIZED,
        "Unrecognized project settings file: " + settings_file
    );

    Set<Setting> settings_to_save;

    if (merge_with_current) {
        for (auto G = settings.front(); G; G = G->next()) {
            const String& setting_name = G->key();
            if (custom_settings.has(setting_name)) {
                continue;
            }

            Setting setting = G->get();
            if (setting.current_value == setting.initial_value) {
                continue;
            }
            if (setting.hide_from_editor) {
                continue;
            }
            setting.flags = PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE;
            settings_to_save.insert(setting);
        }
    }

    for (auto E = custom_settings.front(); E; E = E->next()) {
        const String& setting_name = E->key();
        const Variant& value       = E->get();
        Setting setting(setting_name, value, 0xFFFFFF);
        auto F = settings.find(setting_name);
        if (F) {
            setting.position = F->get().position;
        }
        setting.flags = PROPERTY_USAGE_STORAGE;
        settings_to_save.insert(setting);
    }

    Map<String, List<String>> grouped_settings;

    for (auto E = settings_to_save.front(); E; E = E->next()) {
        String section;
        String setting_name = E->get().name;

        int div = setting_name.find("/");
        if (div > 0) {
            section      = setting_name.substr(0, div);
            setting_name = setting_name.substr(div + 1);
        }
        grouped_settings[section].push_back(setting_name);
    }

    String features;
    for (int i = 0; i < custom_features.size(); i++) {
        if (i > 0) {
            features += ",";
        }
        String feature  = custom_features[i].strip_edges().replace("\"", "");
        features       += feature;
    }

    if (settings_file.ends_with(".binary")) {
        return _save_binary_settings(
            settings_file,
            grouped_settings,
            custom_settings,
            features
        );
    } else {
        return _save_text_settings(
            settings_file,
            grouped_settings,
            custom_settings,
            features
        );
    }
}

void ProjectSettings::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("add_property_info", "hint"),
        &ProjectSettings::_add_property_info
    );
    ClassDB::bind_method(D_METHOD("clear", "name"), &ProjectSettings::clear);
    ClassDB::bind_method(
        D_METHOD("get_order", "name"),
        &ProjectSettings::get_order
    );
    ClassDB::bind_method(
        D_METHOD("get_setting", "name"),
        &ProjectSettings::get_setting
    );
    ClassDB::bind_method(
        D_METHOD("globalize_path", "path"),
        &ProjectSettings::globalize_path
    );
    ClassDB::bind_method(
        D_METHOD("has_setting", "name"),
        &ProjectSettings::has_setting
    );
    ClassDB::bind_method(
        D_METHOD("load_resource_pack", "pack", "replace_files", "offset"),
        &ProjectSettings::_load_resource_pack,
        DEFVAL(true),
        DEFVAL(0)
    );
    ClassDB::bind_method(
        D_METHOD("localize_path", "path"),
        &ProjectSettings::localize_path
    );
    ClassDB::bind_method(
        D_METHOD("property_can_revert", "name"),
        &ProjectSettings::property_can_revert
    );
    ClassDB::bind_method(
        D_METHOD("property_get_revert", "name"),
        &ProjectSettings::property_get_revert
    );
    ClassDB::bind_method(D_METHOD("save"), &ProjectSettings::save);
    ClassDB::bind_method(
        D_METHOD("save_custom", "file"),
        &ProjectSettings::_save_custom_bnd
    );
    ClassDB::bind_method(
        D_METHOD("set_initial_value", "name", "value"),
        &ProjectSettings::set_initial_value
    );
    ClassDB::bind_method(
        D_METHOD("set_order", "name", "position"),
        &ProjectSettings::set_order
    );
    ClassDB::bind_method(
        D_METHOD("set_setting", "name", "value"),
        &ProjectSettings::set_setting
    );
}

bool ProjectSettings::_get(const StringName& name, Variant& r_value) const {
    _THREAD_SAFE_METHOD_

    StringName override_name = name;
    if (!disable_feature_overrides && feature_overrides.has(name)) {
        override_name = feature_overrides[name];
    }
    ERR_FAIL_COND_V_MSG(
        !settings.has(override_name),
        false,
        "Unknown project setting: " + override_name
    );
    r_value = settings[override_name].current_value;
    return true;
}

bool ProjectSettings::_set(const StringName& name, const Variant& value) {
    _THREAD_SAFE_METHOD_

    if (value.get_type() == Variant::NIL) {
        settings.erase(name);
        return true;
    }

    if (name == CoreStringNames::get_singleton()->_custom_features) {
        Vector<String> custom_feature_array = String(value).split(",");
        for (int i = 0; i < custom_feature_array.size(); i++) {
            custom_features.insert(custom_feature_array[i]);
        }
        return true;
    }

    if (!disable_feature_overrides) {
        int dot = name.operator String().find(".");
        if (dot != -1) {
            Vector<String> parts = name.operator String().split(".");
            for (int i = 1; i < parts.size(); i++) {
                String feature = parts[i].strip_edges();
                if (OS::get_singleton()->has_feature(feature)
                    || custom_features.has(feature)) {
                    feature_overrides[parts[0]] = name;
                    break;
                }
            }
        }
    }

    if (settings.has(name)) {
        settings[name].current_value = value;
    } else {
        settings[name] = Setting(name, value, custom_position++);
    }
    return true;
}

void ProjectSettings::_get_property_list(List<PropertyInfo>* r_list) const {
    _THREAD_SAFE_METHOD_

    for (auto E = settings.front(); E; E = E->next()) {
        Setting setting = E->get();
        if (setting.hide_from_editor) {
            continue;
        }

        if (setting.name.begins_with("input/")
            || setting.name.begins_with("import/")
            || setting.name.begins_with("export/")
            || setting.name.begins_with("/remap")
            || setting.name.begins_with("/locale")
            || setting.name.begins_with("/autoload")) {
            setting.flags = PROPERTY_USAGE_STORAGE;
        } else {
            setting.flags = PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE;
        }
        if (setting.restart_if_changed) {
            setting.flags |= PROPERTY_USAGE_RESTART_IF_CHANGED;
        }

        String property_info_name = setting.name;
        int dot                   = setting.name.find(".");
        if (dot > 0) {
            property_info_name = setting.name.substr(0, dot);
        }

        if (custom_property_info.has(property_info_name)) {
            PropertyInfo pi = custom_property_info[property_info_name];
            pi.name         = setting.name;
            pi.usage        = setting.flags;
            r_list->push_back(pi);
        } else {
            r_list->push_back(PropertyInfo(
                setting.current_value.get_type(),
                setting.name,
                PROPERTY_HINT_NONE,
                "",
                setting.flags
            ));
        }
    }
}

void ProjectSettings::_add_property_info(const Dictionary& dict_info) {
    ERR_FAIL_COND_MSG(!dict_info.has("name"), "Dictionary has no key \"name\"");
    ERR_FAIL_COND_MSG(!dict_info.has("type"), "Dictionary has no key \"type\"");

    const String& name        = dict_info["name"];
    const Variant::Type& type = Variant::Type(dict_info["type"].operator int());
    ERR_FAIL_COND_MSG(!settings.has(name), "Unknown setting: " + name);
    ERR_FAIL_INDEX_MSG(type, Variant::VARIANT_MAX, "Unknown Variant type");

    PropertyHint hint = PROPERTY_HINT_NONE;
    String hint_string;
    if (dict_info.has("hint")) {
        hint = PropertyHint(dict_info["hint"].operator int());
    }
    if (dict_info.has("hint_string")) {
        hint_string = dict_info["hint_string"];
    }

    const PropertyInfo property_info(type, name, hint, hint_string);
    set_custom_property_info(name, property_info);
}

void ProjectSettings::_upgrade_settings(int current_version) {
    if (current_version <= 3) {
        // Convert input actions from an array to a dictionary.
        for (auto E = settings.front(); E; E = E->next()) {
            Variant value = E->get().current_value;
            if (String(E->key()).begins_with("input/")
                && value.get_type() == Variant::ARRAY) {
                Array array = value;
                Dictionary action;
                action["deadzone"]     = Variant(0.5f);
                action["events"]       = array;
                E->get().current_value = action;
            }
        }
    }
}

Error ProjectSettings::_setup(
    const String& path,
    const String& main_pack,
    bool try_parent_dirs
) {
    // If using a network client, only load the settings.
    if (FileAccessNetworkClient::get_singleton()) {
        return _load_settings();
    }

    // If defined, use the user defined main pack.
    if (main_pack != "") {
        ERR_FAIL_COND_V_MSG(
            !_load_resource_pack(main_pack),
            ERR_CANT_OPEN,
            "Cannot open resource pack: " + main_pack
        );
        return _load_settings("res://", main_pack.get_base_dir());
    }

    const String executable    = OS::get_singleton()->get_executable_path();
    const String exec_dir      = executable.get_base_dir();
    const String exec_filename = executable.get_file();
    const String exec_basename = exec_filename.get_basename();

    // Try loading the resource pack from the executable.
    // This is the usual case when distributing a Rebel game.
    if (executable != "" && _load_resource_pack(executable)) {
        return _load_settings("res://", exec_dir);
    }

#ifdef MACOS_ENABLED
    // Try loading the resource pack from macOS .app bundle resources.
    const String bundle_dir = OS::get_singleton()->get_bundle_resource_dir();
    if (_load_resource_pack(bundle_dir.plus_file(exec_basename + ".pck"))
        || _load_resource_pack(bundle_dir.plus_file(exec_filename + ".pck"))) {
        return _load_settings("res://", exec_dir);
    }
#endif

    // Try loading the resource pack from the executable or working directories.
    if (_load_resource_pack(exec_dir.plus_file(exec_basename + ".pck"))
        || _load_resource_pack(exec_dir.plus_file(exec_filename + ".pck"))
        || _load_resource_pack(exec_basename + ".pck")
        || _load_resource_pack(exec_filename + ".pck")) {
        return _load_settings("res://", exec_dir);
    }

    // If the OS has a specified resource directory, use that.
    const String os_resource_dir = OS::get_singleton()->get_resource_dir();
    if (os_resource_dir != "") {
        resource_path = os_resource_dir.replace("\\", "/");
        // Trim trailing '/'.
        if (resource_path[resource_path.length() - 1] == '/') {
            resource_path = resource_path.substr(0, resource_path.length() - 1);
        }
        return _load_settings();
    }

    // Try the specified `path`, and, if requested, try parent directories.
    DirAccess* dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    ERR_FAIL_COND_V_MSG(
        dir_access->change_dir(path) != OK,
        ERR_CANT_CREATE,
        "Cannot access " + path
    );
    String current_dir;
    Error error;
    bool found = false;
    while (!found && current_dir != dir_access->get_current_dir()) {
        current_dir = dir_access->get_current_dir();
        error       = _load_settings(current_dir, current_dir);
        if (error == OK) {
            found = true;
        }
        if (try_parent_dirs) {
            dir_access->change_dir("..");
        }
    }
    memdelete(dir_access);

    resource_path = current_dir;
    resource_path = resource_path.replace("\\", "/");
    // Trim trailing '/'.
    if (resource_path.length()
        && resource_path[resource_path.length() - 1] == '/') {
        resource_path = resource_path.substr(0, resource_path.length() - 1);
    }

    return error;
}

bool ProjectSettings::_load_resource_pack(
    const String& pack_file,
    bool replace_files,
    int offset
) {
    if (PackedData::get_singleton()->is_disabled()) {
        return false;
    }

    Error error =
        PackedData::get_singleton()->add_pack(pack_file, replace_files, offset);
    if (error != OK) {
        return false;
    }

    // If data.pck is found, all directory access will be from here.
    DirAccess::make_default<DirAccessPack>(DirAccess::ACCESS_RESOURCES);
    using_datapack = true;
    return true;
}

Error ProjectSettings::_load_binary_settings(const String& settings_file) {
    Error file_access_error;
    FileAccess* file_access =
        FileAccess::open(settings_file, FileAccess::READ, &file_access_error);
    if (!file_access) {
        return file_access_error;
    }

    uint8_t header[4];
    file_access->get_buffer(header, 4);
    if (header[0] != 'E' || header[1] != 'C' || header[2] != 'F'
        || header[3] != 'G') {
        memdelete(file_access);
        ERR_FAIL_V_MSG(
            ERR_FILE_CORRUPT,
            "Corrupted header in binary project.binary (not ECFG)"
        );
    }

    uint32_t length = file_access->get_32();
    for (uint32_t i = 0; i < length; i++) {
        uint32_t key_length = file_access->get_32();
        CharString char_string;
        char_string.resize(key_length + 1);
        char_string[key_length] = 0;
        file_access->get_buffer((uint8_t*)char_string.ptr(), key_length);
        String key;
        key.parse_utf8(char_string.ptr());

        uint32_t value_length = file_access->get_32();
        Vector<uint8_t> value_bytes;
        value_bytes.resize(value_length);
        file_access->get_buffer(value_bytes.ptrw(), value_length);
        Variant value;
        Error error = decode_variant(
            value,
            value_bytes.ptr(),
            value_bytes.size(),
            nullptr,
            true
        );
        ERR_CONTINUE_MSG(error != OK, "Error decoding setting: " + key);
        set(key, value);
    }

    file_access->close();
    memdelete(file_access);
    return OK;
}

Error ProjectSettings::_load_text_settings(const String& settings_file) {
    Error file_access_error;
    FileAccess* file_access =
        FileAccess::open(settings_file, FileAccess::READ, &file_access_error);
    if (!file_access) {
        return file_access_error;
    }

    String section;
    int config_version = 0;

    VariantParser::StreamFile stream;
    stream.f = file_access;
    int line = 0;
    String error_text;
    Error error = OK;

    while (error == OK) {
        VariantParser::Tag tag;
        String key;
        Variant value;

        error = VariantParser::parse_tag_assign_eof(
            &stream,
            line,
            error_text,
            tag,
            key,
            value,
            nullptr,
            true
        );

        if (key != String()) {
            if (section == String() && key == "config_version") {
                config_version = value;
                if (config_version > CONFIG_VERSION) {
                    error = ERR_FILE_UNRECOGNIZED;
                }
            } else {
                if (section == String()) {
                    set(key, value);
                } else {
                    set(section + "/" + key, value);
                }
            }
        } else if (tag.name != String()) {
            section = tag.name;
        }
    }
    memdelete(file_access);

    switch (error) {
        case ERR_FILE_EOF: {
            // Not an error: Settings file loaded successfully.
            _upgrade_settings(config_version);
            last_save_time = FileAccess::get_modified_time(
                resource_path.plus_file("project.rebel")
            );
            error = OK;
        } break;
        case ERR_FILE_UNRECOGNIZED: {
            ERR_PRINT(vformat(
                "Unable to load project settings file '%s'.\n"
                "It's `config_version` (%d) is from a more recent and "
                "incompatible version of the engine.\n"
                "Expected config version: %d or lower",
                settings_file,
                config_version,
                CONFIG_VERSION
            ));
        } break;
        default: {
            ERR_PRINT(vformat(
                "Error parsing settings file '%s' at line %d: %s",
                settings_file,
                line,
                error_text
            ));
        }
    }
    return error;
}

Error ProjectSettings::_load_settings(
    const String& settings_dir,
    const String& override_dir
) {
    const String& binary_file   = settings_dir.plus_file("project.binary");
    const String& text_file     = settings_dir.plus_file("project.rebel");
    const String& override_file = override_dir.plus_file("override.cfg");

    // First, try to load the binary settings file.
    Error error = _load_binary_settings(binary_file);
    if (error != OK && error != ERR_FILE_NOT_FOUND) {
        // If the file exists but can't be loaded, print an error.
        ERR_PRINT(vformat(
            "Error(%d): Couldn't load file %s",
            binary_file,
            itos(error)
        ));
    }

    // If binary settings file fails, try to load the text settings file.
    if (error != OK) {
        error = _load_text_settings(text_file);
    }

    // If settings were loaded, optionally load the override settings file.
    if (error == OK) {
        _load_text_settings(override_file);
    }

    return error;
}

Error ProjectSettings::_save_binary_settings(
    const String& settings_file,
    const Map<String, List<String>>& grouped_settings,
    const CustomSettings& custom_settings,
    const String& custom_features
) {
    Error error;
    FileAccess* file =
        FileAccess::open(settings_file, FileAccess::WRITE, &error);
    ERR_FAIL_COND_V_MSG(
        error != OK,
        error,
        "Cannot write project settings file: " + settings_file
    );

    uint8_t header[4] = {'E', 'C', 'F', 'G'};
    file->store_buffer(header, 4);

    // Store the number of sections; add one for custom features.
    int section_count = 0;
    if (custom_features != String()) {
        section_count++;
    }
    for (auto E = grouped_settings.front(); E; E = E->next()) {
        for (auto F = E->get().front(); F; F = F->next()) {
            section_count++;
        }
    }
    file->store_32(section_count);

    if (custom_features != String()) {
        String key = CoreStringNames::get_singleton()->_custom_features;
        file->store_pascal_string(key);

        int length;
        error = encode_variant(custom_features, nullptr, length, false);
        if (error != OK) {
            memdelete(file);
            ERR_FAIL_V(error);
        }

        Vector<uint8_t> buffer;
        buffer.resize(length);
        error = encode_variant(custom_features, buffer.ptrw(), length, false);
        if (error != OK) {
            memdelete(file);
            ERR_FAIL_V_MSG(
                error,
                "Error when trying to encode: " + custom_features
            );
        }
        file->store_32(length);
        file->store_buffer(buffer.ptr(), buffer.size());
    }

    for (auto E = grouped_settings.front(); E; E = E->next()) {
        const String section = E->key();
        for (auto F = E->get().front(); F; F = F->next()) {
            String key = F->get();
            if (section != "") {
                key = section + "/" + key;
            }
            Variant value;
            if (custom_settings.has(key)) {
                value = custom_settings[key];
            } else {
                value = get(key);
            }

            file->store_pascal_string(key);
            int length;
            error = encode_variant(value, nullptr, length, true);
            if (error != OK) {
                memdelete(file);
                ERR_FAIL_V_MSG(error, "Error when trying to encode Variant");
            }

            Vector<uint8_t> buffer;
            buffer.resize(length);

            error = encode_variant(value, buffer.ptrw(), length, true);
            if (error != OK) {
                memdelete(file);
                ERR_FAIL_V_MSG(error, "Error when trying to encode Variant");
            }
            file->store_32(length);
            file->store_buffer(buffer.ptr(), buffer.size());
        }
    }

    file->close();
    memdelete(file);

    return OK;
}

Error ProjectSettings::_save_text_settings(
    const String& settings_file,
    const Map<String, List<String>>& grouped_settings,
    const CustomSettings& custom_settings,
    const String& custom_features
) {
    Error error;
    FileAccess* file_access =
        FileAccess::open(settings_file, FileAccess::WRITE, &error);
    ERR_FAIL_COND_V_MSG(
        error != OK,
        error,
        "Cannot write project settings file: " + settings_file
    );

    String header =
        "; Engine configuration file.\n"
        "; It's best edited using the editor UI and not directly,\n"
        "; since the parameters that go here are not all obvious.\n"
        ";\n"
        "; Format:\n"
        ";   [section] ; section goes between []\n"
        ";   param=value ; assign values to parameters\n";
    file_access->store_line(header);

    file_access->store_string("config_version=" + itos(CONFIG_VERSION) + "\n");

    if (custom_features != String()) {
        file_access->store_line("custom_features=\"" + custom_features + "\"");
    }

    for (auto E = grouped_settings.front(); E; E = E->next()) {
        file_access->store_line("");

        const String section = E->key();
        if (section != "") {
            file_access->store_line("[" + section + "]");
            file_access->store_line("");
        }

        for (auto F = E->get().front(); F; F = F->next()) {
            const String key = F->get();
            String setting   = key;
            if (section != "") {
                setting = section + "/" + key;
            }
            Variant value;
            if (custom_settings.has(setting)) {
                value = custom_settings[setting];
            } else {
                value = get(setting);
            }

            String value_string;
            VariantWriter::write_to_string(value, value_string);
            file_access->store_line(
                key.property_name_encode() + "=" + value_string
            );
        }
    }

    file_access->close();
    memdelete(file_access);

    return OK;
}

Error ProjectSettings::_save_custom_bnd(const String& settings_file) {
    return save_custom(settings_file);
};

Variant _GLOBAL_DEF(
    const String& name,
    const Variant& default_value,
    bool requires_restart,
    bool ignore_in_docs
) {
    if (!ProjectSettings::get_singleton()->has_setting(name)) {
        ProjectSettings::get_singleton()->set(name, default_value);
    }
    ProjectSettings::get_singleton()->set_initial_value(name, default_value);
    ProjectSettings::get_singleton()->set_builtin_order(name);
    ProjectSettings::get_singleton()->set_restart_if_changed(
        name,
        requires_restart
    );
    ProjectSettings::get_singleton()->set_ignore_value_in_docs(
        name,
        ignore_in_docs
    );
    return ProjectSettings::get_singleton()->get(name);
}

Variant _GLOBAL_DEF_ALIAS(
    const String& name,
    const String& old_name,
    const Variant& default_value,
    bool requires_restart
) {
    // If the new name doesn't exist, try using the value of the old name.
    if (!ProjectSettings::get_singleton()->has_setting(name)) {
        if (ProjectSettings::get_singleton()->has_setting(old_name)) {
            Variant old_value = ProjectSettings::get_singleton()->get(old_name);
            ProjectSettings::get_singleton()->set(name, old_value);
        }
    }
    return _GLOBAL_DEF(name, default_value, requires_restart);
}
