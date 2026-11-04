// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef SERVER_OS_H
#define SERVER_OS_H

#include "drivers/dummy/texture_loader_dummy.h"
#include "drivers/unix/unix_os.h"
#include "main/input_default.h"
#ifdef __APPLE__
#include "platforms/macos/macos_crash_handler.h"
#include "platforms/macos/macos_power.h"
#else
#include "platforms/linux/linux_crash_handler.h"
#include "platforms/linux/linux_power.h"
#endif
#include "servers/audio_server.h"
#include "servers/visual/rasterizer.h"
#include "servers/visual_server.h"

#undef CursorShape

class ServerOS : public UnixOS {
    VisualServer* visual_server;
    VideoMode current_videomode;
    List<String> args;
    MainLoop* main_loop;

    bool grab;

    void delete_main_loop() override;

    bool force_quit;

    InputDefault* input;

#ifdef __APPLE__
    MacOSPower* power_manager;
#else
    LinuxPower* power_manager;
#endif

    CrashHandler crash_handler;

    int video_driver_index;

    Ref<ResourceFormatDummyTexture> resource_loader_dummy;

protected:
    int get_video_driver_count() const override;
    const char* get_video_driver_name(int p_driver) const override;
    int get_current_video_driver() const override;
    int get_audio_driver_count() const override;
    const char* get_audio_driver_name(int p_driver) const override;

    void initialize_core() override;
    Error initialize(
        const VideoMode& p_desired,
        int p_video_driver,
        int p_audio_driver
    ) override;
    void finalize() override;

    void set_main_loop(MainLoop* p_main_loop) override;

public:
    String get_name() const override;

    void set_mouse_show(bool p_show);
    void set_mouse_grab(bool p_grab);
    bool is_mouse_grab_enabled() const;
    Point2 get_mouse_position() const override;
    int get_mouse_button_state() const override;
    void set_window_title(const String& p_title) override;

    MainLoop* get_main_loop() const override;

    bool can_draw() const override;

    void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    ) override;
    VideoMode get_video_mode(int p_screen = 0) const override;
    void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const override;

    Size2 get_window_size() const override;

    void move_window_to_foreground() override;

    void run();

    OS::PowerState get_power_state() override;
    int get_power_seconds_left() override;
    int get_power_percent_left() override;
    bool _check_internal_feature_support(const String& p_feature) override;

    String get_config_path() const override;
    String get_data_path() const override;
    String get_cache_path() const override;

    String get_system_dir(
        SystemDir p_dir,
        bool p_shared_storage = true
    ) const override;

    void disable_crash_handler() override;
    bool is_disable_crash_handler() const override;

    ServerOS();
};

#endif // SERVER_OS_H
