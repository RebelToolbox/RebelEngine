/*************************************************************************/
/*  RebelDownloaderReceiver.java                                         */
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
