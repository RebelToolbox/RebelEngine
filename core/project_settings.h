// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include "core/object.h"
#include "core/os/thread_safe.h"
#include "core/set.h"

struct Setting {
    String name;
    Variant current_value;
    Variant initial_value;
    int position            = 0;
    int flags               = 0;
    bool hide_from_editor   = false;
    bool restart_if_changed = false;
#ifdef DEBUG_METHODS_ENABLED
    bool ignore_value_in_docs = false;
#endif

    Setting() = default;

    Setting(const String& name, const Variant& value, int position) :
        name(name),
        current_value(value),
        position(position) {}
};

bool operator<(const Setting& left, const Setting& right);

class ProjectSettings : public Object {
    GDCLASS(ProjectSettings, Object);
    _THREAD_SAFE_CLASS_

public:
    typedef Map<String, Variant> CustomSettings;
    static const int CUSTOM_SETTINGS_START = 1 << 16;
    static const int CONFIG_VERSION        = 4;

    static ProjectSettings* get_singleton();

    ProjectSettings();
    ~ProjectSettings();

    bool has_setting(const String& name) const;
    Variant get_setting(const String& name) const;
    void set_setting(const String& name, const Variant& value);

    int get_order(const String& name) const;
    void set_order(const String& name, int position);
    void set_builtin_order(const String& name);

    bool get_ignore_value_in_docs(const String& name) const;
    void set_ignore_value_in_docs(const String& name, bool ignore_in_docs);

    Variant property_get_revert(const String& name) const;
    bool property_can_revert(const String& name);

    String get_project_data_dir_name() const;
    String get_project_data_path() const;
    String get_resource_path() const;
    const Map<StringName, PropertyInfo>& get_custom_property_info() const;
    uint64_t get_last_saved_time() const;
    List<String> get_input_presets() const;
    Vector<String> get_optimizer_presets() const;
    bool is_using_datapack() const;
    bool has_custom_feature(const String& feature) const;

    void clear(const String& name);
    void set_initial_value(const String& name, const Variant& value);
    void set_restart_if_changed(const String& name, bool restart_required);
    void set_disable_feature_overrides(bool disable);
    void set_custom_property_info(
        const String& name,
        const PropertyInfo& property_info
    );

    String globalize_path(const String& local_path) const;
    String localize_path(const String& global_path) const;

    Error setup(
        const String& path,
        const String& main_pack,
        bool try_parent_dirs = false
    );

    Error save();
    Error save_custom(
        const String& settings_file,
        const CustomSettings& custom_settings = CustomSettings(),
        const Vector<String>& custom_features = Vector<String>(),
        bool merge_with_current               = true
    );

private:
    static ProjectSettings* singleton;
    static void _bind_methods();

    Map<StringName, Setting> settings;
    Map<StringName, PropertyInfo> custom_property_info;
    Map<StringName, StringName> feature_overrides;
    List<String> input_presets;
    Set<String> custom_features;

    String project_data_dir_name = "import";
    String resource_path;

    int custom_position            = CUSTOM_SETTINGS_START;
    int builtin_position           = 0;
    uint64_t last_save_time        = 0;
    bool disable_feature_overrides = false;
    bool using_datapack            = false;

    bool _get(const StringName& name, Variant& r_value) const;
    bool _set(const StringName& name, const Variant& value);
    void _get_property_list(List<PropertyInfo>* r_list) const;

    void _add_property_info(const Dictionary& p_info);
    void _upgrade_settings(int current_version);

    Error _setup(
        const String& path,
        const String& main_pack,
        bool try_parent_dirs = false
    );

    bool _load_resource_pack(
        const String& pack_file,
        bool replace_files = true,
        int offset         = 0
    );

    Error _load_binary_settings(const String& settings_file);
    Error _load_text_settings(const String& settings_file);
    Error _load_settings(
        const String& settings_dir = "res://",
        const String& override_dir = "res://"
    );
    Error _save_binary_settings(
        const String& settings_file,
        const Map<String, List<String>>& grouped_settings,
        const CustomSettings& custom_settings = CustomSettings(),
        const String& custom_features         = String()
    );
    Error _save_text_settings(
        const String& settings_file,
        const Map<String, List<String>>& grouped_settings,
        const CustomSettings& custom_settings = CustomSettings(),
        const String& custom_features         = String()
    );
    Error _save_custom_bnd(const String& settings_file);
};

Variant _GLOBAL_DEF(
    const String& name,
    const Variant& default_value,
    bool requires_restart = false,
    bool ignore_in_docs   = false
);
Variant _GLOBAL_DEF_ALIAS(
    const String& name,
    const String& old_name,
    const Variant& default_value,
    bool requires_restart = false
);
#define GLOBAL_DEF(name, value)           _GLOBAL_DEF(name, value)
#define GLOBAL_DEF_RST(name, value)       _GLOBAL_DEF(name, value, true)
#define GLOBAL_DEF_NOVAL(name, value)     _GLOBAL_DEF(name, value, false, true)
#define GLOBAL_DEF_RST_NOVAL(name, value) _GLOBAL_DEF(name, value, true, true)
#define GLOBAL_DEF_ALIAS(name, old_name, value)                                \
    _GLOBAL_DEF_ALIAS(name, old_name, value)
#define GLOBAL_DEF_ALIAS_RST(name, old_name, value)                            \
    _GLOBAL_DEF(name, old_name, value, true)
#define GLOBAL_GET(name) ProjectSettings::get_singleton()->get(name)

#endif // PROJECT_SETTINGS_H
