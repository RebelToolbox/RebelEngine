# Third party libraries


## bullet

- Project: https://pybullet.org/wordpress/
- Source: https://github.com/bulletphysics/bullet3
- Version: 3.25 (2c204c49e56ed15ec5fcfa71d199ab6d6570b3f5, April 2022)
- License: zlib

Files copied from the library source:
- `src/*` apart from CMakeLists.txt and premake4.lua files
- `LICENSE.txt`, and `VERSION` as `VERSION.txt`

Note: Includes fixes sent upstream.
Patch files for the changes are included in the patches/ folder.


## certs

- Project: https://curl.se/docs/caextract.html
- Source: https://github.com/bagder/ca-bundle
- Commit: c5a419971b1bec220368c619aaafd0b818aa119f (March 2024)
- License: MPL 2.0

Files copied from the library source:
- ca-bundle.crt


## cvtt

- Project: https://github.com/microsoft/DirectXTex
- Source: https://github.com/elasota/cvtt
- Version: 1.0.0-beta4 (cc8472a04ba110fe999c686d07af40f7839051fd, August 2018)
- License: MIT

Files copied from the library source:
- all .cpp, .h, and .txt files in ConvectionKernels/


## embree

- Project: https://www.embree.org/
- Source: https://github.com/RenderKit/embree
- Version: 4.4.0 (ff9381774dc99fea81a932ad276677aad6a3d4dd, April 2025)
- License: Apache 2.0

Files copied from the library source:
- All cpp files listed in `modules/raycast/update_embree.py`
- All header files in the directories listed in `modules/raycast/update_embree.py`
The `modules/raycast/update_embree.py` script is used to pull the
relevant files from the latest Embree release and apply some automatic changes.

Note: Includes changes required to remove exceptions and fix minor build errors.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
Patch files for the changes are included in the patches/ folder.


## enet

- Project: http://sauerbraten.org/enet/
- Source: https://github.com/lsalzman/enet
- Version: 1.3.18 (2662c0de09e36f2a2030ccc2c528a3e4c9e8138a, April 2024)
- License: MIT

Files copied from the library source:
- all .c files in the root directory (except unix.c win32.c)
- all .h files in the include/enet/ folder (except unix.h win32.h) into enet/ folder
- LICENSE file

Note: Includes changes to use Rebel sockets and support IPv6.
Patch files for the changes are included in the patches/ folder.


## etc2comp

- Source: https://github.com/google/etc2comp
- Commit: 9cd0f9cae0f32338943699bb418107db61bb66f2 (April 2017)
- License: Apache 2.0

Files copied from the library source:
- All .cpp and .h files from EtcLib/Etc/ and EtcLib/EtcCodec/
- EtcFile.cpp, EtcFile.h, EtcFileHeader.cpp, EtcFileHeader.h from EtcTool/
- LICENSE

Note: Includes two fixes.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
Patch files for the changes are included in the patches/ folder.


## fonts

- DroidSans
  * Source: https://android.googlesource.com/platform/frameworks/base/+/master/data/fonts/
  * Version: Unknown
  * License: Apache 2.0
  Fonts:
  * DroidSansFallback.ttf
  * DroidSansJapanese.ttf

- Hack
  * Project: https://sourcefoundry.org/hack/
  * Source: https://github.com/source-foundry/Hack
  * Version: 3.003 (March 2018)
  * License: MIT + Bitstream Vera License
  Fonts:
  * Hack_Regular.ttf`:

- Noto
  * Project: https://fonts.google.com/noto
  * Source: https://github.com/googlei18n/noto-fonts
  * Version: 1.06 (2017)
  * License: OFL-1.1
  Fonts:
  * NotoNaskhArabicUI_Regular.ttf,
  * NotoSansDevanagariUI_Regular.ttf,
  * NotoSansHebrew_Regular.ttf,
  * NotoSansThaiUI_Regular.ttf,
  * NotoSansUI_Bold.ttf,
  * NotoSansUI_Regular.ttf


## freetype

- Project: https://freetype.org/
- Source: https://gitlab.freedesktop.org/freetype/freetype
- Version: 2.13.2 (42608f77f20749dd6ddc9e0536788eaad70ea4b5, August 2023)
- License: FreeType License (BSD-like)

Files copied from the library source:
- the `src/` folder except for the `*.mk` Makefiles and the `tools` subfolder
- the `include/` folder
- `docs/FTL.TXT
- `LICENSE.TXT`


## glad

- Project: https://gen.glad.sh/
- Source: https://github.com/Dav1dde/glad
- Version: 0.1.34 (a5ca31c88a4cc5847ea012629aff3690f261c7c4, September 2020)
- License: MIT

Note: The packaged files are automatically generated.
See the header of glad.c for instructions on how to generate them.


## jpeg-compressor

- Source: https://github.com/richgel999/jpeg-compressor
- Commit: aeb7d3b463aa8228b87a28013c15ee50a7e6fcf3 (May 2020)
- License: Public domain or MIT

Files copied from the library source:
- `jpgd*.{c,h}`


## libogg

- Project: https://www.xiph.org/ogg/
- Source: https://gitlab.xiph.org/xiph/ogg
- Version: 1.3.5 (e1774cd77f471443541596e09078e78fdc342e4f, June 2021)
- License: BSD-3-Clause

Files copied from the library source:
- `src/*.{c,h}`
- `include/ogg/*.h` in `ogg/` (run `configure` to generate `config_types.h`)
- `COPYING`


## libpng

- Project: https://libpng.org/pub/png/libpng.html
- Source: https://sourceforge.net/projects/libpng/
- Version: 1.6.53 (4e3f57d50f552841550a36eabbb3fbcecacb7750, December 2025)
- License: libpng/zlib

Files copied from the library source:
- all .c and .h files of the main directory, except `example.c` and `pngtest.c`
- the arm/ folder
- `scripts/pnglibconf.h.prebuilt` as `pnglibconf.h`
- `LICENSE`


## libsquish

- Source: https://sourceforge.net/projects/libsquish
- Version: 1.15 (r104, March 2017)
- License: MIT

Files copied from the library source:
- all .cpp, .h and .inl files

Note: Includes Rebel changes.
They are marked with `// -- REBEL start --` and `// -- REBEL end --`.
Patch file for the changes is included in the patches/ folder.


## libtheora

- Project: https://www.theora.org/
- Source: https://gitlab.xiph.org/xiph/theora
- Version: 1.1.1 (7ffd8b2ecfc2d93ae5e16028e7528e609266bfbf, Ocotober 2009)
- License: BSD-3-Clause

Files copied from the library source:
- all .c, .h in lib/
- all .h files in include/theora/ as theora/
- COPYING and LICENSE

Note: Includes Rebel changes.
Patch file for the changes is included in the patches/ folder.


## libvorbis

- Project: https://xiph.org/vorbis/
- Source: https://gitlab.xiph.org/xiph/vorbis
- Version: 1.3.7 (0657aee69dec8508a0011f47f3b69d7538e9d262, July 2020)
- License: BSD-3-Clause

Files copied from the library source:
- `lib/*` except from: `lookups.pl`, `Makefile.*`
- `include/vorbis/*.h` as `vorbis/`
- `COPYING`


## libwebp

- Project: https://developers.google.com/speed/webp
- Source: https://github.com/webmproject/libwebp
- Version: 1.6.0 (4fa21912338357f89e4fd51cf2368325b59e9bd9, July 2025)
- License: BSD-3-Clause

Files copied from the library source:
- `sharpyuv/*` except `.am`, `.rc` and `.in` files
- `src/*` except `.am`, `.rc` and `.in` files
- `COPYING`, `PATENTS`

Note: Includes changes required to support the web platform.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
Patch file for the changes is included in the patches/ folder.


## mbedtls

- Project: https://www.trustedfirmware.org/projects/mbed-tls/
- Source: https://github.com/Mbed-TLS/mbedtls
- Version: 3.6.5 (e185d7fd85499c8ce5ca2a54f5cf8fe7dbe3f8df, October 2025)
- License: Apache 2.0

Files copied from the library source:
- All `*.h` from `include/`
- All `*.c` and `*.h` from `library/`
- The `LICENSE` file.


## minimp3

- Source: https://github.com/lieff/minimp3
- Version: git (afb604c06bc8beb145fecd42c0ceb5bda8795144, November 2021)
- License: CC0 1.0

Files copied from the library source:
- `minimp3.h`
- `minimp3_ex.h`
- `LICENSE`


## MiniUPnP

- Project: http://miniupnp.free.fr/
- Source: https://github.com/miniupnp/miniupnp
- Version: 2.3.3 (bf4215a7574f88aa55859db9db00e3ae58cf42d6, May 2025)
- License: BSD-3-Clause

Files copied from the library source:
- All files from miniupnpc/include/ directory
- All files from miniupnpc/src/ directory except listdevices.c, minihttptestserver.c, miniupnpcmodule.c, minixmlvalid.c, upnpc.c, upnperrors.c, and all test* files.
- LICENSE

Files created from make and modified for Rebel Engine:
- miniupnpc/include/miniupnpcstrings.h


## minizip

- Project: https://www.zlib.net
- Source: https://github.com/madler/zlib
- Version: 1.3.1 (51b7f2abdade71cd9bb0e7a373ef2610ec6f9daf, January 2024)
- License: zlib

Files copied from the library source:
- contrib/minizip/crypt.h
- contrib/minizip/ioapi.c
- contrib/minizip/ioapi.h
- contrib/minizip/unzip.c
- contrib/minizip/unzip.h
- contrib/minizip/zip.c
- contrib/minizip/zip.h

Note: Includes changes required to support additional Rebel functionality.
They are marked with `// Rebel changes start.` and `// Rebel changes end.`
Patch files for the changes are included in the patches/ folder.


## misc

Collection of single-file libraries used in Rebel Engine.

- `clipper.{cpp,hpp}`
  * Source: https://sourceforge.net/projects/polyclipping
  * Version: 6.4.2 (2017)
  * License: BSL-1.0
  Note: Includes Rebel changes in `clipper-exceptions.patch`.

- `easing_equations.cpp`
  * Project: https://robertpenner.com/easing/
  * Source: https://github.com/jesusgollonet/ofpennereasing
  * Commit: af72c147c3a74e7e872aa28c7e2abfcced04fdce (November 2008)
  * License: BSD-3-Clause
  Note: Includes Rebel types and style changes.

- `fastlz.{c,h}`
  * Project: https://ariya.github.io/FastLZ/
  * Source: https://github.com/ariya/FastLZ
  * Version: 0.5.0 (4f20f54d46f5a6dd4fae4def134933369b7602d2, February 2020)
  * License: MIT

- `hq2x.{cpp,h}`
  * Source: https://github.com/brunexgeek/hqx
  * License: Apache 2.0

- `ifaddrs-android.{cc,h}`
  * Source: https://chromium.googlesource.com/external/webrtc/stable/talk/+/master/base
  * Commit: 5976650443d68ccfadf1dea24999ee459dd2819d (July 2013)
  * License: BSD-3-Clause
  Note: Includes Rebel changes in `ifaddrs-android.patch`.

- `mikktspace.{c,h}`
  * Project: http://www.mikktspace.com/
  * Source: https://github.com/mmikk/MikkTSpace
  * Version: 1.0 (2011)
  * License: zlib

- `open-simplex-noise.{c,h}`
  * Source: https://github.com/smcameron/open-simplex-noise-in-c
  * Commit: 826f1dd1724e6fb3ff45f58e48c0fbae864c3403 (December 2020)
  * License: Public Domain or Unlicense
  Note: Includes Rebel changes in `open-simplex-noise-no-allocate.patch`

- `pcg.{cpp,h}`
  * Project: https://www.pcg-random.org/
  * Source: https://github.com/imneme/pcg-cpp
  * License: MIT or Apache 2.0

- `smaz.{c,h}`
  * Source: https://github.com/antirez/smaz
  * Commit: 2f625846a775501fb69456567409a8b12f10ea25, February 2012)
  * License: BSD-3-Clause
  * Modifications: use `const char*` instead of `char*` for input string

- `stb_vorbis.c`
  * Source: https://github.com/nothings/stb
  * Version: 1.20 (314d0a6f9af5af27e585336eecea333e95c5a2d8, July 2020)
  * License: Public Domain (Unlicense) or MIT

- `triangulator.{cpp,h}`
  * Source: https://github.com/ivanfratric/polypartition
  * License: MIT

- `yuv2rgb.h`
  * Project: http://wss.co.uk/pinknoise/yuv2rgb/
  * License: BSD


## nanosvg

- Source: https://github.com/memononen/nanosvg
- Commit: ccdb1995134d340a93fb20e3a3d323ccb3838dd0 (September 2021)
- License: zlib

Files copied from the library source:
- All .h files in `src/`
- LICENSE.txt


## oidn

- Project: https://www.openimagedenoise.org/
- Source: https://github.com/RenderKit/oidn
- Version: 1.1.0 (c58c5216db05ceef4cde5a096862f2eeffd14c06, October 2019)
- License: Apache 2.0

Files copied from the library source:
- common/* (except tasking.* and CMakeLists.txt)
- core/*
- include/OpenImageDenoise/* (except version.h.in)
- mkl-dnn/include/*
- mkl-dnn/src/* (except CMakeLists.txt)
- weights/rtlightmap_hdr.tza
- LICENSE.txt

Note: Includes Rebel changes.
Patch files for the changes are included in the patches/ folder.


## pcre2

- Project: https://pcre2project.github.io/pcre2/
- Source: https://github.com/PCRE2Project/pcre2
- Version: 10.36 (r1288, 2020)
- License: BSD-3-Clause

Files copied from the library source:
- Files listed in the file NON-AUTOTOOLS-BUILD steps 1-4
- All .h files in src/ apart from pcre2posix.h
- src/pcre2_jit_match.c
- src/pcre2_jit_misc.c
- src/sljit/
- AUTHORS and LICENCE


## pvrtccompressor

- Source: https://github.com/LibreGamesArchive/PVRTCCompressor
- Version: hg (cf7177748ee0dcdccfe89716dc11a47d2dc81af5, 2015)
- License: BSD-3-Clause

Files copied from the library source:
- all .cpp and .h files apart from `main.cpp`
- LICENSE.TXT


## recastnavigation

- Project: https://recastnav.com/
- Source: https://github.com/recastnavigation/recastnavigation
- Version: git (57610fa6ef31b39020231906f8c5d40eaa8294ae, 2019)
- License: zlib

Files copied from the library source:
- `Recast/` folder without `CMakeLists.txt`
- License.txt


## tinyexr

- Source: https://github.com/syoyo/tinyexr
- Version: 1.0.0 (e4b7840d9448b7d57a88384ce26143004f3c0c71, 2020)
- License: BSD-3-Clause

Files copied from the library source:
- `tinyexr.{cc,h}`


## vhacd

- Source: https://github.com/kmammou/v-hacd
- Version: 4.1
- License: BSD-3-Clause

Files copied from the library source:
- `include`
- `LICENSE`

Note: Includes fixes sent upstream.
Patch files for the changes are included in the patches/ folder.


## wslay

- Project: https://tatsuhiro-t.github.io/wslay/
- Source: https://github.com/tatsuhiro-t/wslay
- Version: 1.1.1 (c9a84aa6df8512584c77c8cd15be9536b89c35aa, 2020)
- License: MIT

Files copied from the library source:
- All `*.c` and `*.h` in `lib/` and `lib/includes/`

Note: Includes fix.
Patch file for the changes is `msvcfix.diff`.


## xatlas

- Source: https://github.com/jpcy/xatlas
- Version: git (5571fc7ef0d06832947c0a935ccdcf083f7a9264, 2020)
- License: MIT

Files copied from the library source:
- `xatlas.{cpp,h}`
- `LICENSE`


## zlib

- Source: https://www.zlib.net
- Version: 1.3.1 (2024)
- License: zlib

Files copied from the library source:
- All .c and .h files from the root directory.


## zstd

- Project: https://facebook.github.io/zstd/
- Source: https://github.com/facebook/zstd
- Version: 1.4.8 (97a3da1df009d4dc67251de0c4b1c9d7fe286fc1, 2020)
- License: BSD-3-Clause

Files copied from the library source:
- lib/{common/,compress/,decompress/,zstd.h}
- LICENSE
