import os
import sys
from methods import get_darwin_sdk_path, get_darwin_sdk_version


def is_active():
    return True


def get_name():
    return "iOS"


def can_build():
    if sys.platform == "darwin":
        if get_darwin_sdk_version("ios") < 13.0:
            print("Detected iOS SDK version older than 13")
            return False
        return True
    elif "OSXCROSS_IOS" in os.environ:
        return True

    return False


def get_opts():
    from SCons.Variables import BoolVariable

    return [
        (
            "xctoolchain",
            "Path to the xctoolchain directory",
            "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain",
        ),
        BoolVariable("ios_simulator", "Build for iOS Simulator", False),
        BoolVariable("ios_exceptions", "Enable exceptions", False),
        ("ios_triple", "Triple for ios toolchain", ""),
    ]


def get_flags():
    return [
        ("tools", False),
    ]


def configure(env):
    ## Architecture
    if env["bits"] == "32":
        print("iOS does not support 32 bit builds.")
        sys.exit()
    if not env["arch"]:
        env["arch"] = "arm64"
    supported_archs = ["x86_64", "arm64"]
    if env["arch"] not in supported_archs:
        print("iOS does not support arch=%s.", env["arch"])
        sys.exit()

    ## Build type
    if env["target"].startswith("release"):
        env.Append(CPPDEFINES=["NDEBUG", ("NS_BLOCK_ASSERTIONS", 1)])
        if env["optimize"] == "speed":  # optimize for speed (default)
            env.Append(CCFLAGS=["-O2", "-ftree-vectorize", "-fomit-frame-pointer"])
            env.Append(LINKFLAGS=["-O2"])
        elif env["optimize"] == "size":  # optimize for size
            env.Append(CCFLAGS=["-Os", "-ftree-vectorize"])
            env.Append(LINKFLAGS=["-Os"])
    elif env["target"] == "debug":
        env.Append(CCFLAGS=["-gdwarf-2", "-O0"])
        env.Append(CPPDEFINES=["_DEBUG", ("DEBUG", 1)])

    if env["use_lto"]:
        env.Append(CCFLAGS=["-flto"])
        env.Append(LINKFLAGS=["-flto"])

    ## Compiler configuration

    # Save this in environment for use by other modules
    if "OSXCROSS_IOS" in os.environ:
        env["osxcross"] = True

    env["ENV"]["PATH"] = (
        env["xctoolchain"] + "/Developer/usr/bin/:" + env["ENV"]["PATH"]
    )

    compiler_path = "$xctoolchain/usr/bin/${ios_triple}"
    s_compiler_path = "$xctoolchain/Developer/usr/bin/"

    ccache_path = os.environ.get("CCACHE")
    if ccache_path is None:
        env["CC"] = compiler_path + "clang"
        env["CXX"] = compiler_path + "clang++"
        env["S_compiler"] = s_compiler_path + "gcc"
    else:
        # there aren't any ccache wrappers available for iOS,
        # to enable caching we need to prepend the path to the ccache binary
        env["CC"] = ccache_path + " " + compiler_path + "clang"
        env["CXX"] = ccache_path + " " + compiler_path + "clang++"
        env["S_compiler"] = ccache_path + " " + s_compiler_path + "gcc"
    env["AR"] = compiler_path + "ar"
    env["RANLIB"] = compiler_path + "ranlib"

    ## Compile flags
    sdk_path = get_darwin_sdk_path("ios")
    if env["ios_simulator"]:
        sdk_path = get_darwin_sdk_path("ios_simulator")
        env.Append(CCFLAGS=["-mios-simulator-version-min=10.0"])
        env.Append(LINKFLAGS=["-mios-simulator-version-min=10.0"])
        env.extra_suffix = ".simulator" + env.extra_suffix
    else:
        env.Append(CCFLAGS=["-miphoneos-version-min=10.0"])
        env.Append(LINKFLAGS=["-miphoneos-version-min=10.0"])
    env.Append(CCFLAGS=["-isysroot", sdk_path])

    if env["arch"] == "x86_64":
        env["ENV"]["MACOSX_DEPLOYMENT_TARGET"] = "10.9"
        env.Append(
            CCFLAGS=(
                "-arch x86_64 "
                + "-fobjc-arc -fobjc-abi-version=2 -fobjc-legacy-dispatch "
                + "-fmessage-length=0 -fpascal-strings -fblocks -fasm-blocks "
            ).split()
        )
    else:  # env["arch"] == "arm64"
        env.Append(
            CCFLAGS=(
                "-arch arm64 "
                + "-fobjc-arc "
                + "-fmessage-length=0 -fpascal-strings -fblocks "
                + "-fno-strict-aliasing -fvisibility=hidden "
                + "-fdiagnostics-print-source-range-info "
                + "-fdiagnostics-show-category=id "
                + "-fdiagnostics-parseable-fixits "
                + "-MMD -MT dependencies "
            ).split()
        )
        env.Append(CPPDEFINES=["NEED_LONG_INT"])
        env.Append(CPPDEFINES=["LIBYUV_DISABLE_NEON"])

    # Disable exceptions on non-tools (template) builds
    if not env["tools"]:
        if env["ios_exceptions"]:
            env.Append(CCFLAGS=["-fexceptions"])
        else:
            env.Append(CCFLAGS=["-fno-exceptions"])

    # Temp fix for ABS/MAX/MIN macros in iPhone SDK blocking compilation
    env.Append(CCFLAGS=["-Wno-ambiguous-macro"])

    ## Link flags

    if env["arch"] == "x86_64":
        env.Append(
            LINKFLAGS=(
                "-arch x86_64 "
                + "-Xlinker -objc_abi_version "
                + "-Xlinker 2 "
                + f"-F{sdk_path}"
            ).split()
        )
    else:  # env["arch"] == "arm64"
        env.Append(LINKFLAGS=["-arch", "arm64", "-Wl,-dead_strip"])

    env.Append(LINKFLAGS=["-isysroot", sdk_path])

    env.Prepend(
        CPPPATH=[
            f"{sdk_path}/usr/include",
            f"{sdk_path}/System/Library/Frameworks/OpenGLES.framework/Headers",
            f"{sdk_path}/System/Library/Frameworks/AudioUnit.framework/Headers",
        ]
    )

    env["ENV"][
        "CODESIGN_ALLOCATE"
    ] = "/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/codesign_allocate"

    env.Prepend(CPPPATH=["#platform/ios"])
    env.Append(
        CPPDEFINES=[
            "IOS_ENABLED",
            "UNIX_ENABLED",
            "GLES_ENABLED",
            "COREAUDIO_ENABLED",
        ]
    )
