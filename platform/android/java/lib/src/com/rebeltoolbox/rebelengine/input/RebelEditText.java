/*************************************************************************/
/*  RebelEditText.java                                                   */
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

package com.rebeltoolbox.rebelengine.input;

import com.rebeltoolbox.rebelengine.*;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.text.InputFilter;
import android.text.InputType;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

import java.lang.ref.WeakReference;

public class RebelEditText extends EditText {
	// ===========================================================
	// Constants
	// ===========================================================
	private final static int HANDLER_OPEN_IME_KEYBOARD = 2;
	private final static int HANDLER_CLOSE_IME_KEYBOARD = 3;

	// ===========================================================
	// Fields
	// ===========================================================
	private RebelView rebelView;
	private GodotTextInputWrapper mInputWrapper;
	private final EditHandler editHandler = new EditHandler(this);
	private String mOriginText;
	private int mMaxInputLength = Integer.MAX_VALUE;
	private boolean mMultiline = false;

	private static class EditHandler extends Handler {
		private final WeakReference<RebelEditText> rebelEditTextReference;
		public EditHandler(RebelEditText rebelEditText) {
			rebelEditTextReference = new WeakReference<>(rebelEditText);
		}

		@Override
		public void handleMessage(Message msg) {
			RebelEditText rebelEditText = rebelEditTextReference.get();
			if (rebelEditText != null) {
				rebelEditText.handleMessage(msg);
			}
		}
	}

	// ===========================================================
	// Constructors
	// ===========================================================
	public RebelEditText(final Context context) {
		super(context);
		this.initView();
	}

	public RebelEditText(final Context context, final AttributeSet attrs) {
		super(context, attrs);
		this.initView();
	}

	public RebelEditText(final Context context, final AttributeSet attrs, final int defStyle) {
		super(context, attrs, defStyle);
		this.initView();
	}

	protected void initView() {
		this.setPadding(0, 0, 0, 0);
		this.setImeOptions(EditorInfo.IME_FLAG_NO_EXTRACT_UI | EditorInfo.IME_ACTION_DONE);
	}

	public boolean isMultiline() {
		return mMultiline;
	}

	private void handleMessage(final Message msg) {
		switch (msg.what) {
			case HANDLER_OPEN_IME_KEYBOARD: {
				RebelEditText rebelEditText = (RebelEditText)msg.obj;
				String text = rebelEditText.mOriginText;
				if (rebelEditText.requestFocus()) {
					rebelEditText.removeTextChangedListener(rebelEditText.mInputWrapper);
					setMaxInputLength(rebelEditText);
					rebelEditText.setText("");
					rebelEditText.append(text);
					if (msg.arg2 != -1) {
						rebelEditText.setSelection(msg.arg1, msg.arg2);
						rebelEditText.mInputWrapper.setSelection(true);
					} else {
						rebelEditText.mInputWrapper.setSelection(false);
					}

					int inputType = InputType.TYPE_CLASS_TEXT;
					if (rebelEditText.isMultiline()) {
						inputType |= InputType.TYPE_TEXT_FLAG_MULTI_LINE;
					}
					rebelEditText.setInputType(inputType);

					rebelEditText.mInputWrapper.setOriginText(text);
					rebelEditText.addTextChangedListener(rebelEditText.mInputWrapper);
					final InputMethodManager imm = (InputMethodManager)rebelView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
					imm.showSoftInput(rebelEditText, 0);
				}
			} break;

			case HANDLER_CLOSE_IME_KEYBOARD: {
				RebelEditText edit = (RebelEditText)msg.obj;

				edit.removeTextChangedListener(mInputWrapper);
				final InputMethodManager imm = (InputMethodManager)rebelView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
				imm.hideSoftInputFromWindow(edit.getWindowToken(), 0);
				edit.rebelView.requestFocus();
			} break;
		}
	}

	private void setMaxInputLength(EditText p_edit_text) {
		InputFilter[] filters = new InputFilter[1];
		filters[0] = new InputFilter.LengthFilter(this.mMaxInputLength);
		p_edit_text.setFilters(filters);
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================
	public void setRebelView(final RebelView rebelView) {
		this.rebelView = rebelView;
		if (mInputWrapper == null)
			mInputWrapper = new GodotTextInputWrapper(rebelView, this);
		this.setOnEditorActionListener(mInputWrapper);
		rebelView.requestFocus();
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================
	@Override
	public boolean onKeyDown(final int keyCode, final KeyEvent keyEvent) {
		// Let SurfaceView get focus if back key is pressed
		// Handle special key events
		if (isSpecialKey(keyCode, keyEvent) && rebelView.getInputHandler().onKeyDown(keyCode, keyEvent)) {
			return true;
		} else {
			return super.onKeyDown(keyCode, keyEvent);
		}
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent keyEvent) {
		if (isSpecialKey(keyCode, keyEvent) && rebelView.getInputHandler().onKeyUp(keyCode, keyEvent)) {
			return true;
		} else {
			return super.onKeyUp(keyCode, keyEvent);
		}
	}

	private boolean isSpecialKey(int keyCode, KeyEvent keyEvent) {
		boolean isArrowKey = keyCode == KeyEvent.KEYCODE_DPAD_UP || keyCode == KeyEvent.KEYCODE_DPAD_DOWN ||
				keyCode == KeyEvent.KEYCODE_DPAD_LEFT || keyCode == KeyEvent.KEYCODE_DPAD_RIGHT;
		boolean isModifiedKey = keyEvent.isAltPressed() || keyEvent.isCtrlPressed() || keyEvent.isSymPressed() ||
				keyEvent.isFunctionPressed() || keyEvent.isMetaPressed();
		return isArrowKey || keyCode == KeyEvent.KEYCODE_TAB || KeyEvent.isModifierKey(keyCode) ||
				isModifiedKey;
	}

	// ===========================================================
	// Methods
	// ===========================================================
	public void showKeyboard(String p_existing_text, boolean p_multiline, int p_max_input_length, int p_cursor_start, int p_cursor_end) {
		int maxInputLength = (p_max_input_length <= 0) ? Integer.MAX_VALUE : p_max_input_length;
		if (p_cursor_start == -1) { // cursor position not given
			this.mOriginText = p_existing_text;
			this.mMaxInputLength = maxInputLength;
		} else if (p_cursor_end == -1) { // not text selection
			this.mOriginText = p_existing_text.substring(0, p_cursor_start);
			this.mMaxInputLength = maxInputLength - (p_existing_text.length() - p_cursor_start);
		} else {
			this.mOriginText = p_existing_text.substring(0, p_cursor_end);
			this.mMaxInputLength = maxInputLength - (p_existing_text.length() - p_cursor_end);
		}

		this.mMultiline = p_multiline;

		final Message msg = new Message();
		msg.what = HANDLER_OPEN_IME_KEYBOARD;
		msg.obj = this;
		msg.arg1 = p_cursor_start;
		msg.arg2 = p_cursor_end;
		editHandler.sendMessage(msg);
	}

	public void hideKeyboard() {
		final Message msg = new Message();
		msg.what = HANDLER_CLOSE_IME_KEYBOARD;
		msg.obj = this;
		editHandler.sendMessage(msg);
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
