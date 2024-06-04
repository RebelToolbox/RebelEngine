import os
import sys
import platform
import subprocess


def is_active():
    return True


def get_name():
    return "Android"


def can_build():
    return os.path.exists(get_env_android_sdk_root())


def get_opts():
    from SCons.Variables import BoolVariable, EnumVariable

    return [
        EnumVariable(
            "android_arch",
            "Target architecture",
            "arm64v8",
            ("armv7", "arm64v8", "x86", "x86_64"),
        ),
        (
            "android_ndk_api",
            "Android NDK API level (minimum SDK supported)",
            21,
        ),
        BoolVariable(
            "android_neon",
            "Set to False to disable NEON support (armv7 only)",
            True,
        ),
    ]


# Return the ANDROID_SDK_ROOT environment variable.
def get_env_android_sdk_root():
    return os.environ.get("ANDROID_SDK_ROOT", -1)


def get_android_ndk_root():
    return get_env_android_sdk_root() + "/ndk/" + get_ndk_version()


def get_ndk_version():
    # Also update platform/android/project/app/config.gradle ndkVersion
    return "26.2.11394342"


def get_flags():
    return [
        ("tools", False),
    ]


def install_ndk_if_needed():
    print("Checking for Android NDK...")
    if not os.path.exists(get_android_ndk_root()):
        extension = ".bat" if os.name == "nt" else ""
        sdkmanager = (
            get_env_android_sdk_root()
            + "/cmdline-tools/latest/bin/sdkmanager"
            + extension
        )
        if os.path.exists(sdkmanager):
            # Install the Android NDK
            print("Installing Android NDK...")
            ndk_download_args = "ndk;" + get_ndk_version()
            subprocess.check_call([sdkmanager, ndk_download_args])
        else:
            print("ERROR: Cannot find " + sdkmanager)
            print(
                "Please ensure ANDROID_SDK_ROOT is correct and cmdline-tools are installed."
            )
            print(
                "Alternatively, install NDK version " + get_ndk_version() + " manually."
            )
            sys.exit()


def configure(env):
    install_ndk_if_needed()
    ndk_api_level = int(env["android_ndk_api"])
    if ndk_api_level < 21:
        print("WARNING: Minimum Android NDK API level is 21")
        ndk_api_level = 21
    print("Building for minimum Android SDK API level " + str(ndk_api_level))
    if ndk_api_level >= 24:
        env.Append(CPPDEFINES=["_FILE_OFFSET_BITS", 64])

    # Architecture
    if env["android_arch"] not in ["armv7", "arm64v8", "x86", "x86_64"]:
        print("ERROR: Unrecognised android arch: " + env["android_arch"])
        sys.exit()
    neon_text = ""
    if env["android_arch"] == "armv7" and env["android_neon"]:
        neon_text = " with NEON"
    print("Building for Android (" + env["android_arch"] + neon_text + ")")

    if env["android_arch"] == "armv7":
        triple = "arm-linux-androideabi"
        target_triple = "armv7a-linux-androideabi"
        if env["android_neon"]:
            env.extra_suffix = ".armv7.neon" + env.extra_suffix
        else:
            env.extra_suffix = ".armv7" + env.extra_suffix
    elif env["android_arch"] == "arm64v8":
        triple = "aarch64-linux-android"
        target_triple = triple
        env.extra_suffix = ".armv8" + env.extra_suffix
    elif env["android_arch"] == "x86":
        triple = "i686-linux-android"
        target_triple = triple
        env.extra_suffix = ".x86" + env.extra_suffix
    elif env["android_arch"] == "x86_64":
        triple = "x86_64-linux-android"
        target_triple = triple
        env.extra_suffix = ".x86_64" + env.extra_suffix
    target_triple = target_triple + str(ndk_api_level)
    env.Append(CCFLAGS=["-target", target_triple])
    env.Append(ASFLAGS=["-target", target_triple])
    env.Append(LINKFLAGS=["-target", target_triple])

    env["neon_enabled"] = False
    if env["android_arch"] == "armv7":
        if env["android_neon"]:
            env["neon_enabled"] = True
            env.Append(CCFLAGS=["-mfpu=neon"])
        else:
            env.Append(CCFLAGS=["-mfpu=vfpv3-d16"])
    elif env["android_arch"] == "arm64v8":
        env.Append(CCFLAGS=["-mfix-cortex-a53-835769"])
    elif env["android_arch"] == "x86":
        # For x86 targets prior to Android Nougat (API 24),
        # -mstackrealign is needed to properly align stacks for global constructors.
        env.Append(CCFLAGS=["-mstackrealign"])

    # Build type
    if env["target"].startswith("release"):
        if env["optimize"] == "speed":
            env.Append(CCFLAGS=["-O2", "-fomit-frame-pointer"])
        elif env["optimize"] == "size":
            env.Append(CCFLAGS=["-Oz"])
        env.Append(CPPDEFINES=["NDEBUG"])
        env.Append(CCFLAGS=["-ftree-vectorize"])
    elif env["target"] == "debug":
        env.Append(LINKFLAGS=["-O0"])
        env.Append(CCFLAGS=["-O0", "-g", "-fno-limit-debug-info"])
        env.Append(CPPDEFINES=["_DEBUG"])
        env.Append(CPPFLAGS=["-UNDEBUG"])

    # Compiler configuration
    env.Prepend(CPPPATH=["#platform/android"])
    env["SHLIBSUFFIX"] = ".so"

    if env["PLATFORM"] == "win32":
        env.use_windows_spawn_fix()

    if sys.platform.startswith("linux"):
        host_subpath = "linux-x86_64"
    elif sys.platform.startswith("darwin"):
        host_subpath = "darwin-x86_64"
    elif sys.platform.startswith("win"):
        if platform.machine().endswith("64"):
            host_subpath = "windows-x86_64"
        else:
            host_subpath = "windows"

    ndk_root = get_android_ndk_root()
    toolchain_path = ndk_root + "/toolchains/llvm/prebuilt/" + host_subpath
    compiler_path = toolchain_path + "/bin"
    libc_shared_path = toolchain_path + "/sysroot/usr/lib/" + triple

    env["CC"] = compiler_path + "/clang"
    env["CXX"] = compiler_path + "/clang++"
    env["AR"] = compiler_path + "/llvm-ar"
    env["RANLIB"] = compiler_path + "/llvm-ranlib"
    env["LIBC"] = libc_shared_path + "/libc++_shared.so"

    # Disable exceptions and rtti on non-tools (template) builds
    if env["tools"]:
        env.Append(CXXFLAGS=["-frtti"])
    else:
        env.Append(CXXFLAGS=["-fno-rtti", "-fno-exceptions"])
        # Don't use dynamic_cast, necessary with no-rtti.
        env.Append(CPPDEFINES=["NO_SAFE_CAST"])

    env.Append(
        CPPDEFINES=[
            "NO_STATVFS",
            "GLES_ENABLED",
            "ANDROID_ENABLED",
            "UNIX_ENABLED",
            "NO_FCNTL",
        ]
    )

    env.Append(
        CCFLAGS=[
            "-fpic",
            "-ffunction-sections",
            "-funwind-tables",
            "-fstack-protector-strong",
            "-fvisibility=hidden",
            "-fno-strict-aliasing",
        ]
    )

    env.Append(
        LINKFLAGS=[
            "-Wl,--gc-sections",
            "-Wl,--no-undefined",
            "-Wl,-z,now",
            "-Wl,-soname,librebel_android.so",
        ]
    )

    env.Append(
        LIBS=[
            "OpenSLES",
            "EGL",
            "GLESv3",
            "GLESv2",
            "android",
            "log",
            "z",
            "dl",
        ]
    )
