// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_plugin_config.h"

#include "core/os/file_access.h"

const char* AndroidPluginConfig::PLUGIN_CONFIG_EXT = ".gdap";

const char* AndroidPluginConfig::CONFIG_SECTION         = "config";
const char* AndroidPluginConfig::CONFIG_NAME_KEY        = "name";
const char* AndroidPluginConfig::CONFIG_BINARY_TYPE_KEY = "binary_type";
const char* AndroidPluginConfig::CONFIG_BINARY_KEY      = "binary";

const char* AndroidPluginConfig::DEPENDENCIES_SECTION    = "dependencies";
const char* AndroidPluginConfig::DEPENDENCIES_LOCAL_KEY  = "local";
const char* AndroidPluginConfig::DEPENDENCIES_REMOTE_KEY = "remote";
const char* AndroidPluginConfig::DEPENDENCIES_CUSTOM_MAVEN_REPOS_KEY =
    "custom_maven_repos";

const char* AndroidPluginConfig::BINARY_TYPE_LOCAL  = "local";
const char* AndroidPluginConfig::BINARY_TYPE_REMOTE = "remote";

const char* AndroidPluginConfig::PLUGIN_VALUE_SEPARATOR = "|";

/*
 * Set of prebuilt plugins.
 * Currently unused, this is just for future reference:
 */
// static const AndroidPluginConfig MY_PREBUILT_PLUGIN = {
//	/*.valid_config =*/true,
//	/*.last_updated =*/0,
//	/*.name =*/"PluginName",
//	/*.binary_type =*/"local",
//	/*.binary =*/"res://android/project/libs/plugins/PluginName.release.aar",
//	/*.local_dependencies =*/{},
// 	/*.remote_dependencies
// =*/String("com.android.billingclient:billing:2.2.1").split("|"),
// 	/*.custom_maven_repos =*/{}
// };

String AndroidPluginConfig::resolve_local_dependency_path(
    String plugin_config_dir,
    String dependency_path
) {
    String absolute_path;
    if (!dependency_path.empty()) {
        if (dependency_path.is_abs_path()) {
            absolute_path =
                ProjectSettings::get_singleton()->globalize_path(dependency_path
                );
        } else {
            absolute_path = plugin_config_dir.plus_file(dependency_path);
        }
    }

    return absolute_path;
}

AndroidPluginConfig AndroidPluginConfig::resolve_prebuilt_plugin(
    AndroidPluginConfig prebuilt_plugin,
    String plugin_config_dir
) {
    AndroidPluginConfig resolved = prebuilt_plugin;
    resolved.binary =
        resolved.binary_type == AndroidPluginConfig::BINARY_TYPE_LOCAL
            ? resolve_local_dependency_path(
                plugin_config_dir,
                prebuilt_plugin.binary
            )
            : prebuilt_plugin.binary;
    if (!prebuilt_plugin.local_dependencies.empty()) {
        resolved.local_dependencies.clear();
        for (int i = 0; i < prebuilt_plugin.local_dependencies.size(); i++) {
            resolved.local_dependencies.push_back(resolve_local_dependency_path(
                plugin_config_dir,
                prebuilt_plugin.local_dependencies[i]
            ));
        }
    }
    return resolved;
}

Vector<AndroidPluginConfig> AndroidPluginConfig::get_prebuilt_plugins(
    String plugins_base_dir
) {
    Vector<AndroidPluginConfig> prebuilt_plugins;
    // prebuilt_plugins.push_back(resolve_prebuilt_plugin(MY_PREBUILT_PLUGIN,
    // plugins_base_dir));
    return prebuilt_plugins;
}

bool AndroidPluginConfig::is_plugin_config_valid(
    AndroidPluginConfig plugin_config
) {
    bool valid_name = !plugin_config.name.empty();
    bool valid_binary_type =
        plugin_config.binary_type == AndroidPluginConfig::BINARY_TYPE_LOCAL
        || plugin_config.binary_type == AndroidPluginConfig::BINARY_TYPE_REMOTE;

    bool valid_binary = false;
    if (valid_binary_type) {
        valid_binary = !plugin_config.binary.empty()
                    && (plugin_config.binary_type
                            == AndroidPluginConfig::BINARY_TYPE_REMOTE
                        || FileAccess::exists(plugin_config.binary));
    }

    bool valid_local_dependencies = true;
    if (!plugin_config.local_dependencies.empty()) {
        for (int i = 0; i < plugin_config.local_dependencies.size(); i++) {
            if (!FileAccess::exists(plugin_config.local_dependencies[i])) {
                valid_local_dependencies = false;
                break;
            }
        }
    }
    return valid_name && valid_binary && valid_binary_type
        && valid_local_dependencies;
}

uint64_t AndroidPluginConfig::get_plugin_modification_time(
    const AndroidPluginConfig& plugin_config,
    const String& config_path
) {
    uint64_t last_modified = FileAccess::get_modified_time(config_path);
    last_modified =
        MAX(last_modified, FileAccess::get_modified_time(plugin_config.binary));

    for (int i = 0; i < plugin_config.local_dependencies.size(); i++) {
        String local_dependency = plugin_config.local_dependencies.get(i);
        last_modified =
            MAX(last_modified, FileAccess::get_modified_time(local_dependency));
    }

    return last_modified;
}

AndroidPluginConfig AndroidPluginConfig::load_plugin_config(
    Ref<ConfigFile> config_file,
    const String& path
) {
    AndroidPluginConfig plugin_config = {};

    if (config_file.is_valid()) {
        Error err = config_file->load(path);
        if (err == OK) {
            String config_base_dir = path.get_base_dir();

            plugin_config.name = config_file->get_value(
                AndroidPluginConfig::CONFIG_SECTION,
                AndroidPluginConfig::CONFIG_NAME_KEY,
                String()
            );
            plugin_config.binary_type = config_file->get_value(
                AndroidPluginConfig::CONFIG_SECTION,
                AndroidPluginConfig::CONFIG_BINARY_TYPE_KEY,
                String()
            );

            String binary_path = config_file->get_value(
                AndroidPluginConfig::CONFIG_SECTION,
                AndroidPluginConfig::CONFIG_BINARY_KEY,
                String()
            );
            plugin_config.binary =
                plugin_config.binary_type
                        == AndroidPluginConfig::BINARY_TYPE_LOCAL
                    ? resolve_local_dependency_path(
                        config_base_dir,
                        binary_path
                    )
                    : binary_path;

            if (config_file->has_section(
                    AndroidPluginConfig::DEPENDENCIES_SECTION
                )) {
                Vector<String> local_dependencies_paths =
                    config_file->get_value(
                        AndroidPluginConfig::DEPENDENCIES_SECTION,
                        AndroidPluginConfig::DEPENDENCIES_LOCAL_KEY,
                        Vector<String>()
                    );
                if (!local_dependencies_paths.empty()) {
                    for (int i = 0; i < local_dependencies_paths.size(); i++) {
                        plugin_config.local_dependencies.push_back(
                            resolve_local_dependency_path(
                                config_base_dir,
                                local_dependencies_paths[i]
                            )
                        );
                    }
                }

                plugin_config.remote_dependencies = config_file->get_value(
                    AndroidPluginConfig::DEPENDENCIES_SECTION,
                    AndroidPluginConfig::DEPENDENCIES_REMOTE_KEY,
                    Vector<String>()
                );
                plugin_config.maven_repos = config_file->get_value(
                    AndroidPluginConfig::DEPENDENCIES_SECTION,
                    AndroidPluginConfig::DEPENDENCIES_CUSTOM_MAVEN_REPOS_KEY,
                    Vector<String>()
                );
            }

            plugin_config.valid_config = is_plugin_config_valid(plugin_config);
            plugin_config.last_updated =
                get_plugin_modification_time(plugin_config, path);
        }
    }

    return plugin_config;
}

String AndroidPluginConfig::get_plugins_dependencies(
    String type,
    Vector<AndroidPluginConfig> plugins_configs
) {
    String plugins_dependencies;
    if (!plugins_configs.empty()) {
        Vector<String> dependencies;
        for (int i = 0; i < plugins_configs.size(); i++) {
            AndroidPluginConfig config = plugins_configs[i];
            if (!config.valid_config) {
                continue;
            }

            if (config.binary_type == type) {
                dependencies.push_back(config.binary);
            }

            if (type == AndroidPluginConfig::BINARY_TYPE_LOCAL) {
                dependencies.append_array(config.local_dependencies);
            }

            if (type == AndroidPluginConfig::BINARY_TYPE_REMOTE) {
                dependencies.append_array(config.remote_dependencies);
            }
        }

        plugins_dependencies =
            String(AndroidPluginConfig::PLUGIN_VALUE_SEPARATOR)
                .join(dependencies);
    }

    return plugins_dependencies;
}

String AndroidPluginConfig::get_plugins_maven_repos(
    Vector<AndroidPluginConfig> plugins_configs
) {
    String maven_repos;
    if (!plugins_configs.empty()) {
        Vector<String> repos_urls;
        for (int i = 0; i < plugins_configs.size(); i++) {
            AndroidPluginConfig config = plugins_configs[i];
            if (!config.valid_config) {
                continue;
            }

            repos_urls.append_array(config.maven_repos);
        }

        maven_repos = String(AndroidPluginConfig::PLUGIN_VALUE_SEPARATOR)
                          .join(repos_urls);
    }
    return maven_repos;
}

String AndroidPluginConfig::get_plugins_names(
    Vector<AndroidPluginConfig> plugins_configs
) {
    String plugins_names;
    if (!plugins_configs.empty()) {
        Vector<String> names;
        for (int i = 0; i < plugins_configs.size(); i++) {
            AndroidPluginConfig config = plugins_configs[i];
            if (!config.valid_config) {
                continue;
            }

            names.push_back(config.name);
        }
        plugins_names =
            String(AndroidPluginConfig::PLUGIN_VALUE_SEPARATOR).join(names);
    }

    return plugins_names;
}
