// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine;

import android.app.Instrumentation;
import android.content.Intent;
import android.os.Bundle;

public class RebelInstrumentation extends Instrumentation {
    private Intent intent;

    @Override
    public void onCreate(Bundle arguments) {
        intent = arguments.getParcelable("intent");
        start();
    }

    @Override
    public void onStart() {
        startActivitySync(intent);
    }
}
