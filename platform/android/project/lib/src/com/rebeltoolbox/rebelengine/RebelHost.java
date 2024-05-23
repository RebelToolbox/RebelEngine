// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine;

import java.util.Collections;
import java.util.List;

/**
 * Denotes a component (e.g: Activity, Fragment) that hosts the {@link
 * RebelFragment} fragment.
 */
public interface RebelHost {
    /**
     * Provides a set of command line parameters to setup the engine.
     */
    default List<String> getCommandLine() {
        return Collections.emptyList();
    }

    /**
     * Invoked on the render thread when the Rebel Engine setup is complete.
     */
    default void onSetupCompleted() {}

    /**
     * Invoked on the render thread when the Rebel Engine main loop has started.
     */
    default void onMainLoopStarted() {}

    /**
     * Invoked on the UI thread as the last step of the Rebel Engine clean up
     * phase.
     */
    default void onQuit(RebelFragment quitFragment) {}

    /**
     * Invoked on the GL thread when the Rebel Engine is restarted. It's up to
     * the host to perform the appropriate action(s).
     */
    default void onRestart(RebelFragment restartFragment) {}
}
