# Third party libraries

Please keep categories (`##` level) listed alphabetically and matching their
respective folder names. Use two empty lines to separate categories for
readability.


## bullet

- Upstream: https://github.com/bulletphysics/bullet3
- Version: 3.25
- License: zlib

Files extracted from upstream source:

- `src/*` apart from CMakeLists.txt and premake4.lua files
- `LICENSE.txt`, and `VERSION` as `VERSION.txt`


## certs

- Upstream: https://github.com/bagder/ca-bundle
- Commit: c5a419971b1bec220368c619aaafd0b818aa119f (March 2024)
- License: MPL 2.0

File extracted from upstream source:

- ca-bundle.crt

## cvtt

- Upstream: https://github.com/elasota/cvtt
- Version: 1.0.0-beta4 (cc8472a04ba110fe999c686d07af40f7839051fd, 2018)
- License: MIT

Files extracted from upstream source:

- all .cpp, .h, and .txt files in ConvectionKernels/


## embree

- Upstream: https://github.com/RenderKit/embree
- Version: 4.4.0 (ff9381774dc99fea81a932ad276677aad6a3d4dd, 2025)
- License: Apache 2.0

Files extracted from upstream:

- All cpp files listed in `modules/raycast/update_embree.py`
- All header files in the directories listed in `modules/raycast/update_embree.py`

The `modules/raycast/update_embree.py` script is used to pull the
relevant files from the latest Embree release and apply some automatic changes.

Note: Includes changes required to remove exceptions and fix minor build errors.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
Patch files for the changes are included in the patches folder.


## enet

- Upstream: https://github.com/lsalzman/enet
- Version: 1.3.18 (2662c0de09e36f2a2030ccc2c528a3e4c9e8138a, 2024)
- License: MIT

Files extracted from upstream source:
- all .c files in the root directory (except unix.c win32.c)
- all .h files in the include/enet/ folder (except unix.h win32.h) into enet/ folder
- LICENSE file

Note: Rebel Engine updates ENet to use Rebel sockets and support IPv6.
Rebel Enet patch file and additional Rebel socket implementation files:
- patches/rebel_enet.patch
- rebel_enet.cpp
- enet/rebel_enet.h
- enet/rebel_enet_ext.h


## etc2comp

- Upstream: https://github.com/google/etc2comp
- Version: git (9cd0f9cae0f32338943699bb418107db61bb66f2, 2017)
- License: Apache 2.0

Files extracted from upstream source:

- All .cpp and .h files from EtcLib/Etc/ and EtcLib/EtcCodec/
- EtcFile.cpp, EtcFile.h, EtcFileHeader.cpp, EtcFileHeader.h from EtcTool/
- LICENSE

Note: Includes two fixes.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
Patch files for the changes are included in the patches folder.


## fonts

- `NotoSans*.ttf`, `NotoNaskhArabicUI_Regular.ttf`:
  * Upstream: https://github.com/googlei18n/noto-fonts
  * Version: 1.06 (2017)
  * License: OFL-1.1
  * Comment: Use UI font variant if available, because it has tight vertical metrics and
    good for UI.
- `Hack_Regular.ttf`:
  * Upstream: https://github.com/source-foundry/Hack
  * Version: 3.003 (2018)
  * License: MIT + Bitstream Vera License
- `DroidSans*.ttf`:
  * Upstream: https://android.googlesource.com/platform/frameworks/base/+/master/data/fonts/
  * Version: ? (pre-2014 commit when DroidSansJapanese.ttf was obsoleted)
  * License: Apache 2.0


## freetype

- Upstream: https://gitlab.freedesktop.org/freetype/freetype
- Version: 2.13.2 (August 2023)
- License: FreeType License (BSD-like)

Files extracted from upstream source:

- the `src/` folder except for the `*.mk` Makefiles and the `tools` subfolder
- the `include/` folder
- `docs/FTL.TXT
- `LICENSE.TXT`


## glad

- Upstream: https://github.com/Dav1dde/glad
- Version: 0.1.34 (a5ca31c88a4cc5847ea012629aff3690f261c7c4, 2020)
- License: MIT

The files we package are automatically generated.
See the header of glad.c for instructions on how to generate them for
the GLES version Godot targets.


## jpeg-compressor

- Upstream: https://github.com/richgel999/jpeg-compressor
- Version: 2.00 (aeb7d3b463aa8228b87a28013c15ee50a7e6fcf3, 2020)
- License: Public domain or MIT

Files extracted from upstream source:

- `jpgd*.{c,h}`


## libogg

- Upstream: https://www.xiph.org/ogg
- Version: 1.3.5 (e1774cd77f471443541596e09078e78fdc342e4f, 2021)
- License: BSD-3-Clause

Files extracted from upstream source:

- `src/*.{c,h}`
- `include/ogg/*.h` in `ogg/` (run `configure` to generate `config_types.h`)
- `COPYING`


## libpng

- Upstream: http://libpng.org/pub/png/libpng.html
- Version: 1.6.50 (2025)
- License: libpng/zlib

Files extracted from upstream source:

- all .c and .h files of the main directory, except `example.c` and `pngtest.c`
- the arm/ folder
- `scripts/pnglibconf.h.prebuilt` as `pnglibconf.h`
- `LICENSE`


## libsquish

- Upstream: https://sourceforge.net/projects/libsquish
- Version: 1.15 (r104, 2017)
- License: MIT

Files extracted from upstream source:

- all .cpp, .h and .inl files

Important: Some files have Rebel changes.
They are marked with `// -- REBEL start --` and `// -- REBEL end --` comments.
A patch file is provided in the thirdparty/libsquish/ folder.


## libtheora

- Upstream: https://www.theora.org
- Version: 1.1.1 (2010)
- License: BSD-3-Clause

Files extracted from upstream source:

- all .c, .h in lib/
- all .h files in include/theora/ as theora/
- COPYING and LICENSE

Upstream patches included in the `patches` directory have been applied
on top of the 1.1.1 source (not included in any stable release yet).


## libvorbis

- Upstream: https://www.xiph.org/vorbis
- Version: 1.3.7 (0657aee69dec8508a0011f47f3b69d7538e9d262, 2020)
- License: BSD-3-Clause

Files extracted from upstream source:

- `lib/*` except from: `lookups.pl`, `Makefile.*`
- `include/vorbis/*.h` as `vorbis/`
- `COPYING`


## libwebp

- Upstream: https://github.com/webmproject/libwebp
- Version: 1.6.0 (4fa21912338357f89e4fd51cf2368325b59e9bd9, 2025)
- License: BSD-3-Clause

Files extracted from upstream source:

- `sharpyuv/*` except `.am`, `.rc` and `.in` files
- `src/*` except `.am`, `.rc` and `.in` files
- `COPYING`, `PATENTS`

Note: Includes changes required to support the web platform.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
The patch files for the changes is included in the patches folder.


## mbedtls

- Upstream: https://github.com/Mbed-TLS/mbedtls
- Version: 3.6.5 (e185d7fd85499c8ce5ca2a54f5cf8fe7dbe3f8df, 2025)
- License: Apache 2.0

File extracted from upstream release tarball:

- All `*.h` from `include/`
- All `*.c` and `*.h` from `library/`
- The `LICENSE` file.

## minimp3

- Upstream: https://github.com/lieff/minimp3
- Version: git (afb604c06bc8beb145fecd42c0ceb5bda8795144, 2021)
- License: CC0 1.0

Files extracted from upstream repository:

- `minimp3.h`
- `minimp3_ex.h`
- `LICENSE`


## MiniUPnP

- Upstream: https://github.com/miniupnp/
- Version: 2.3.3 (bf4215a7574f88aa55859db9db00e3ae58cf42d6, 2025)
- License: BSD-3-Clause

Files extracted from upstream source:
- All files from miniupnpc/include/ directory
- All files from miniupnpc/src/ directory except listdevices.c, minihttptestserver.c, miniupnpcmodule.c, minixmlvalid.c, upnpc.c, upnperrors.c, and all test* files.
- LICENSE

Files created from make and modified for Rebel Engine:
- miniupnpc/include/miniupnpcstrings.h


## minizip

- Upstream: https://www.zlib.net
- Version: 1.3.1 (2024)
- License: zlib

Files extracted from the upstream source:
- contrib/minizip/crypt.h
- contrib/minizip/ioapi.c
- contrib/minizip/ioapi.h
- contrib/minizip/unzip.c
- contrib/minizip/unzip.h
- contrib/minizip/zip.c
- contrib/minizip/zip.h

Note: Includes changes required to support additional Rebel functionality.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
Patch files for the changes are included in the patches folder.


## misc

Collection of single-file libraries used in Godot components.

- `clipper.{cpp,hpp}`
  * Upstream: https://sourceforge.net/projects/polyclipping
  * Version: 6.4.2 (2017) + Godot changes (added optional exceptions handling)
  * License: BSL-1.0
- `easing_equations.cpp`
  * Upstream: http://robertpenner.com/easing/ via https://github.com/jesusgollonet/ofpennereasing (modified to fit Godot types)
  * Version: git (af72c147c3a74e7e872aa28c7e2abfcced04fdce, 2008) + Godot types and style changes
  * License: BSD-3-Clause
- `fastlz.{c,h}`
  * Upstream: https://github.com/ariya/FastLZ
  * Version: 0.5.0 (4f20f54d46f5a6dd4fae4def134933369b7602d2, 2020)
  * License: MIT
- `hq2x.{cpp,h}`
  * Upstream: https://github.com/brunexgeek/hqx
  * Version: TBD, file structure differs
  * License: Apache 2.0
- `ifaddrs-android.{cc,h}`
  * Upstream: https://chromium.googlesource.com/external/webrtc/stable/talk/+/master/base/ifaddrs-android.h
  * Version: git (5976650443d68ccfadf1dea24999ee459dd2819d, 2013)
  * License: BSD-3-Clause
- `mikktspace.{c,h}`
  * Upstream: https://archive.blender.org/wiki/index.php/Dev:Shading/Tangent_Space_Normal_Maps/
  * Version: 1.0 (2011)
  * License: zlib
- `open-simplex-noise.{c,h}`
  * Upstream: https://github.com/smcameron/open-simplex-noise-in-c
  * Version: git (826f1dd1724e6fb3ff45f58e48c0fbae864c3403, 2020) + custom changes
  * License: Public Domain or Unlicense
- `pcg.{cpp,h}`
  * Upstream: http://www.pcg-random.org
  * Version: minimal C implementation, http://www.pcg-random.org/download.html
  * License: Apache 2.0
- `smaz.{c,h}`
  * Upstream: https://github.com/antirez/smaz
  * Version: git (2f625846a775501fb69456567409a8b12f10ea25, 2012)
  * License: BSD-3-Clause
  * Modifications: use `const char*` instead of `char*` for input string
- `stb_rect_pack.h`
  * Upstream: https://github.com/nothings/stb
  * Version: 1.00
  * License: Public Domain (Unlicense) or MIT
- `stb_vorbis.c`
  * Upstream: https://github.com/nothings/stb
  * Version: 1.20 (314d0a6f9af5af27e585336eecea333e95c5a2d8, 2020)
  * License: Public Domain or Unlicense or MIT
- `triangulator.{cpp,h}`
  * Upstream: https://github.com/ivanfratric/polypartition (`src/polypartition.cpp`)
  * Version: TBD, class was renamed
  * License: MIT
- `yuv2rgb.h`
  * Upstream: http://wss.co.uk/pinknoise/yuv2rgb/ (to check)
  * Version: ?
  * License: BSD


## nanosvg

- Upstream: https://github.com/memononen/nanosvg
- Version: git (ccdb1995134d340a93fb20e3a3d323ccb3838dd0, 2021)
- License: zlib

Files extracted from the upstream source:

- All .h files in `src/`
- LICENSE.txt


## oidn

- Upstream: https://github.com/RenderKit/oidn
- Version: 1.1.0 (c58c5216db05ceef4cde5a096862f2eeffd14c06, 2019)
- License: Apache 2.0

Files extracted from upstream source:

common/* (except tasking.* and CMakeLists.txt)
core/*
include/OpenImageDenoise/* (except version.h.in)
LICENSE.txt
mkl-dnn/include/*
mkl-dnn/src/* (except CMakeLists.txt)
weights/rtlightmap_hdr.tza
scripts/resource_to_cpp.py

Modified files:
Patch files are provided in `oidn/patches/`.

core/autoencoder.cpp
core/autoencoder.h
core/common.h
core/device.cpp
core/device.h
core/transfer_function.cpp

scripts/resource_to_cpp.py (used in modules/denoise/resource_to_cpp.py)


## pcre2

- Upstream: http://www.pcre.org
- Version: 10.36 (r1288, 2020)
- License: BSD-3-Clause

Files extracted from upstream source:

- Files listed in the file NON-AUTOTOOLS-BUILD steps 1-4
- All .h files in src/ apart from pcre2posix.h
- src/pcre2_jit_match.c
- src/pcre2_jit_misc.c
- src/sljit/
- AUTHORS and LICENCE


## pvrtccompressor

- Upstream: https://bitbucket.org/jthlim/pvrtccompressor (dead link)
  Unofficial backup fork: https://github.com/LibreGamesArchive/PVRTCCompressor
- Version: hg (cf7177748ee0dcdccfe89716dc11a47d2dc81af5, 2015)
- License: BSD-3-Clause

Files extracted from upstream source:

- all .cpp and .h files apart from `main.cpp`
- LICENSE.TXT


## recastnavigation

- Upstream: https://github.com/recastnavigation/recastnavigation
- Version: git (57610fa6ef31b39020231906f8c5d40eaa8294ae, 2019)
- License: zlib

Files extracted from upstream source:

- `Recast/` folder without `CMakeLists.txt`
- License.txt


## tinyexr

- Upstream: https://github.com/syoyo/tinyexr
- Version: 1.0.0 (e4b7840d9448b7d57a88384ce26143004f3c0c71, 2020)
- License: BSD-3-Clause

Files extracted from upstream source:

- `tinyexr.{cc,h}`


## vhacd

- Upstream: https://github.com/kmammou/v-hacd
- Version: 4.1
- License: BSD-3-Clause

Files extracted from upstream source:

- `include`
- `LICENSE`

Note: Includes the patches in the patches/ folder that have been applied
upstream, but are not part of version 4.1.


## wslay

- Upstream: https://github.com/tatsuhiro-t/wslay
- Version: 1.1.1 (c9a84aa6df8512584c77c8cd15be9536b89c35aa, 2020)
- License: MIT

File extracted from upstream release tarball:

- All `*.c` and `*.h` in `lib/` and `lib/includes/`
- `wslay.h` has a small Godot addition to fix MSVC build.
  See `thirdparty/wslay/msvcfix.diff`


## xatlas

- Upstream: https://github.com/jpcy/xatlas
- Version: git (5571fc7ef0d06832947c0a935ccdcf083f7a9264, 2020)
- License: MIT

Files extracted from upstream source:

- `xatlas.{cpp,h}`
- `LICENSE`


## zlib

- Upstream: https://www.zlib.net
- Version: 1.3.1 (2024)
- License: zlib

Files extracted from upstream source:

- all .c and .h files from the root directory


## zstd

- Upstream: https://github.com/facebook/zstd
- Version: 1.4.8 (97a3da1df009d4dc67251de0c4b1c9d7fe286fc1, 2020)
- License: BSD-3-Clause

Files extracted from upstream source:

- lib/{common/,compress/,decompress/,zstd.h}
- LICENSE
