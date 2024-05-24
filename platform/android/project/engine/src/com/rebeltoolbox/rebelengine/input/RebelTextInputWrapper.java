// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.input;

import com.rebeltoolbox.rebelengine.*;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class RebelTextInputWrapper
    implements TextWatcher, OnEditorActionListener {
    // ===========================================================
    // Constants
    // ===========================================================
    private static final String TAG =
        RebelTextInputWrapper.class.getSimpleName();

    // ===========================================================
    // Fields
    // ===========================================================
    private final RebelView rebelView;
    private final RebelEditText rebelEditText;
    private String mOriginText;
    private boolean mHasSelection;

    // ===========================================================
    // Constructors
    // ===========================================================

    public RebelTextInputWrapper(
        final RebelView rebelView,
        final RebelEditText rebelEditText
    ) {
        this.rebelView     = rebelView;
        this.rebelEditText = rebelEditText;
    }

    // ===========================================================
    // Getter & Setter
    // ===========================================================

    private boolean isFullScreenEdit() {
        final InputMethodManager imm =
            (InputMethodManager)rebelEditText.getContext().getSystemService(
                Context.INPUT_METHOD_SERVICE
            );
        return imm.isFullscreenMode();
    }

    public void setOriginText(final String originText) {
        this.mOriginText = originText;
    }

    public void setSelection(boolean selection) {
        mHasSelection = selection;
    }

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    @Override
    public void afterTextChanged(final Editable s) {}

    @Override
    public void beforeTextChanged(
        final CharSequence pCharSequence,
        final int start,
        final int count,
        final int after
    ) {
        // Log.d(TAG, "beforeTextChanged(" + pCharSequence + ")start: " + start
        // + ",count: " + count + ",after: " + after);

        for (int i = 0; i < count; ++i) {
            RebelEngine
                .key(KeyEvent.KEYCODE_DEL, KeyEvent.KEYCODE_DEL, 0, true);
            RebelEngine
                .key(KeyEvent.KEYCODE_DEL, KeyEvent.KEYCODE_DEL, 0, false);

            if (mHasSelection) {
                mHasSelection = false;
                break;
            }
        }
    }

    @Override
    public void onTextChanged(
        final CharSequence pCharSequence,
        final int start,
        final int before,
        final int count
    ) {
        // Log.d(TAG, "onTextChanged(" + pCharSequence + ")start: " + start +
        // ",count: " + count + ",before: " + before);

        final int[] newChars = new int[count];
        for (int i = start; i < start + count; ++i) {
            newChars[i - start] = pCharSequence.charAt(i);
        }
        for (int i = 0; i < count; ++i) {
            int key = newChars[i];
            if ((key == '\n') && !rebelEditText.isMultiline()) {
                // Return keys are handled through action events
                continue;
            }
            RebelEngine.key(0, 0, key, true);
            RebelEngine.key(0, 0, key, false);
        }
    }

    @Override
    public boolean onEditorAction(
        final TextView pTextView,
        final int pActionID,
        final KeyEvent pKeyEvent
    ) {
        if (this.rebelEditText == pTextView && this.isFullScreenEdit()) {
            final String characters = pKeyEvent.getCharacters();

            for (int i = 0; i < characters.length(); i++) {
                final int ch = characters.codePointAt(i);
                RebelEngine.key(0, 0, ch, true);
                RebelEngine.key(0, 0, ch, false);
            }
        }

        if (pActionID == EditorInfo.IME_ACTION_DONE) {
            // Enter key has been pressed
            rebelView.queueEvent(() -> {
                RebelEngine.key(
                    KeyEvent.KEYCODE_ENTER,
                    KeyEvent.KEYCODE_ENTER,
                    0,
                    true
                );
                RebelEngine.key(
                    KeyEvent.KEYCODE_ENTER,
                    KeyEvent.KEYCODE_ENTER,
                    0,
                    false
                );
            });
            rebelView.requestFocus();
            return true;
        }
        return false;
    }

    // ===========================================================
    // Methods
    // ===========================================================

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
}
