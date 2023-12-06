// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ANDROID_GODOT_PLUGIN_CONFIG_H
#define ANDROID_GODOT_PLUGIN_CONFIG_H

#include "core/error_list.h"
#include "core/io/config_file.h"
#include "core/project_settings.h"
#include "core/ustring.h"

/*
 The `config` section and fields are required and defined as follow:
- **name**: name of the plugin.
- **binary_type**: can be either `local` or `remote`. The type affects the
**binary** field.
- **binary**:
  - if **binary_type** is `local`, then this should be the filename of the
plugin `aar` file in the `res://android/plugins` directory (e.g:
`MyPlugin.aar`).
  - if **binary_type** is `remote`, then this should be a declaration for a
remote gradle binary (e.g: "org.godot.example:my-plugin:0.0.0").

The `dependencies` section and fields are optional and defined as follow:
- **local**: contains a list of local `.aar` binary files the plugin depends on.
The local binary dependencies must also be located in the
`res://android/plugins` directory.
- **remote**: contains a list of remote binary gradle dependencies for the
plugin.
- **maven_repos**: contains a list of urls specifying custom maven repos
required for the plugin's dependencies.

 See https://github.com/godotengine/godot/issues/38157#issuecomment-618773871
 */
struct PluginConfigAndroid {
    static const char* PLUGIN_CONFIG_EXT;

    static const char* CONFIG_SECTION;
    static const char* CONFIG_NAME_KEY;
    static const char* CONFIG_BINARY_TYPE_KEY;
    static const char* CONFIG_BINARY_KEY;

    static const char* DEPENDENCIES_SECTION;
    static const char* DEPENDENCIES_LOCAL_KEY;
    static const char* DEPENDENCIES_REMOTE_KEY;
    static const char* DEPENDENCIES_CUSTOM_MAVEN_REPOS_KEY;

    static const char* BINARY_TYPE_LOCAL;
    static const char* BINARY_TYPE_REMOTE;

    static const char* PLUGIN_VALUE_SEPARATOR;

    // Set to true when the config file is properly loaded.
    bool valid_config     = false;
    // Unix timestamp of last change to this plugin.
    uint64_t last_updated = 0;

    // Required config section
    String name;
    String binary_type;
    String binary;

    // Optional dependencies section
    Vector<String> local_dependencies;
    Vector<String> remote_dependencies;
    Vector<String> maven_repos;

    static String resolve_local_dependency_path(
        String plugin_config_dir,
        String dependency_path
    );

    static PluginConfigAndroid resolve_prebuilt_plugin(
        PluginConfigAndroid prebuilt_plugin,
        String plugin_config_dir
    );

    static Vector<PluginConfigAndroid> get_prebuilt_plugins(
        String plugins_base_dir
    );

    static bool is_plugin_config_valid(PluginConfigAndroid plugin_config);

    static uint64_t get_plugin_modification_time(
        const PluginConfigAndroid& plugin_config,
        const String& config_path
    );

    static PluginConfigAndroid load_plugin_config(
        Ref<ConfigFile> config_file,
        const String& path
    );

    static String get_plugins_dependencies(
        String type,
        Vector<PluginConfigAndroid> plugins_configs
    );

    static String get_plugins_maven_repos(
        Vector<PluginConfigAndroid> plugins_configs
    );

    static String get_plugins_names(Vector<PluginConfigAndroid> plugins_configs
    );
};

#endif // ANDROID_GODOT_PLUGIN_CONFIG_H
