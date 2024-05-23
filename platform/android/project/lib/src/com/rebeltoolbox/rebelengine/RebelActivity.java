// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

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
 * Base activity for Android apps intending to use a Rebel Game as the primary
 * and only screen. It's also a reference implementation for how to setup and
 * use the {@link RebelFragment} within an Android app.
 */
public abstract class RebelActivity
    extends FragmentActivity implements RebelHost {
    private static final String TAG = RebelActivity.class.getSimpleName();

    @Nullable
    private RebelFragment rebelFragment;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.rebel_fragment);

        Fragment currentFragment =
            getSupportFragmentManager().findFragmentById(R.id.rebel_fragment);
        if (currentFragment instanceof RebelFragment) {
            Log.v(TAG, "Reusing existing Rebel Fragment.");
            rebelFragment = (RebelFragment)currentFragment;
        } else {
            Log.v(TAG, "Creating new Rebel Fragment.");
            rebelFragment = createRebelFragment();

            getSupportFragmentManager()
                .beginTransaction()
                .replace(R.id.rebel_fragment, rebelFragment)
                .setPrimaryNavigationFragment(rebelFragment)
                .commitNowAllowingStateLoss();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        onQuit(rebelFragment);
    }

    @Override
    public final void onQuit(RebelFragment quitFragment) {
        if (rebelFragment == quitFragment) {
            System.exit(0);
        }
    }

    @Override
    public final void onRestart(RebelFragment restartFragment) {
        if (rebelFragment == restartFragment) {
            // HACK:
            //
            // Currently it's very hard to properly terminate Rebel Engine on
            // Android to restart the game from scratch. Therefore, we need to
            // kill the whole app process and relaunch it.
            //
            // Restarting only the activity, wouldn't be enough unless it did
            // proper cleanup (including releasing and reloading native libs or
            // resetting their state somehow and clearing statics).
            //
            // Using instrumentation is a way of making the whole app process
            // restart, because Android will kill any process of the same
            // package which was already running.
            //
            Bundle args = new Bundle();
            args.putParcelable("intent", getIntent());
            startInstrumentation(
                new ComponentName(this, RebelInstrumentation.class),
                null,
                args
            );
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
    public void onRequestPermissionsResult(
        int requestCode,
        String[] permissions,
        int[] grantResults
    ) {
        super
            .onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (rebelFragment != null) {
            rebelFragment.onRequestPermissionsResult(
                requestCode,
                permissions,
                grantResults
            );
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
     * Used to initialize the Rebel Engine instance in {@link
     * RebelActivity#onCreate(Bundle)}.
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
