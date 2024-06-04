// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelapp;

import com.rebeltoolbox.rebelengine.RebelActivity;

import android.os.Bundle;

/**
 * Template activity for Android custom builds.
 * Feel free to extend and modify this class for your custom logic.
 */
public class RebelApp extends RebelActivity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        setTheme(R.style.MainTheme);
        super.onCreate(savedInstanceState);
    }
}
