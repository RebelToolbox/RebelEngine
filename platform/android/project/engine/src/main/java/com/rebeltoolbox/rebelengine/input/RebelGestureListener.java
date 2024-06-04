// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.input;

import com.rebeltoolbox.rebelengine.RebelEngine;
import com.rebeltoolbox.rebelengine.RebelView;

import android.view.GestureDetector;
import android.view.MotionEvent;

/**
 * Handles gesture input related events for the {@link RebelView}.
 * https://developer.android.com/reference/android/view/GestureDetector.SimpleOnGestureListener
 */
public class RebelGestureListener
    extends GestureDetector.SimpleOnGestureListener {
    private final RebelView rebelView;

    public RebelGestureListener(RebelView rebelView) {
        this.rebelView = rebelView;
    }

    private void queueEvent(Runnable task) {
        rebelView.queueEvent(task);
    }

    @Override
    public boolean onDown(MotionEvent event) {
        super.onDown(event);
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent event) {
        super.onSingleTapConfirmed(event);
        return true;
    }

    @Override
    public void onLongPress(MotionEvent event) {}

    @Override
    public boolean onDoubleTap(MotionEvent event) {
        final int x          = Math.round(event.getX());
        final int y          = Math.round(event.getY());
        final int buttonMask = event.getButtonState();
        RebelEngine.doubleTap(buttonMask, x, y);
        return true;
    }

    @Override
    public boolean onScroll(
        MotionEvent e1,
        MotionEvent e2,
        float distanceX,
        float distanceY
    ) {
        final int x = Math.round(distanceX);
        final int y = Math.round(distanceY);
        RebelEngine.scroll(x, y);
        return true;
    }

    @Override
    public boolean onFling(
        MotionEvent event1,
        MotionEvent event2,
        float velocityX,
        float velocityY
    ) {
        return true;
    }
}
