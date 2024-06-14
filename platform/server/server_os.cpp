// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "server_os.h"

#include "core/print_string.h"
#include "drivers/dummy/rasterizer_dummy.h"
#include "drivers/dummy/texture_loader_dummy.h"
#include "main/main.h"
#include "servers/visual/visual_server_raster.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ServerOS::get_video_driver_count() const {
    return 1;
}

const char* ServerOS::get_video_driver_name(int p_driver) const {
    return "Dummy";
}

int ServerOS::get_audio_driver_count() const {
    return 1;
}

const char* ServerOS::get_audio_driver_name(int p_driver) const {
    return "Dummy";
}

int ServerOS::get_current_video_driver() const {
    return video_driver_index;
}

void ServerOS::initialize_core() {
    crash_handler.initialize();

    OS_Unix::initialize_core();
}

Error ServerOS::initialize(
    const VideoMode& p_desired,
    int p_video_driver,
    int p_audio_driver
) {
    args              = OS::get_singleton()->get_cmdline_args();
    current_videomode = p_desired;
    main_loop         = NULL;

    RasterizerDummy::make_current();

    video_driver_index = p_video_driver; // unused in server platform, but
                                         // should still be initialized

    visual_server = memnew(VisualServerRaster);
    visual_server->init();

    AudioDriverManager::initialize(p_audio_driver);

    input = memnew(InputDefault);

#ifdef __APPLE__
    power_manager = memnew(MacOSPower);
#else
    power_manager = memnew(LinuxPower);
#endif

    resource_loader_dummy.instance();
    ResourceLoader::add_resource_format_loader(resource_loader_dummy);

    return OK;
}

void ServerOS::finalize() {
    if (main_loop) {
        memdelete(main_loop);
    }
    main_loop = NULL;

    visual_server->finish();
    memdelete(visual_server);

    memdelete(input);

    memdelete(power_manager);

    ResourceLoader::remove_resource_format_loader(resource_loader_dummy);
    resource_loader_dummy.unref();

    args.clear();
}

void ServerOS::set_mouse_show(bool p_show) {}

void ServerOS::set_mouse_grab(bool p_grab) {
    grab = p_grab;
}

bool ServerOS::is_mouse_grab_enabled() const {
    return grab;
}

int ServerOS::get_mouse_button_state() const {
    return 0;
}

Point2 ServerOS::get_mouse_position() const {
    return Point2();
}

void ServerOS::set_window_title(const String& p_title) {}

void ServerOS::set_video_mode(const VideoMode& p_video_mode, int p_screen) {}

OS::VideoMode ServerOS::get_video_mode(int p_screen) const {
    return current_videomode;
}

Size2 ServerOS::get_window_size() const {
    return Vector2(current_videomode.width, current_videomode.height);
}

void ServerOS::get_fullscreen_mode_list(List<VideoMode>* p_list, int p_screen)
    const {}

MainLoop* ServerOS::get_main_loop() const {
    return main_loop;
}

void ServerOS::delete_main_loop() {
    if (main_loop) {
        memdelete(main_loop);
    }
    main_loop = NULL;
}

void ServerOS::set_main_loop(MainLoop* p_main_loop) {
    main_loop = p_main_loop;
    input->set_main_loop(p_main_loop);
}

bool ServerOS::can_draw() const {
    return false; // can never draw
};

String ServerOS::get_name() const {
    return "Server";
}

void ServerOS::move_window_to_foreground() {}

OS::PowerState ServerOS::get_power_state() {
    return power_manager->get_power_state();
}

int ServerOS::get_power_seconds_left() {
    return power_manager->get_power_seconds_left();
}

int ServerOS::get_power_percent_left() {
    return power_manager->get_power_percent_left();
}

bool ServerOS::_check_internal_feature_support(const String& p_feature) {
    return p_feature == "pc";
}

void ServerOS::run() {
    force_quit = false;

    if (!main_loop) {
        return;
    }

    main_loop->init();

    while (!force_quit) {
        if (Main::iteration()) {
            break;
        }
    };

    main_loop->finish();
}

String ServerOS::get_config_path() const {
    if (has_environment("XDG_CONFIG_HOME")) {
        return get_environment("XDG_CONFIG_HOME");
    } else if (has_environment("HOME")) {
        return get_environment("HOME").plus_file(".config");
    } else {
        return ".";
    }
}

String ServerOS::get_data_path() const {
    if (has_environment("XDG_DATA_HOME")) {
        return get_environment("XDG_DATA_HOME");
    } else if (has_environment("HOME")) {
        return get_environment("HOME").plus_file(".local/share");
    } else {
        return get_config_path();
    }
}

String ServerOS::get_cache_path() const {
    if (has_environment("XDG_CACHE_HOME")) {
        return get_environment("XDG_CACHE_HOME");
    } else if (has_environment("HOME")) {
        return get_environment("HOME").plus_file(".cache");
    } else {
        return get_config_path();
    }
}

String ServerOS::get_system_dir(SystemDir p_dir, bool p_shared_storage) const {
    String xdgparam;

    switch (p_dir) {
        case SYSTEM_DIR_DESKTOP: {
            xdgparam = "DESKTOP";
        } break;
        case SYSTEM_DIR_DCIM: {
            xdgparam = "PICTURES";

        } break;
        case SYSTEM_DIR_DOCUMENTS: {
            xdgparam = "DOCUMENTS";

        } break;
        case SYSTEM_DIR_DOWNLOADS: {
            xdgparam = "DOWNLOAD";

        } break;
        case SYSTEM_DIR_MOVIES: {
            xdgparam = "VIDEOS";

        } break;
        case SYSTEM_DIR_MUSIC: {
            xdgparam = "MUSIC";

        } break;
        case SYSTEM_DIR_PICTURES: {
            xdgparam = "PICTURES";

        } break;
        case SYSTEM_DIR_RINGTONES: {
            xdgparam = "MUSIC";

        } break;
    }

    String pipe;
    List<String> arg;
    arg.push_back(xdgparam);
    Error err = const_cast<ServerOS*>(this)
                    ->execute("xdg-user-dir", arg, true, NULL, &pipe);
    if (err != OK) {
        return ".";
    }
    return pipe.strip_edges();
}

void ServerOS::disable_crash_handler() {
    crash_handler.disable();
}

bool ServerOS::is_disable_crash_handler() const {
    return crash_handler.is_disabled();
}

ServerOS::ServerOS() {
    // adriver here
    grab = false;
};
