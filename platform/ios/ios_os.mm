// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef IOS_ENABLED

#include "ios_os.h"

#import "app_delegate.h"
#include "core/io/file_access_pack.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/project_settings.h"
#import "device_metrics.h"
#include "drivers/gles2/rasterizer_gles2.h"
#include "drivers/gles3/rasterizer_gles3.h"
#include "drivers/unix/unix_syslog_logger.h"
#import "keyboard_input_view.h"
#include "main/main.h"
#import "native_video_view.h"
#import "rebel_view.h"
#include "servers/visual/visual_server_raster.h"
#include "servers/visual/visual_server_wrap_mt.h"
#import "view_controller.h"

#import <UIKit/UIKit.h>
#include <dlfcn.h>
#import <sys/utsname.h>

extern int gl_view_base_fb;  // from gl_view.mm
extern bool gles3_available; // from gl_view.mm

// Initialization order between compilation units is not guaranteed,
// so we use this as a hack to ensure certain code is called before
// everything else, but after all units are initialized.
typedef void (*init_callback)();
static init_callback* ios_init_callbacks = NULL;
static int ios_init_callbacks_count      = 0;
static int ios_init_callbacks_capacity   = 0;
HashMap<String, void*> IosOS::dynamic_symbol_lookup_table;

int IosOS::get_video_driver_count() const {
    return 2;
};

const char* IosOS::get_video_driver_name(int p_driver) const {
    switch (p_driver) {
        case VIDEO_DRIVER_GLES3:
            return "GLES3";
        case VIDEO_DRIVER_GLES2:
            return "GLES2";
    }
    ERR_FAIL_V_MSG(NULL, "Invalid video driver index: " + itos(p_driver) + ".");
};

IosOS* IosOS::get_singleton() {
    return (IosOS*)OS::get_singleton();
};

void IosOS::set_data_dir(String p_dir) {
    DirAccess* da = DirAccess::open(p_dir);

    data_dir = da->get_current_dir();
    printf(
        "setting data dir to %ls from %ls\n",
        data_dir.c_str(),
        p_dir.c_str()
    );
    memdelete(da);
};

String IosOS::get_unique_id() const {
    NSString* uuid = [UIDevice currentDevice].identifierForVendor.UUIDString;
    return String::utf8([uuid UTF8String]);
};

void IosOS::initialize_core() {
    UnixOS::initialize_core();

    set_data_dir(data_dir);
};

int IosOS::get_current_video_driver() const {
    return video_driver_index;
}

void IosOS::start() {
    Main::start();

    if (ios_joypad) {
        ios_joypad->start_processing();
    }
}

Error IosOS::initialize(
    const VideoMode& p_desired,
    int p_video_driver,
    int p_audio_driver
) {
    bool use_gl3 =
        GLOBAL_GET("rendering/quality/driver/driver_name") == "GLES3";
    bool gl_initialization_error = false;

    while (true) {
        if (use_gl3) {
            if (RasterizerGLES3::is_viable() == OK && gles3_available) {
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
                RasterizerGLES2::register_config();
                RasterizerGLES2::make_current();
                break;
            } else {
                gl_initialization_error = true;
                break;
            }
        }
    }

    if (gl_initialization_error) {
        OS::get_singleton()->alert(
            "Your device does not support any of the supported OpenGL "
            "versions.",
            "Unable to initialize Video driver"
        );
        return ERR_UNAVAILABLE;
    }

    video_driver_index = p_video_driver;
    visual_server      = memnew(VisualServerRaster);
    // FIXME: Reimplement threaded rendering
    if (get_render_thread_mode() != RENDER_THREAD_UNSAFE) {
        visual_server = memnew(VisualServerWrapMT(visual_server, false));
    }

    visual_server->init();
    // visual_server->cursor_set_visible(false, 0);

    // reset this to what it should be, it will have been set to 0 after
    // visual_server->init() is called
    if (use_gl3) {
        RasterizerStorageGLES3::system_fbo = gl_view_base_fb;
    } else {
        RasterizerStorageGLES2::system_fbo = gl_view_base_fb;
    }

    AudioDriverManager::initialize(p_audio_driver);

    input = memnew(InputDefault);

    ios = memnew(iOS);
    Engine::get_singleton()->add_singleton(Engine::Singleton("iOS", ios));

    ios_joypad = memnew(IosJoypad);

    return OK;
};

MainLoop* IosOS::get_main_loop() const {
    return main_loop;
};

void IosOS::set_main_loop(MainLoop* p_main_loop) {
    main_loop = p_main_loop;

    if (main_loop) {
        input->set_main_loop(p_main_loop);
        main_loop->init();
    }
};

bool IosOS::iterate() {
    if (!main_loop) {
        return true;
    }

    input->flush_buffered_events();

    return Main::iteration();
};

void IosOS::key(uint32_t p_key, bool p_pressed) {
    Ref<InputEventKey> ev;
    ev.instance();
    ev->set_echo(false);
    ev->set_pressed(p_pressed);
    ev->set_scancode(p_key);
    ev->set_physical_scancode(p_key);
    ev->set_unicode(p_key);
    perform_event(ev);
};

void IosOS::pencil_press(
    int p_idx,
    int p_x,
    int p_y,
    bool p_pressed,
    bool p_doubleclick
) {
    Ref<InputEventMouseButton> ev;
    ev.instance();
    ev->set_button_index(1);
    ev->set_pressed(p_pressed);
    ev->set_position(Vector2(p_x, p_y));
    ev->set_global_position(Vector2(p_x, p_y));
    ev->set_doubleclick(p_doubleclick);
    perform_event(ev);
};

void IosOS::pencil_drag(
    int p_idx,
    int p_prev_x,
    int p_prev_y,
    int p_x,
    int p_y,
    float p_force
) {
    Ref<InputEventMouseMotion> ev;
    ev.instance();
    ev->set_pressure(p_force);
    ev->set_position(Vector2(p_x, p_y));
    ev->set_global_position(Vector2(p_x, p_y));
    ev->set_relative(Vector2(p_x - p_prev_x, p_y - p_prev_y));
    perform_event(ev);
};

void IosOS::pencil_cancelled(int p_idx) {
    pencil_press(p_idx, -1, -1, false, false);
}

void IosOS::touch_press(
    int p_idx,
    int p_x,
    int p_y,
    bool p_pressed,
    bool p_doubleclick
) {
    if (GLOBAL_DEF("debug/disable_touch", false)) {
        return;
    }

    Ref<InputEventScreenTouch> ev;
    ev.instance();

    ev->set_index(p_idx);
    ev->set_pressed(p_pressed);
    ev->set_position(Vector2(p_x, p_y));
    perform_event(ev);
};

void IosOS::touch_drag(
    int p_idx,
    int p_prev_x,
    int p_prev_y,
    int p_x,
    int p_y
) {
    if (GLOBAL_DEF("debug/disable_touch", false)) {
        return;
    }

    Ref<InputEventScreenDrag> ev;
    ev.instance();
    ev->set_index(p_idx);
    ev->set_position(Vector2(p_x, p_y));
    ev->set_relative(Vector2(p_x - p_prev_x, p_y - p_prev_y));
    perform_event(ev);
}

void IosOS::perform_event(const Ref<InputEvent>& p_event) {
    input->parse_input_event(p_event);
}

void IosOS::touches_cancelled(int p_idx) {
    touch_press(p_idx, -1, -1, false, false);
}

static const float ACCEL_RANGE = 1;

void IosOS::update_gravity(float p_x, float p_y, float p_z) {
    input->set_gravity(Vector3(p_x, p_y, p_z));
};

void IosOS::update_accelerometer(float p_x, float p_y, float p_z) {
    // Found out the Z should not be negated! Pass as is!
    input->set_accelerometer(Vector3(
        p_x / (float)ACCEL_RANGE,
        p_y / (float)ACCEL_RANGE,
        p_z / (float)ACCEL_RANGE
    ));
};

void IosOS::update_magnetometer(float p_x, float p_y, float p_z) {
    input->set_magnetometer(Vector3(p_x, p_y, p_z));
};

void IosOS::update_gyroscope(float p_x, float p_y, float p_z) {
    input->set_gyroscope(Vector3(p_x, p_y, p_z));
};

int IosOS::get_unused_joy_id() {
    return input->get_unused_joy_id();
};

void IosOS::joy_connection_changed(int p_idx, bool p_connected, String p_name) {
    input->joy_connection_changed(p_idx, p_connected, p_name);
};

void IosOS::joy_button(int p_device, int p_button, bool p_pressed) {
    input->joy_button(p_device, p_button, p_pressed);
};

void IosOS::joy_axis(
    int p_device,
    int p_axis,
    const InputDefault::JoyAxis& p_value
) {
    input->joy_axis(p_device, p_axis, p_value);
};

void IosOS::delete_main_loop() {
    if (main_loop) {
        main_loop->finish();
        memdelete(main_loop);
    };

    main_loop = NULL;
};

void IosOS::finalize() {
    delete_main_loop();

    if (ios_joypad) {
        memdelete(ios_joypad);
    }

    if (input) {
        memdelete(input);
    }

    if (ios) {
        memdelete(ios);
    }

    visual_server->finish();
    memdelete(visual_server);
    //	memdelete(rasterizer);
}

void IosOS::set_mouse_show(bool p_show) {
    // Not supported for iOS
}

void IosOS::set_mouse_grab(bool p_grab) {
    // Not supported for iOS
}

bool IosOS::is_mouse_grab_enabled() const {
    // Not supported for iOS
    return true;
}

Point2 IosOS::get_mouse_position() const {
    // Not supported for iOS
    return Point2();
}

int IosOS::get_mouse_button_state() const {
    // Not supported for iOS
    return 0;
}

void IosOS::set_window_title(const String& p_title) {
    // Not supported for iOS
}

void IosOS::alert(const String& p_alert, const String& p_title) {
    const CharString utf8_alert = p_alert.utf8();
    const CharString utf8_title = p_title.utf8();
    iOS::alert(utf8_alert.get_data(), utf8_title.get_data());
}

// MARK: Dynamic Libraries

Error IosOS::open_dynamic_library(
    const String p_path,
    void*& p_library_handle,
    bool p_also_set_library_path
) {
    if (p_path.length() == 0) {
        p_library_handle = RTLD_SELF;
        return OK;
    }
    return UnixOS::open_dynamic_library(
        p_path,
        p_library_handle,
        p_also_set_library_path
    );
}

Error IosOS::close_dynamic_library(void* p_library_handle) {
    if (p_library_handle == RTLD_SELF) {
        return OK;
    }
    return UnixOS::close_dynamic_library(p_library_handle);
}

void register_dynamic_symbol(char* name, void* address) {
    IosOS::dynamic_symbol_lookup_table[String(name)] = address;
}

Error IosOS::get_dynamic_library_symbol_handle(
    void* p_library_handle,
    const String p_name,
    void*& p_symbol_handle,
    bool p_optional
) {
    if (p_library_handle == RTLD_SELF) {
        void** ptr = IosOS::dynamic_symbol_lookup_table.getptr(p_name);
        if (ptr) {
            p_symbol_handle = *ptr;
            return OK;
        }
    }
    return UnixOS::get_dynamic_library_symbol_handle(
        p_library_handle,
        p_name,
        p_symbol_handle,
        p_optional
    );
}

void IosOS::set_video_mode(const VideoMode& p_video_mode, int p_screen) {
    video_mode = p_video_mode;
}

OS::VideoMode IosOS::get_video_mode(int p_screen) const {
    return video_mode;
}

void IosOS::get_fullscreen_mode_list(List<VideoMode>* p_list, int p_screen)
    const {
    p_list->push_back(video_mode);
}

bool IosOS::can_draw() const {
    if (native_video_is_playing()) {
        return false;
    }
    return true;
}

int IosOS::set_base_framebuffer(int p_fb) {
    // gl_view_base_fb has not been updated yet
    RasterizerStorageGLES3::system_fbo = p_fb;

    return 0;
}

bool IosOS::has_virtual_keyboard() const {
    return true;
};

void IosOS::show_virtual_keyboard(
    const String& p_existing_text,
    const Rect2& p_screen_rect,
    bool p_multiline,
    int p_max_input_length,
    int p_cursor_start,
    int p_cursor_end
) {
    NSString* existingString =
        [[NSString alloc] initWithUTF8String:p_existing_text.utf8().get_data()];

    [AppDelegate.viewController.keyboardView
        becomeFirstResponderWithString:existingString
                             multiline:p_multiline
                           cursorStart:p_cursor_start
                             cursorEnd:p_cursor_end];
};

void IosOS::hide_virtual_keyboard() {
    [AppDelegate.viewController.keyboardView resignFirstResponder];
}

void IosOS::set_virtual_keyboard_height(int p_height) {
    virtual_keyboard_height = p_height * [UIScreen mainScreen].nativeScale;
}

int IosOS::get_virtual_keyboard_height() const {
    return virtual_keyboard_height;
}

Error IosOS::shell_open(String p_uri) {
    NSString* urlPath =
        [[NSString alloc] initWithUTF8String:p_uri.utf8().get_data()];
    NSURL* url = [NSURL URLWithString:urlPath];

    if (![[UIApplication sharedApplication] canOpenURL:url]) {
        return ERR_CANT_OPEN;
    }

    printf("opening url %s\n", p_uri.utf8().get_data());

    [[UIApplication sharedApplication] openURL:url
                                       options:@{}
                             completionHandler:nil];

    return OK;
}

void IosOS::set_keep_screen_on(bool p_enabled) {
    OS::set_keep_screen_on(p_enabled);
    [UIApplication sharedApplication].idleTimerDisabled = p_enabled;
};

String IosOS::get_user_data_dir() const {
    return data_dir;
}

String IosOS::get_name() const {
    return "iOS";
}

void IosOS::set_clipboard(const String& p_text) {
    [UIPasteboard generalPasteboard].string =
        [NSString stringWithUTF8String:p_text.utf8()];
}

String IosOS::get_clipboard() const {
    NSString* text = [UIPasteboard generalPasteboard].string;

    return String::utf8([text UTF8String]);
}

String IosOS::get_model_name() const {
    String model = ios->get_model();
    if (model != "") {
        return model;
    }

    return UnixOS::get_model_name();
}

Size2 IosOS::get_window_size() const {
    return Vector2(video_mode.width, video_mode.height);
}

int IosOS::get_screen_dpi(int p_screen) const {
    struct utsname systemInfo;
    uname(&systemInfo);

    NSString* string = [NSString stringWithCString:systemInfo.machine
                                          encoding:NSUTF8StringEncoding];

    NSDictionary* iOSModelToDPI = [RebelDeviceMetrics dpiList];

    for (NSArray* keyArray in iOSModelToDPI) {
        if ([keyArray containsObject:string]) {
            NSNumber* value = iOSModelToDPI[keyArray];
            return [value intValue];
        }
    }

    // If device wasn't found in dictionary
    // make a best guess from device metrics.
    CGFloat scale = [UIScreen mainScreen].scale;

    UIUserInterfaceIdiom idiom = [UIDevice currentDevice].userInterfaceIdiom;

    switch (idiom) {
        case UIUserInterfaceIdiomPad:
            return scale == 2 ? 264 : 132;
        case UIUserInterfaceIdiomPhone: {
            if (scale == 3) {
                CGFloat nativeScale = [UIScreen mainScreen].nativeScale;
                return nativeScale == 3 ? 458 : 401;
            }

            return 326;
        }
        default:
            return 72;
    }
}

Rect2 IosOS::get_window_safe_area() const {
    if (@available(iOS 11, *)) {
        UIEdgeInsets insets = UIEdgeInsetsZero;
        UIView* view        = AppDelegate.viewController.rebelView;

        if ([view respondsToSelector:@selector(safeAreaInsets)]) {
            insets = [view safeAreaInsets];
        }

        float scale            = [UIScreen mainScreen].nativeScale;
        Size2i insets_position = Size2i(insets.left, insets.top) * scale;
        Size2i insets_size =
            Size2i(insets.left + insets.right, insets.top + insets.bottom)
            * scale;

        return Rect2i(insets_position, get_window_size() - insets_size);
    } else {
        return Rect2i(Size2i(0, 0), get_window_size());
    }
}

bool IosOS::has_touchscreen_ui_hint() const {
    return true;
}

String IosOS::get_locale() const {
    NSString* preferedLanguage = [NSLocale preferredLanguages].firstObject;

    if (preferedLanguage) {
        return String::utf8([preferedLanguage UTF8String]).replace("-", "_");
    }

    NSString* localeIdentifier = [[NSLocale currentLocale] localeIdentifier];
    return String::utf8([localeIdentifier UTF8String]).replace("-", "_");
}

Error IosOS::native_video_play(
    String p_path,
    float p_volume,
    String p_audio_track,
    String p_subtitle_track
) {
    FileAccess* f = FileAccess::open(p_path, FileAccess::READ);
    bool exists   = f && f->is_open();

    String user_data_dir = IosOS::get_singleton()->get_user_data_dir();

    if (!exists) {
        return FAILED;
    }

    String tempFile = IosOS::get_singleton()->get_user_data_dir();

    if (p_path.begins_with("res://")) {
        if (PackedData::get_singleton()->has_path(p_path)) {
            printf(
                "Unable to play %s using the native player as it resides in a "
                ".pck file\n",
                p_path.utf8().get_data()
            );
            return ERR_INVALID_PARAMETER;
        } else {
            p_path = p_path.replace(
                "res:/",
                ProjectSettings::get_singleton()->get_resource_path()
            );
        }
    } else if (p_path.begins_with("user://")) {
        p_path = p_path.replace("user:/", user_data_dir);
    }

    memdelete(f);

    printf("Playing video: %s\n", p_path.utf8().get_data());

    String file_path = ProjectSettings::get_singleton()->globalize_path(p_path);

    NSString* filePath =
        [[NSString alloc] initWithUTF8String:file_path.utf8().get_data()];
    NSString* audioTrack = [NSString stringWithUTF8String:p_audio_track.utf8()];
    NSString* subtitleTrack =
        [NSString stringWithUTF8String:p_subtitle_track.utf8()];

    if (![AppDelegate.viewController playVideoAtPath:filePath
                                              volume:p_volume
                                               audio:audioTrack
                                            subtitle:subtitleTrack]) {
        return OK;
    }

    return FAILED;
}

bool IosOS::native_video_is_playing() const {
    return [AppDelegate.viewController.videoView isVideoPlaying];
}

void IosOS::native_video_pause() {
    if (native_video_is_playing()) {
        [AppDelegate.viewController.videoView pauseVideo];
    }
}

void IosOS::native_video_unpause() {
    [AppDelegate.viewController.videoView unpauseVideo];
}

void IosOS::native_video_focus_out() {
    [AppDelegate.viewController.videoView unfocusVideo];
}

void IosOS::native_video_stop() {
    if (native_video_is_playing()) {
        [AppDelegate.viewController.videoView stopVideo];
    }
}

void IosOS::vibrate_handheld(int p_duration_ms) {
    // iOS does not support duration for vibration
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

bool IosOS::_check_internal_feature_support(const String& p_feature) {
    return p_feature == "mobile";
}

void add_ios_init_callback(init_callback cb) {
    if (ios_init_callbacks_count == ios_init_callbacks_capacity) {
        void* new_ptr = realloc(ios_init_callbacks, sizeof(cb) * 32);
        if (new_ptr) {
            ios_init_callbacks           = (init_callback*)(new_ptr);
            ios_init_callbacks_capacity += 32;
        }
    }
    if (ios_init_callbacks_capacity > ios_init_callbacks_count) {
        ios_init_callbacks[ios_init_callbacks_count] = cb;
        ++ios_init_callbacks_count;
    }
}

IosOS::IosOS(String p_data_dir) {
    for (int i = 0; i < ios_init_callbacks_count; ++i) {
        ios_init_callbacks[i]();
    }
    free(ios_init_callbacks);
    ios_init_callbacks          = NULL;
    ios_init_callbacks_count    = 0;
    ios_init_callbacks_capacity = 0;

    main_loop     = NULL;
    visual_server = NULL;

    // can't call set_data_dir from here, since it requires DirAccess
    // which is initialized in initialize_core
    data_dir = p_data_dir;

    Vector<Logger*> loggers;
    loggers.push_back(memnew(UnixSyslogLogger));
#ifdef DEBUG_ENABLED
    // it seems iOS app's stdout/stderr is only obtainable if you launch it from
    // Xcode
    loggers.push_back(memnew(StdLogger));
#endif
    _set_logger(memnew(CompositeLogger(loggers)));

    AudioDriverManager::add_driver(&audio_driver);
};

IosOS::~IosOS() {}

void IosOS::on_focus_out() {
    if (is_focused) {
        is_focused = false;

        if (get_main_loop()) {
            get_main_loop()->notification(MainLoop::NOTIFICATION_WM_FOCUS_OUT);
        }

        [AppDelegate.viewController.rebelView stopRendering];

        if (native_video_is_playing()) {
            native_video_focus_out();
        }

        audio_driver.stop();
    }
}

void IosOS::on_focus_in() {
    if (!is_focused) {
        is_focused = true;

        if (get_main_loop()) {
            get_main_loop()->notification(MainLoop::NOTIFICATION_WM_FOCUS_IN);
        }

        [AppDelegate.viewController.rebelView startRendering];

        if (native_video_is_playing()) {
            native_video_unpause();
        }

        audio_driver.start();
    }
}

#endif
