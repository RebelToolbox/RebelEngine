import os
import subprocess
import sys

from emscripten_helpers import (
    run_closure_compiler,
    create_engine_file,
    add_js_libraries,
    add_js_pre,
    add_js_externs,
    create_template_zip,
)
from methods import get_compiler_version
from SCons.Platform import TempFileMunge
from shutil import which


def is_active():
    return True


def get_name():
    return "Web"


def can_build():
    return True


def get_opts():
    from SCons.Variables import BoolVariable

    return [
        ("initial_memory", "Initial WASM memory (in MiB)", 32),
        BoolVariable("use_assertions", "Use Emscripten runtime assertions", False),
        BoolVariable("use_thinlto", "Use ThinLTO", False),
        BoolVariable(
            "use_ubsan", "Use Emscripten undefined behavior sanitizer (UBSAN)", False
        ),
        BoolVariable("use_asan", "Use Emscripten address sanitizer (ASAN)", False),
        BoolVariable("use_lsan", "Use Emscripten leak sanitizer (LSAN)", False),
        BoolVariable("use_safe_heap", "Use Emscripten SAFE_HEAP sanitizer", False),
        # eval() can be a security concern, so it can be disabled.
        BoolVariable("web_eval", "Enable Web eval interface", True),
        BoolVariable(
            "threads_enabled",
            "Enable WebAssembly Threads support (limited browser support)",
            False,
        ),
        BoolVariable(
            "gdnative_enabled",
            "Enable WebAssembly GDNative support (produces bigger binaries)",
            False,
        ),
        BoolVariable(
            "use_closure_compiler",
            "Use closure compiler to minimize JavaScript code",
            False,
        ),
        (
            "emsdk",
            "Path to emsdk",
            (
                os.environ["EMSDK"]
                if "EMSDK" in os.environ
                else os.path.join(os.getcwd(), "emsdk")
            ),
        ),
        (
            "emscripten_version",
            "Version of Emscripten to use.",
            "latest",
        ),
    ]


def get_flags():
    return [
        ("tools", False),
        ("builtin_pcre2_with_jit", False),
        # Disabling the mbedtls module reduces file size.
        # The module has little use due to the limited networking functionality
        # in this platform. For the available networking methods, the browser
        # manages TLS.
        ("module_mbedtls_enabled", False),
    ]


def configure(env):
    try:
        env["initial_memory"] = int(env["initial_memory"])
    except Exception:
        print("Initial memory must be a valid integer")
        sys.exit(255)

    prepare_web(env)
    if is_emscripten_active(env):
        print("Building with active Emscripten")
    else:
        print("Building with Emscripten " + env["emscripten_version"] + ".")
    if get_host_platform() == "windows":
        use_response_files(env)

    ## Build type
    if env["target"].startswith("release"):
        # Use -Os to prioritize optimizing for reduced file size. This is
        # particularly valuable for the web platform because it directly
        # decreases download time.
        # -Os reduces file size by around 5 MiB over -O3. -Oz only saves about
        # 100 KiB over -Os, which does not justify the negative impact on
        # run-time performance.
        if env["optimize"] != "none":
            env.Append(CCFLAGS=["-Os"])
            env.Append(LINKFLAGS=["-Os"])

        if env["target"] == "release_debug":
            # Retain function names for backtraces at the cost of file size.
            env.Append(LINKFLAGS=["--profiling-funcs"])
    else:  # "debug"
        env.Append(CCFLAGS=["-O1", "-g"])
        env.Append(LINKFLAGS=["-O1", "-g"])
        env["use_assertions"] = True

    if env["use_assertions"]:
        env.Append(LINKFLAGS=["-s", "ASSERTIONS=1"])

    if env["tools"]:
        if not env["threads_enabled"]:
            print(
                "Threads must be enabled to build the editor. Please add the 'threads_enabled=yes' option"
            )
            sys.exit(255)
        if env["initial_memory"] < 64:
            print("Editor build requires at least 64MiB of initial memory. Forcing it.")
            env["initial_memory"] = 64
    else:
        # Disable exceptions and rtti on non-tools (template) builds
        # These flags help keep the file size down.
        env.Append(CCFLAGS=["-fno-exceptions", "-fno-rtti"])
        # Don't use dynamic_cast, necessary with no-rtti.
        env.Append(CPPDEFINES=["NO_SAFE_CAST"])

    env.Append(LINKFLAGS=["-s", "INITIAL_MEMORY=%sMB" % env["initial_memory"]])

    # LTO
    if env["use_thinlto"]:
        env.Append(CCFLAGS=["-flto=thin"])
        env.Append(LINKFLAGS=["-flto=thin"])
    elif env["use_lto"]:
        env.Append(CCFLAGS=["-flto=full"])
        env.Append(LINKFLAGS=["-flto=full"])

    # Sanitizers
    if env["use_ubsan"]:
        env.Append(CCFLAGS=["-fsanitize=undefined"])
        env.Append(LINKFLAGS=["-fsanitize=undefined"])
    if env["use_asan"]:
        env.Append(CCFLAGS=["-fsanitize=address"])
        env.Append(LINKFLAGS=["-fsanitize=address"])
    if env["use_lsan"]:
        env.Append(CCFLAGS=["-fsanitize=leak"])
        env.Append(LINKFLAGS=["-fsanitize=leak"])
    if env["use_safe_heap"]:
        env.Append(LINKFLAGS=["-s", "SAFE_HEAP=1"])

    # Closure compiler
    if env["use_closure_compiler"]:
        # For emscripten support code.
        env.Append(LINKFLAGS=["--closure", "1"])
        # Register builder for our Engine files
        jscc = env.Builder(
            generator=run_closure_compiler, suffix=".cc.js", src_suffix=".js"
        )
        env.Append(BUILDERS={"BuildJS": jscc})

    # Add helper method for adding libraries, externs, pre-js.
    env["JS_LIBS"] = []
    env["JS_PRE"] = []
    env["JS_EXTERNS"] = []
    env.AddMethod(add_js_libraries, "AddJSLibraries")
    env.AddMethod(add_js_pre, "AddJSPre")
    env.AddMethod(add_js_externs, "AddJSExterns")

    # Add method that joins/compiles our Engine files.
    env.AddMethod(create_engine_file, "CreateEngineFile")

    # Add method for creating the final zip file
    env.AddMethod(create_template_zip, "CreateTemplateZip")

    # Closure compiler extern and support for ecmascript specs (const, let, etc).
    env["ENV"]["EMCC_CLOSURE_ARGS"] = "--language_in ECMASCRIPT6"

    env["CC"] = "emcc"
    env["CXX"] = "em++"
    env["AR"] = "emar"
    env["RANLIB"] = "emranlib"

    # Use TempFileMunge since some AR invocations are too long for cmd.exe.
    # Use POSIX-style paths, required with TempFileMunge.
    env["ARCOM_POSIX"] = (
        env["ARCOM"]
        .replace("$TARGET", "$TARGET.posix")
        .replace("$SOURCES", "$SOURCES.posix")
    )
    env["ARCOM"] = "${TEMPFILE(ARCOM_POSIX)}"

    # Program() output consists of multiple files, so specify suffixes manually at builder.
    env["PROGPREFIX"] = ""
    env["PROGSUFFIX"] = ""
    env["LIBPREFIX"] = "lib"
    env["LIBSUFFIX"] = ".a"
    env["LIBPREFIXES"] = ["$LIBPREFIX"]
    env["LIBSUFFIXES"] = ["$LIBSUFFIX"]

    env.Prepend(CPPPATH=["#platforms/web"])
    env.Append(CPPDEFINES=["WEB_ENABLED", "UNIX_ENABLED"])

    if env["web_eval"]:
        env.Append(CPPDEFINES=["WEB_EVAL_ENABLED"])

    if env["threads_enabled"] and env["gdnative_enabled"]:
        print(
            "Threads and GDNative support can't be both enabled due to WebAssembly limitations"
        )
        sys.exit(255)

    # Thread support (via SharedArrayBuffer).
    if env["threads_enabled"]:
        env.Append(CPPDEFINES=["PTHREAD_NO_RENAME"])
        env.Append(CCFLAGS=["-s", "USE_PTHREADS=1"])
        env.Append(LINKFLAGS=["-s", "USE_PTHREADS=1"])
        env.Append(LINKFLAGS=["-s", "PTHREAD_POOL_SIZE=8"])
        env.Append(LINKFLAGS=["-s", "WASM_MEM_MAX=2048MB"])
        env.extra_suffix = ".threads" + env.extra_suffix
    else:
        env.Append(CPPDEFINES=["NO_THREADS"])

    if env["gdnative_enabled"]:
        env.Append(CCFLAGS=["-s", "RELOCATABLE=1"])
        env.Append(LINKFLAGS=["-s", "RELOCATABLE=1"])
        # Weak symbols are broken upstream: https://github.com/emscripten-core/emscripten/issues/12819
        env.Append(CPPDEFINES=["ZSTD_HAVE_WEAK_SYMBOLS=0"])
        env.extra_suffix = ".gdnative" + env.extra_suffix

    # Reduce code size by generating less support code (e.g. skip NodeJS support).
    env.Append(LINKFLAGS=["-s", "ENVIRONMENT=web,worker"])

    # Wrap the JavaScript support code around a closure named Rebel.
    env.Append(LINKFLAGS=["-s", "MODULARIZE=1", "-s", "EXPORT_NAME='Rebel'"])

    # Allow increasing memory buffer size during runtime. This is efficient
    # when using WebAssembly (in comparison to asm.js) and works well for
    # us since we don't know requirements at compile-time.
    env.Append(LINKFLAGS=["-s", "ALLOW_MEMORY_GROWTH=1"])

    # This setting just makes WebGL 2 APIs available, it does NOT disable WebGL 1.
    env.Append(LINKFLAGS=["-s", "USE_WEBGL2=1"])

    # Do not call main immediately when the support code is ready.
    env.Append(LINKFLAGS=["-s", "INVOKE_RUN=0"])

    # Allow use to take control of swapping WebGL buffers.
    env.Append(LINKFLAGS=["-s", "OFFSCREEN_FRAMEBUFFER=1"])

    # callMain for manual start, cwrap for the mono version.
    env.Append(LINKFLAGS=["-s", "EXPORTED_RUNTIME_METHODS=['callMain','cwrap']"])

    # Add code that allow exiting runtime.
    env.Append(LINKFLAGS=["-s", "EXIT_RUNTIME=1"])


def prepare_web(environment):
    if is_emscripten_active(environment):
        print("EMSDK environment variable found.")
        check_active_emscripten()
        configure_active_emscripten_scons(environment)
        return
    if is_emsdk_cloned(environment):
        update_emsdk(environment)
    else:
        install_emsdk(environment)
    install_emscripten(environment)
    env_string = activate_emscripten(environment)
    configure_emscripten_scons(environment, env_string)


def is_emscripten_active(environment):
    return "EMSDK" in os.environ and environment["emsdk"] == os.environ["EMSDK"]


def check_active_emscripten():
    if (
        not which("emcc")
        or not which("em++")
        or not which("emar")
        or not which("emranlib")
    ):
        print("ERROR: Required Emscripten tools not found:")
        if not which("emcc"):
            print("- emcc")
        if not which("em++"):
            print("- em++")
        if not which("emar"):
            print("- emar")
        if not which("emranlib"):
            print("- emranlib")
        print("Please check your Emscripten configuration and try again.")
        exit(1)


def install_emscripten(environment):
    version = environment["emscripten_version"]
    emsdk_app = get_emsdk_app(environment)
    if not emsdk_app:
        install_emsdk(environment)
        emsdk_app = get_emsdk_app(environment)
    print("Installing emscipten version:", version)
    subprocess.run([emsdk_app, "install", version])


def activate_emscripten(environment):
    version = environment["emscripten_version"]
    emsdk_path = environment["emsdk"]
    # We call the emsdk python code directly, because
    # we need to obtain the environment string that it generates.
    sys.path.append(emsdk_path)
    import emsdk as sdk

    print("Activating emscipten version:", version)
    sdk.main(["activate", version])
    # .emscripten file is created or updated by activate; so load it (again).
    print("Loading Emscripten config.")
    sdk.load_em_config()
    print("Getting environment construction string.")
    active_tools = sdk.currently_active_tools()
    print("Currently active tools:", active_tools)
    active_tools = sdk.process_tool_list(active_tools)
    print("Processed tool list:", active_tools)
    env_string = sdk.construct_env(active_tools, False, False)
    print("Environment construction string:")
    print(env_string)
    return env_string


def configure_emscripten_scons(environment, env_string):
    print("Applying Emscripten environment settings to SCons environment:")
    emsdk_path = os.path.abspath(environment["emsdk"])
    lines = env_string.split("\n")
    for line in lines:
        if line.startswith("SET"):
            # Drop "SET " at start of line.
            clean_line = line[4:]
            path_separator = ";"
        else:
            # Drop "export " at start of line.
            clean_line = line[7:]
            path_separator = ":"
        parts = clean_line.split("=")
        if len(parts) != 2:
            if line:
                print("Skipping line:", line)
            continue
        key = parts[0]
        value = parts[1]
        if key.startswith("EMSDK"):
            print("Adding variable:", key, "=", value)
            environment[key] = value
        if key.startswith("EMSDK_PYTHON"):
            python_path = os.path.dirname(value)
            if python_path.startswith('"') and not python_path.endswith('"'):
                python_path += '"'
            print("Adding path:", python_path)
            environment.PrependENVPath("PATH", python_path)
        if key.startswith("PATH"):
            paths = value.split(path_separator)
            for path in paths:
                path = path.strip('"')
                path = path.strip("'")
                if path.startswith("/") and get_host_platform() == "windows":
                    path = windows_path_from_bash_path(path)
                if path.startswith(emsdk_path):
                    print("Adding path:", path)
                    environment.PrependENVPath("PATH", path)


def windows_path_from_bash_path(path):
    drive = path[1].capitalize() + ":\\"
    # Drop leading "/<drive-letter>/".
    path = path[3:]
    path = path.replace("/", "\\")
    # Add drive letter.
    return drive + path


def configure_active_emscripten_scons(environment):
    print("Applying active Emscripten settings to SCons environment:")
    emsdk_path = os.path.normpath(environment["emsdk"])
    for key, value in os.environ.items():
        if key.startswith("EMSDK"):
            print("Adding variable:", key, "=", value)
            environment[key] = value
        if key == "PATH":
            paths = value.split(os.pathsep)
            for path in paths:
                if path.startswith(emsdk_path):
                    print("Adding path:", path)
                    environment.PrependENVPath("PATH", path)


def is_emsdk_cloned(environment):
    if get_emsdk_app(environment):
        return True
    return False


def get_emsdk_app(environment):
    emsdk_path = environment["emsdk"]
    emsdk_app = os.path.join(emsdk_path, "emsdk" + (".bat" if os.name == "nt" else ""))
    if not os.path.exists(emsdk_app):
        if os.path.exists(emsdk_path):
            print("ERROR: ", emsdk_app, "not found, but", emsdk_path, "exists!?")
            print("Please fix the issue and try again.")
            exit(1)
        return None
    return emsdk_app


def install_emsdk(environment):
    emsdk_path = environment["emsdk"]
    parent = os.path.dirname(emsdk_path)
    cwd = os.getcwd()
    if parent != cwd:
        os.chdir(parent)
    print("Downloading Emscripten SDK into", parent)
    subprocess.run(["git", "clone", "https://github.com/emscripten-core/emsdk.git"])
    if parent != cwd:
        os.chdir(cwd)


def update_emsdk(environment):
    emsdk_path = environment["emsdk"]
    cwd = os.getcwd()
    if emsdk_path != cwd:
        os.chdir(emsdk_path)
    print("Updating Emscripten SDK in", emsdk_path)
    subprocess.run(["git", "pull"])
    if emsdk_path != cwd:
        os.chdir(cwd)


def get_host_platform():
    if sys.platform == "linux":
        return "linux"
    elif sys.platform == "darwin":
        return "macos"
    elif sys.platform == "win32":
        return "windows"
    else:
        return None


def use_response_files(environment):
    print("Configuring response files:")
    environment["TEMPFILE"] = TempFileMunge
    environment["ARCOM"] = "${TEMPFILE('" + environment["ARCOM"] + "', '$ARCOMSTR')}"
    environment["LINKCOM"] = (
        "${TEMPFILE('" + environment["LINKCOM"] + "', '$LINKCOMSTR')}"
    )
    environment["TEMPFILEARGESCFUNC"] = fix_windows_backslash


def fix_windows_backslash(argument):
    # Apply default quote spaces function.
    from SCons.Subst import quote_spaces

    argument = quote_spaces(argument)

    if get_host_platform() != "windows":
        return argument
    argument = argument.replace("\\", "\\\\")
    return argument
