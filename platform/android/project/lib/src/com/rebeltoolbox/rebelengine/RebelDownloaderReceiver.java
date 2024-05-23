// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.util.Log;

import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;

/**
 * You should start your derived downloader class when this receiver gets the
 * message from the alarm service using the provided service helper function
 * within the DownloaderClientMarshaller. This class must be then registered in
 * your AndroidManifest.xml file with a section like this: <receiver
 * android:name=".RebelDownloaderReceiver"/>
 */
public class RebelDownloaderReceiver extends BroadcastReceiver {
    private static final String TAG =
        RebelDownloaderReceiver.class.getSimpleName();

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "Rebel Downloader Alarm received");
        try {
            DownloaderClientMarshaller.startDownloadServiceIfRequired(
                context,
                intent,
                RebelDownloaderService.class
            );
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            Log.d(
                TAG,
                "Exception: " + e.getClass().getName() + ":" + e.getMessage()
            );
        }
    }
}
