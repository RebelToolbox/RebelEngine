// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.input;

import android.util.SparseArray;

import java.util.ArrayList;
import java.util.List;

/**
 * POJO class to represent a Joystick input device.
 */
class Joystick {
    int device_id;
    String name;
    List<Integer> axes                            = new ArrayList<>();
    protected boolean hasAxisHat                  = false;
    /*
     * Keep track of values so we can prevent flooding the engine with useless
     * events.
     */
    protected final SparseArray<Float> axesValues = new SparseArray<>(4);
    protected int hatX;
    protected int hatY;
}
