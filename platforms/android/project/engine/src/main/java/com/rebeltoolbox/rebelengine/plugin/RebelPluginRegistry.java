// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.plugin;

import com.rebeltoolbox.rebelengine.RebelFragment;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.Nullable;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Registry used to load and access the registered Rebel plugins.
 */
public final class RebelPluginRegistry {
    private static final String TAG = RebelPluginRegistry.class.getSimpleName();

    private static final String META_DATA_NAME_PREFIX = "rebel.plugin.";

    private static RebelPluginRegistry instance;
    private final ConcurrentHashMap<String, RebelPlugin> registry;

    private RebelPluginRegistry(RebelFragment rebelFragment) {
        registry = new ConcurrentHashMap<>();
        loadPlugins(rebelFragment);
    }

    /**
     * Retrieve the plugin tied to the given plugin name.
     * @param pluginName Name of the plugin
     * @return {@link RebelPlugin} handle if it exists, null otherwise.
     */
    @Nullable
    public RebelPlugin getPlugin(String pluginName) {
        return registry.get(pluginName);
    }

    /**
     * Retrieve the full set of loaded plugins.
     */
    public Collection<RebelPlugin> getAllRebelPlugins() {
        return registry.values();
    }

    /**
     * Parse the manifest file and load all included Rebel plugins.
     * <p>
     * A plugin manifest entry is a '<meta-data>' tag setup as described in the
     * {@link RebelPlugin} documentation.
     *
     * @param rebelFragment Rebel Fragment
     * @return A singleton instance of {@link RebelPluginRegistry}. This ensures
     *     that only one instance
     * of each Rebel plugin is available at runtime.
     */
    public static RebelPluginRegistry initialize(RebelFragment rebelFragment) {
        if (instance == null) {
            instance = new RebelPluginRegistry(rebelFragment);
        }

        return instance;
    }

    /**
     * Return the plugin registry if it's initialized.
     * Throws a {@link IllegalStateException} exception if not.
     *
     * @throws IllegalStateException if {@link
     *     RebelPluginRegistry#initialize(RebelFragment)} has not been called
     *     prior to calling this method.
     */
    public static RebelPluginRegistry getPluginRegistry()
        throws IllegalStateException {
        if (instance == null) {
            throw new IllegalStateException(
                "Plugin registry hasn't been initialized."
            );
        }

        return instance;
    }

    private void loadPlugins(RebelFragment rebelFragment) {
        try {
            final Activity activity = rebelFragment.getActivity();
            ApplicationInfo appInfo =
                activity.getPackageManager().getApplicationInfo(
                    activity.getPackageName(),
                    PackageManager.GET_META_DATA
                );
            Bundle metaData = appInfo.metaData;
            if (metaData == null || metaData.isEmpty()) {
                return;
            }

            for (String metaDataName : metaData.keySet()) {
                // Parse the meta-data looking for entry with the Rebel Plugin
                // name prefix.
                if (metaDataName.startsWith(META_DATA_NAME_PREFIX)) {
                    String rebelPluginName =
                        metaDataName.substring(META_DATA_NAME_PREFIX.length())
                            .trim();
                    Log.i(TAG, "Initializing Rebel plugin " + rebelPluginName);

                    // Retrieve the plugin class full name.
                    String pluginHandleClassFullName =
                        metaData.getString(metaDataName);
                    if (!TextUtils.isEmpty(pluginHandleClassFullName)) {
                        try {
                            // Attempt to create the plugin init class via
                            // reflection.
                            @SuppressWarnings("unchecked")
                            Class<RebelPlugin> pluginClass =
                                (Class<RebelPlugin>)Class.forName(
                                    pluginHandleClassFullName
                                );
                            Constructor<RebelPlugin> pluginConstructor =
                                pluginClass.getConstructor(RebelFragment.class);
                            RebelPlugin rebelPlugin =
                                pluginConstructor.newInstance(rebelFragment);

                            // Load the plugin initializer into the registry
                            // using the plugin name as key.
                            if (!rebelPluginName.equals(
                                    rebelPlugin.getPluginName()
                                )) {
                                Log.w(
                                    TAG,
                                    "Meta-data plugin name does not match the "
                                        + "value returned by the plugin "
                                        + "handle: " + rebelPluginName + " =/= "
                                        + rebelPlugin.getPluginName()
                                );
                            }
                            registry.put(rebelPluginName, rebelPlugin);
                            Log.i(
                                TAG,
                                "Completed initialization for Rebel plugin "
                                    + rebelPluginName
                            );
                        } catch (
                            ClassNotFoundException | IllegalAccessException
                            | InstantiationException | NoSuchMethodException
                            | InvocationTargetException e
                        ) {
                            Log.w(
                                TAG,
                                "Unable to load Rebel plugin "
                                    + rebelPluginName,
                                e
                            );
                        }
                    } else {
                        Log.w(
                            TAG,
                            "Invalid plugin loader class for " + rebelPluginName
                        );
                    }
                }
            }
        } catch (PackageManager.NameNotFoundException e) {
            Log.e(TAG, "Unable load Rebel plugins from the manifest file.", e);
        }
    }
}
