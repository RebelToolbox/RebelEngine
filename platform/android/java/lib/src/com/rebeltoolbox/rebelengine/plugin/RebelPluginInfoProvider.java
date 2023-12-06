// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.plugin;

import androidx.annotation.NonNull;

import java.util.Collections;
import java.util.Set;

public interface RebelPluginInfoProvider {
    @NonNull
    String getPluginName();

    @NonNull
    default Set<SignalInfo> getPluginSignals() {
        return Collections.emptySet();
    }

    /**
     * Returns the paths for the plugin's gdnative libraries (if any).
     *
     * The paths must be relative to the 'assets' directory and point to a
     * '*.gdnlib' file.
     */
    @NonNull
    default Set<String> getPluginGDNativeLibrariesPaths() {
        return Collections.emptySet();
    }

    /**
     * This is invoked on the render thread when the plugin described by this
     * instance has been registered.
     */
    default void onPluginRegistered() {}
}
