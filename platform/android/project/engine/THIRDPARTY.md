# Third-party libraries

This file list third-party libraries used in the Android source folder,
with their provenance and, when relevant, modifications made to those files.

## com.google.android.vending.expansion.downloader

- Upstream: https://github.com/google/play-apk-expansion/tree/master/apkx_library
- Version: git (9ecf54e, 2017)
- License: Apache 2.0

Overwrite all files under:

- `src/main/java/com/google/android/vending/expansion/downloader`

Some files have been modified for yet unclear reasons.
See the `patches/com.google.android.vending.expansion.downloader.patch` file.

## com.google.android.vending.licensing

- Upstream: https://github.com/google/play-licensing/tree/master/lvl_library/
- Version: git (eb57657, 2018) with modifications
- License: Apache 2.0

Overwrite all files under:

- `src/main/aidl/com/android/vending/licensing`
- `src/main/java/com/google/android/vending/licensing`

Some files have been modified to silence linter errors or fix downstream issues.
See the `patches/com.google.android.vending.licensing.patch` file.
