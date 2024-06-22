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
#include "platform/macos/macos_crash_handler.h"
#include "platform/macos/macos_power.h"
#else
#include "platform/linux/linux_crash_handler.h"
#include "platform/linux/linux_power.h"
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

    virtual void delete_main_loop();

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
    virtual int get_video_driver_count() const;
    virtual const char* get_video_driver_name(int p_driver) const;
    virtual int get_current_video_driver() const;
    virtual int get_audio_driver_count() const;
    virtual const char* get_audio_driver_name(int p_driver) const;

    virtual void initialize_core();
    virtual Error initialize(
        const VideoMode& p_desired,
        int p_video_driver,
        int p_audio_driver
    );
    virtual void finalize();

    virtual void set_main_loop(MainLoop* p_main_loop);

public:
    virtual String get_name() const;

    virtual void set_mouse_show(bool p_show);
    virtual void set_mouse_grab(bool p_grab);
    virtual bool is_mouse_grab_enabled() const;
    virtual Point2 get_mouse_position() const;
    virtual int get_mouse_button_state() const;
    virtual void set_window_title(const String& p_title);

    virtual MainLoop* get_main_loop() const;

    virtual bool can_draw() const;

    virtual void set_video_mode(
        const VideoMode& p_video_mode,
        int p_screen = 0
    );
    virtual VideoMode get_video_mode(int p_screen = 0) const;
    virtual void get_fullscreen_mode_list(
        List<VideoMode>* p_list,
        int p_screen = 0
    ) const;

    virtual Size2 get_window_size() const;

    virtual void move_window_to_foreground();

    void run();

    virtual OS::PowerState get_power_state();
    virtual int get_power_seconds_left();
    virtual int get_power_percent_left();
    virtual bool _check_internal_feature_support(const String& p_feature);

    virtual String get_config_path() const;
    virtual String get_data_path() const;
    virtual String get_cache_path() const;

    virtual String get_system_dir(SystemDir p_dir, bool p_shared_storage = true)
        const;

    void disable_crash_handler();
    bool is_disable_crash_handler() const;

    ServerOS();
};

#endif // SERVER_OS_H
