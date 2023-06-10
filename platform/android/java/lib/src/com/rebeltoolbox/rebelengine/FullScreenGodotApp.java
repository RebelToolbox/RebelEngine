/*************************************************************************/
/*  FullScreenGodotApp.java                                              */
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

import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.CallSuper;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;

/**
 * Base activity for Android apps intending to use a Rebel Game as the primary and only screen.
 * It's also a reference implementation for how to setup and use the {@link RebelFragment}
 * within an Android app.
 */
public abstract class FullScreenGodotApp extends FragmentActivity implements RebelHost {
	private static final String TAG = FullScreenGodotApp.class.getSimpleName();

	@Nullable
	private RebelFragment rebelFragment;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.godot_app_layout);

		Fragment currentFragment = getSupportFragmentManager().findFragmentById(R.id.godot_fragment_container);
		if (currentFragment instanceof RebelFragment) {
			Log.v(TAG, "Reusing existing Rebel Fragment.");
			rebelFragment = (RebelFragment)currentFragment;
		} else {
			Log.v(TAG, "Creating new Rebel Fragment.");
			rebelFragment = createRebelFragment();

			getSupportFragmentManager().beginTransaction().replace(R.id.godot_fragment_container, rebelFragment).setPrimaryNavigationFragment(rebelFragment).commitNowAllowingStateLoss();
		}
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		onGodotForceQuit(rebelFragment);
	}

	@Override
	public final void onGodotForceQuit(RebelFragment quitFragment) {
		if (rebelFragment == quitFragment) {
			System.exit(0);
		}
	}

	@Override
	public final void onGodotRestartRequested(RebelFragment restartFragment) {
		if (rebelFragment == restartFragment) {
			// HACK:
			//
			// Currently it's very hard to properly deinitialize Godot on Android to restart the game
			// from scratch. Therefore, we need to kill the whole app process and relaunch it.
			//
			// Restarting only the activity, wouldn't be enough unless it did proper cleanup (including
			// releasing and reloading native libs or resetting their state somehow and clearing statics).
			//
			// Using instrumentation is a way of making the whole app process restart, because Android
			// will kill any process of the same package which was already running.
			//
			Bundle args = new Bundle();
			args.putParcelable("intent", getIntent());
			startInstrumentation(new ComponentName(this, GodotInstrumentation.class), null, args);
		}
	}

	@Override
	public void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		if (rebelFragment != null) {
			rebelFragment.onNewIntent(intent);
		}
	}

	@CallSuper
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (rebelFragment != null) {
			rebelFragment.onActivityResult(requestCode, resultCode, data);
		}
	}

	@CallSuper
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		if (rebelFragment != null) {
			rebelFragment.onRequestPermissionsResult(requestCode, permissions, grantResults);
		}
	}

	@Override
	public void onBackPressed() {
		if (rebelFragment != null) {
			rebelFragment.onBackPressed();
		} else {
			super.onBackPressed();
		}
	}

	/**
	 * Used to initialize the Godot fragment instance in {@link FullScreenGodotApp#onCreate(Bundle)}.
	 */
	@NonNull
	protected RebelFragment createRebelFragment() {
		return new RebelFragment();
	}

	@Nullable
	protected final RebelFragment getRebelFragment() {
		return rebelFragment;
	}
}
