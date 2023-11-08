/*************************************************************************/
/*  WifiMulticastLock.java                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           REBEL ENGINE                                */
/*************************************************************************/
/* Copyright (c) 2022-Present Rebel Engine contributors                  */
/* Copyright (c) 2014-2022 Godot Engine contributors                     */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur                  */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

package com.rebeltoolbox.rebelengine.utils;

import android.app.Activity;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.util.Log;

public class WifiMulticastLock {
	/* A single, reference counted, multicast lock, or null if permission CHANGE_WIFI_MULTICAST_STATE is missing */
	private static final String TAG = WifiMulticastLock.class.getSimpleName();

	private WifiManager.MulticastLock multicastLock;

	public WifiMulticastLock(Activity p_activity) {
		if (PermissionsUtil.hasManifestPermission(p_activity, "android.permission.CHANGE_WIFI_MULTICAST_STATE")) {
			WifiManager wifi = (WifiManager)p_activity.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
			multicastLock = wifi.createMulticastLock(TAG);
			multicastLock.setReferenceCounted(true);
		}
	}

	/**
	 * Acquire the multicast lock. This is required on some devices to receive broadcast/multicast packets.
	 * This is done automatically when enabling broadcast or joining a multicast group on a socket.
	 */
	public void multicastLockAcquire() {
		if (multicastLock == null)
			return;
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
		if (multicastLock == null)
			return;
		try {
			multicastLock.release();
		} catch (RuntimeException e) {
			Log.e(TAG, "Exception during multicast lock release: " + e);
		}
	}
}
