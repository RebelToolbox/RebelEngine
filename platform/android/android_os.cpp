// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "android_os.h"

#include "android_file_access.h"
#include "android_jni_dir_access.h"
#include "android_jni_io.h"
#include "android_jni_os.h"
#include "android_net_socket.h"
#include "core/os/keyboard.h"
#include "core/project_settings.h"
#include "drivers/gles2/rasterizer_gles2.h"
#include "drivers/gles3/rasterizer_gles3.h"
#include "drivers/unix/unix_dir_access.h"
#include "drivers/unix/unix_file_access.h"
#include "main/main.h"
#include "servers/visual/visual_server_raster.h"
#include "servers/visual/visual_server_wrap_mt.h"

#include <android/input.h>
#include <android/log.h>
#include <dlfcn.h>

String _remove_symlink(const String& dir) {
    // Workaround for Android 6.0+ using a symlink.
    // Save the current directory.
    char current_dir_name[2048];
    getcwd(current_dir_name, 2048);
    // Change directory to the external data directory.
    chdir(dir.utf8().get_data());
    // Get the actual directory without the potential symlink.
    char dir_name_wihout_symlink[2048];
    getcwd(dir_name_wihout_symlink, 2048);
    // Convert back to a String.
    String dir_without_symlink(dir_name_wihout_symlink);
    // Restore original current directory.
    chdir(current_dir_name);
    return dir_without_symlink;
}

class AndroidLogger : public Logger {
public:
    virtual void logv(const char* p_format, va_list p_list, bool p_err) {
        __android_log_vprint(
            p_err ? ANDROID_LOG_ERROR : ANDROID_LOG_INFO,
            "RebelEngine",
            p_format,
            p_list
        );
    }

    virtual ~AndroidLogger() {}
};

int AndroidOS::get_video_driver_count() const {
    return 2;
}

const char* AndroidOS::get_video_driver_name(int p_driver) const {
    switch (p_driver) {
        case VIDEO_DRIVER_GLES3:
            return "GLES3";
        case VIDEO_DRIVER_GLES2:
            return "GLES2";
    }
    ERR_FAIL_V_MSG(NULL, "Invalid video driver index: " + itos(p_driver) + ".");
}

int AndroidOS::get_audio_driver_count() const {
    return 1;
}

const char* AndroidOS::get_audio_driver_name(int p_driver) const {
    return "Android";
}

void AndroidOS::initialize_core() {
    UnixOS::initialize_core();

    if (use_apk_expansion) {
        FileAccess::make_default<UnixFileAccess>(FileAccess::ACCESS_RESOURCES);
    } else {
        FileAccess::make_default<AndroidFileAccess>(FileAccess::ACCESS_RESOURCES
        );
    }
    FileAccess::make_default<UnixFileAccess>(FileAccess::ACCESS_USERDATA);
    FileAccess::make_default<UnixFileAccess>(FileAccess::ACCESS_FILESYSTEM);
    if (use_apk_expansion) {
        DirAccess::make_default<UnixDirAccess>(DirAccess::ACCESS_RESOURCES);
    } else {
        DirAccess::make_default<AndroidJNIDirAccess>(DirAccess::ACCESS_RESOURCES
        );
    }
    DirAccess::make_default<UnixDirAccess>(DirAccess::ACCESS_USERDATA);
    DirAccess::make_default<UnixDirAccess>(DirAccess::ACCESS_FILESYSTEM);

    AndroidNetSocket::make_default();
}

void AndroidOS::set_opengl_extensions(const char* p_gl_extensions) {
    ERR_FAIL_COND(!p_gl_extensions);
    gl_extensions = p_gl_extensions;
}

int AndroidOS::get_current_video_driver() const {
    return video_driver_index;
}

Error AndroidOS::initialize(
    const VideoMode& p_desired,
    int p_video_driver,
    int p_audio_driver
) {
    bool use_gl3 = android_jni_os->get_gles_version_code() >= 0x00030000;
    use_gl3      = use_gl3
           && (GLOBAL_GET("rendering/quality/driver/driver_name") == "GLES3");
    bool gl_initialization_error = false;

    while (true) {
        if (use_gl3) {
            if (RasterizerGLES3::is_viable() == OK) {
                android_jni_os->gfx_init(false);
                RasterizerGLES3::register_config();
                RasterizerGLES3::make_current();
                break;
            } else {
                if (GLOBAL_GET("rendering/quality/driver/fallback_to_gles2")) {
                    p_video_driver = VIDEO_DRIVER_GLES2;
                    use_gl3        = false;
                    continue;
                } else {
                    gl_initialization_error = true;
                    break;
                }
            }
        } else {
            if (RasterizerGLES2::is_viable() == OK) {
                android_jni_os->gfx_init(true);
                RasterizerGLES2::register_config();
                RasterizerGLES2::make_current();
                break;
            } else {
                gl_initialization_error = true;
                break;
            }
        }
    }

    transparency_enabled = p_desired.layered;

    if (gl_initialization_error) {
        OS::get_singleton()->alert(
            "Your device does not support any of the supported OpenGL "
            "versions.\n"
            "Please try updating your Android version.",
            "Unable to initialize Video driver"
        );
        return ERR_UNAVAILABLE;
    }

    video_driver_index = p_video_driver;

    visual_server = memnew(VisualServerRaster);
    if (get_render_thread_mode() != RENDER_THREAD_UNSAFE) {
        visual_server = memnew(VisualServerWrapMT(visual_server, false));
    }

    visual_server->init();

    AudioDriverManager::initialize(p_audio_driver);

    input = memnew(InputDefault);
    input->set_use_input_buffering(true
    ); // Needed because events will come directly from the UI thread
    input->set_fallback_mapping(android_jni_os->get_input_fallback_mapping());

    // power_manager = memnew(PowerAndroid);

    return OK;
}

void AndroidOS::set_main_loop(MainLoop* p_main_loop) {
    main_loop = p_main_loop;
    input->set_main_loop(p_main_loop);
}

void AndroidOS::delete_main_loop() {
    memdelete(main_loop);
}

void AndroidOS::finalize() {
    memdelete(input);
}

AndroidJNIOS* AndroidOS::get_android_jni_os() {
    return android_jni_os;
}

AndroidJNIIO* AndroidOS::get_android_jni_io() {
    return android_jni_io;
}

void AndroidOS::alert(const String& p_alert, const String& p_title) {
    // print("ALERT: %s\n", p_alert.utf8().get_data());
    android_jni_os->alert(p_alert, p_title);
}

bool AndroidOS::request_permission(const String& p_name) {
    return android_jni_os->request_permission(p_name);
}

bool AndroidOS::request_permissions() {
    return android_jni_os->request_permissions();
}

Vector<String> AndroidOS::get_granted_permissions() const {
    return android_jni_os->get_granted_permissions();
}

Error AndroidOS::open_dynamic_library(
    const String p_path,
    void*& p_library_handle,
    bool p_also_set_library_path
) {
    p_library_handle = dlopen(p_path.utf8().get_data(), RTLD_NOW);
    ERR_FAIL_COND_V_MSG(
        !p_library_handle,
        ERR_CANT_OPEN,
        "Can't open dynamic library: " + p_path + ", error: " + dlerror() + "."
    );
    return OK;
}

void AndroidOS::set_mouse_show(bool p_show) {
    // android has no mouse...
}

void AndroidOS::set_mouse_grab(bool p_grab) {
    // it really has no mouse...!
}

bool AndroidOS::is_mouse_grab_enabled() const {
    //*sigh* technology has evolved so much since i was a kid..
    return false;
}

Point2 AndroidOS::get_mouse_position() const {
    return input->get_mouse_position();
}

int AndroidOS::get_mouse_button_state() const {
    return input->get_mouse_button_mask();
}

void AndroidOS::set_window_title(const String& p_title) {
    // This queries/updates the currently connected devices/joypads
    // Set_window_title is called when initializing the main loop (main.cpp)
    // therefore this place is found to be suitable (I found no better).
    android_jni_os->init_input_devices();
}

void AndroidOS::set_video_mode(const VideoMode& p_video_mode, int p_screen) {}

OS::VideoMode AndroidOS::get_video_mode(int p_screen) const {
    return default_videomode;
}

void AndroidOS::get_fullscreen_mode_list(List<VideoMode>* p_list, int p_screen)
    const {
    p_list->push_back(default_videomode);
}

void AndroidOS::set_keep_screen_on(bool p_enabled) {
    OS::set_keep_screen_on(p_enabled);

    android_jni_os->set_keep_screen_on(p_enabled);
}

Size2 AndroidOS::get_window_size() const {
    return Vector2(default_videomode.width, default_videomode.height);
}

Rect2 AndroidOS::get_window_safe_area() const {
    int xywh[4];
    android_jni_io->get_window_safe_area(xywh);
    return Rect2(xywh[0], xywh[1], xywh[2], xywh[3]);
}

String AndroidOS::get_name() const {
    return "Android";
}

MainLoop* AndroidOS::get_main_loop() const {
    return main_loop;
}

bool AndroidOS::can_draw() const {
    return true; // always?
}

void AndroidOS::main_loop_begin() {
    if (main_loop) {
        main_loop->init();
    }
}

bool AndroidOS::main_loop_iterate() {
    if (!main_loop) {
        return false;
    }
    return Main::iteration();
}

void AndroidOS::main_loop_end() {
    if (main_loop) {
        main_loop->finish();
    }
}

void AndroidOS::main_loop_focusout() {
    if (main_loop) {
        main_loop->notification(MainLoop::NOTIFICATION_WM_FOCUS_OUT);
    }
    android_audio_driver.set_pause(true);
}

void AndroidOS::main_loop_focusin() {
    if (main_loop) {
        main_loop->notification(MainLoop::NOTIFICATION_WM_FOCUS_IN);
    }
    android_audio_driver.set_pause(false);
}

void AndroidOS::process_accelerometer(const Vector3& p_accelerometer) {
    input->set_accelerometer(p_accelerometer);
}

void AndroidOS::process_gravity(const Vector3& p_gravity) {
    input->set_gravity(p_gravity);
}

void AndroidOS::process_magnetometer(const Vector3& p_magnetometer) {
    input->set_magnetometer(p_magnetometer);
}

void AndroidOS::process_gyroscope(const Vector3& p_gyroscope) {
    input->set_gyroscope(p_gyroscope);
}

bool AndroidOS::has_touchscreen_ui_hint() const {
    return true;
}

bool AndroidOS::has_virtual_keyboard() const {
    return true;
}

int AndroidOS::get_virtual_keyboard_height() const {
    return android_jni_io->get_vk_height();

    // ERR_PRINT("Cannot obtain virtual keyboard height.");
    // return 0;
}

void AndroidOS::show_virtual_keyboard(
    const String& p_existing_text,
    const Rect2& p_screen_rect,
    bool p_multiline,
    int p_max_input_length,
    int p_cursor_start,
    int p_cursor_end
) {
    if (android_jni_io->has_vk()) {
        android_jni_io->show_vk(
            p_existing_text,
            p_multiline,
            p_max_input_length,
            p_cursor_start,
            p_cursor_end
        );
    } else {
        ERR_PRINT("Virtual keyboard not available");
    };
}

void AndroidOS::hide_virtual_keyboard() {
    if (android_jni_io->has_vk()) {
        android_jni_io->hide_vk();
    } else {
        ERR_PRINT("Virtual keyboard not available");
    };
}

void AndroidOS::init_video_mode(int p_video_width, int p_video_height) {
    default_videomode.width      = p_video_width;
    default_videomode.height     = p_video_height;
    default_videomode.fullscreen = true;
    default_videomode.resizable  = false;
}

void AndroidOS::set_display_size(Size2 p_size) {
    default_videomode.width  = p_size.x;
    default_videomode.height = p_size.y;
}

Error AndroidOS::shell_open(String p_uri) {
    return android_jni_io->open_uri(p_uri);
}

String AndroidOS::get_resource_dir() const {
    return "/"; // android has its own filesystem for resources inside the APK
}

String AndroidOS::get_locale() const {
    String locale = android_jni_io->get_locale();
    if (locale != "") {
        return locale;
    }

    return UnixOS::get_locale();
}

void AndroidOS::set_clipboard(const String& p_text) {
    // DO we really need the fallback to UnixOS here?!
    if (android_jni_os->has_set_clipboard()) {
        android_jni_os->set_clipboard(p_text);
    } else {
        UnixOS::set_clipboard(p_text);
    }
}

String AndroidOS::get_clipboard() const {
    // DO we really need the fallback to UnixOS here?!
    if (android_jni_os->has_get_clipboard()) {
        return android_jni_os->get_clipboard();
    }

    return UnixOS::get_clipboard();
}

String AndroidOS::get_model_name() const {
    String model = android_jni_io->get_model();
    if (model != "") {
        return model;
    }

    return UnixOS::get_model_name();
}

int AndroidOS::get_screen_dpi(int p_screen) const {
    return android_jni_io->get_screen_dpi();
}

String AndroidOS::get_data_path() const {
    return get_user_data_dir();
}

String AndroidOS::get_user_data_dir() const {
    if (data_dir_cache != String()) {
        return data_dir_cache;
    }

    String data_dir = android_jni_io->get_user_data_dir();
    if (data_dir != "") {
        data_dir_cache = _remove_symlink(data_dir);
        return data_dir_cache;
    }
    return ".";
}

String AndroidOS::get_cache_path() const {
    String cache_dir = android_jni_io->get_cache_dir();
    if (cache_dir != "") {
        cache_dir = _remove_symlink(cache_dir);
        return cache_dir;
    }
    return ".";
}

void AndroidOS::set_screen_orientation(ScreenOrientation p_orientation) {
    android_jni_io->set_screen_orientation(p_orientation);
}

OS::ScreenOrientation AndroidOS::get_screen_orientation() const {
    const int orientation = android_jni_io->get_screen_orientation();
    ERR_FAIL_INDEX_V_MSG(
        orientation,
        7,
        OS::ScreenOrientation(0),
        "Unrecognized screen orientation."
    );
    return OS::ScreenOrientation(orientation);
}

String AndroidOS::get_unique_id() const {
    String unique_id = android_jni_io->get_unique_id();
    if (unique_id != "") {
        return unique_id;
    }

    return OS::get_unique_id();
}

String AndroidOS::get_system_dir(SystemDir p_dir, bool p_shared_storage) const {
    return android_jni_io->get_system_dir(p_dir, p_shared_storage);
}

void AndroidOS::set_context_is_16_bits(bool p_is_16) {
    // use_16bits_fbo = p_is_16;
    // if (rasterizer)
    //	rasterizer->set_force_16_bits_fbo(p_is_16);
}

bool AndroidOS::is_joy_known(int p_device) {
    return input->is_joy_mapped(p_device);
}

String AndroidOS::get_joy_guid(int p_device) const {
    return input->get_joy_guid_remapped(p_device);
}

void AndroidOS::vibrate_handheld(int p_duration_ms) {
    android_jni_os->vibrate(p_duration_ms);
}

bool AndroidOS::_check_internal_feature_support(const String& p_feature) {
    if (p_feature == "mobile") {
        // TODO support etc2 only if GLES3 driver is selected
        return true;
    }
#if defined(__aarch64__)
    if (p_feature == "arm64-v8a") {
        return true;
    }
#elif defined(__ARM_ARCH_7A__)
    if (p_feature == "armeabi-v7a" || p_feature == "armeabi") {
        return true;
    }
#elif defined(__arm__)
    if (p_feature == "armeabi") {
        return true;
    }
#endif
    return false;
}

AndroidOS::AndroidOS(
    AndroidJNIOS* p_android_jni_os,
    AndroidJNIIO* p_android_jni_io,
    bool p_use_apk_expansion
) {
    use_apk_expansion            = p_use_apk_expansion;
    default_videomode.width      = 800;
    default_videomode.height     = 600;
    default_videomode.fullscreen = true;
    default_videomode.resizable  = false;

    main_loop     = NULL;
    gl_extensions = NULL;
    // rasterizer = NULL;
    use_gl2       = false;

    android_jni_os = p_android_jni_os;
    android_jni_io = p_android_jni_io;

    Vector<Logger*> loggers;
    loggers.push_back(memnew(AndroidLogger));
    _set_logger(memnew(CompositeLogger(loggers)));

    AudioDriverManager::add_driver(&android_audio_driver);
}

AndroidOS::~AndroidOS() {}
