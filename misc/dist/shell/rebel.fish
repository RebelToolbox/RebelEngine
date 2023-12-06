# SPDX-FileCopyrightText: 2023 Rebel Engine contributors
# SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
# SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
#
# SPDX-License-Identifier: MIT

# Fish completion for the Rebel Editor
# To use it, install this file in `~/.config/fish/completions` then restart your shell.
# You can also `source` this file directly in your shell startup file.

function video_driver_args
    # Use a function instead of a fixed string to customize the argument descriptions.
    echo -e "GLES3\tOpenGL ES 2.0 renderer"
    echo -e "GLES2\tOpenGL ES 2.0 renderer"
end

# Erase existing completions for Rebel Engine.
complete -c rebel -e

# General options:
complete -c rebel -s h -l help -d "Display the full help message"
complete -c rebel -l version -d "Display the version string"
complete -c rebel -s v -l verbose -d "Use verbose stdout mode"
complete -c rebel -l quiet -d "Quiet mode, silences stdout messages (errors are still displayed)"

# Run options:
complete -c rebel -s e -l editor -d "Start the editor instead of running the scene"
complete -c rebel -s p -l project-manager -d "Start the project manager, even if a project is auto-detected"
complete -c rebel -s q -l quit -d "Quit after the first iteration"
complete -c rebel -s l -l language -d "Use a specific locale (<locale> being a two-letter code)" -x
complete -c rebel -l path -d "Path to a project (<directory> must contain a 'project.rebel' file)" -r
complete -c rebel -s u -l upwards -d "Scan folders upwards for project.rebel file"
complete -c rebel -l main-pack -d "Path to a pack (.pck) file to load" -r
complete -c rebel -l render-thread -d "Set the render thread mode" -x -a "unsafe safe separate"
complete -c rebel -l remote-fs -d "Use a remote filesystem (<host/IP>[:<port>] address)" -x
complete -c rebel -l remote-fs-password -d "Password for remote filesystem" -x
complete -c rebel -l audio-driver -d "Set the audio driver" -x
complete -c rebel -l video-driver -d "Set the video driver" -x -a "(video_driver_args)"

# Display options:
complete -c rebel -s f -l fullscreen -d "Request fullscreen mode"
complete -c rebel -s m -l maximized -d "Request a maximized window"
complete -c rebel -s w -l windowed -d "Request windowed mode"
complete -c rebel -s t -l always-on-top -d "Request an always-on-top window"
complete -c rebel -l resolution -d "Request window resolution" -x
complete -c rebel -l position -d "Request window position" -x
complete -c rebel -l low-dpi -d "Force low-DPI mode (macOS and Windows only)"
complete -c rebel -l no-window -d "Disable window creation (Windows only), useful together with --script"
complete -c rebel -l enable-vsync-via-compositor -d "When Vsync is enabled, Vsync via the OS' window compositor (Windows only)"
complete -c rebel -l disable-vsync-via-compositor -d "Disable Vsync via the OS' window compositor (Windows only)"

# Debug options:
complete -c rebel -s d -l debug -d "Debug (local stdout debugger)"
complete -c rebel -s b -l breakpoints -d "Specify the breakpoint list as source::line comma-separated pairs, no spaces (use %20 instead)" -x
complete -c rebel -l profiling -d "Enable profiling in the script debugger"
complete -c rebel -l remote-debug -d "Enable remote debugging"
complete -c rebel -l debug-collisions -d "Show collision shapes when running the scene"
complete -c rebel -l debug-navigation -d "Show navigation polygons when running the scene"
complete -c rebel -l frame-delay -d "Simulate high CPU load (delay each frame by the given number of milliseconds)" -x
complete -c rebel -l time-scale -d "Force time scale (higher values are faster, 1.0 is normal speed)" -x
complete -c rebel -l disable-render-loop -d "Disable render loop so rendering only occurs when called explicitly from script"
complete -c rebel -l disable-crash-handler -d "Disable crash handler when supported by the platform code"
complete -c rebel -l fixed-fps -d "Force a fixed number of frames per second (this setting disables real-time synchronization)" -x
complete -c rebel -l print-fps -d "Print the frames per second to the stdout"

# Standalone tools:
complete -c rebel -s s -l script -d "Run a script" -r
complete -c rebel -l check-only -d "Only parse for errors and quit (use with --script)"
complete -c rebel -l export -d "Export the project using the given preset and matching release template" -x
complete -c rebel -l export-debug -d "Same as --export, but using the debug template" -x
complete -c rebel -l export-pack -d "Same as --export, but only export the game pack for the given preset" -x
complete -c rebel -l doctool -d "Dump the engine API reference to the given path in XML format, merging if existing files are found" -r
complete -c rebel -l no-docbase -d "Disallow dumping the base types (used with --doctool)"
complete -c rebel -l build-solutions -d "Build the scripting solutions (e.g. for C# projects)"
complete -c rebel -l gdnative-generate-json-api -d "Generate JSON dump of the Rebel Engine API for GDNative bindings"
complete -c rebel -l test -d "Run a unit test" -x
