// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.utils;

import android.app.Activity;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.util.Log;

public class WifiMulticastLock {
    /* A single, reference counted, multicast lock, or null if permission
     * CHANGE_WIFI_MULTICAST_STATE is missing */
    private static final String TAG = WifiMulticastLock.class.getSimpleName();

    private WifiManager.MulticastLock multicastLock;

    public WifiMulticastLock(Activity p_activity) {
        if (PermissionsUtil.hasManifestPermission(
                p_activity,
                "android.permission.CHANGE_WIFI_MULTICAST_STATE"
            )) {
            WifiManager wifi = (WifiManager)p_activity.getApplicationContext()
                                   .getSystemService(Context.WIFI_SERVICE);
            multicastLock = wifi.createMulticastLock(TAG);
            multicastLock.setReferenceCounted(true);
        }
    }

    /**
     * Acquire the multicast lock. This is required on some devices to receive
     * broadcast/multicast packets. This is done automatically when enabling
     * broadcast or joining a multicast group on a socket.
     */
    public void multicastLockAcquire() {
        if (multicastLock == null) return;
        try {
            multicastLock.acquire();
        } catch (RuntimeException e) {
            Log.e(TAG, "Exception during multicast lock acquire: " + e);
        }
    }

    /**
     * Release the multicast lock.
     * This is done automatically when the lock is no longer needed by a socket.
     */
    public void multicastLockRelease() {
        if (multicastLock == null) return;
        try {
            multicastLock.release();
        } catch (RuntimeException e) {
            Log.e(TAG, "Exception during multicast lock release: " + e);
        }
    }
}
