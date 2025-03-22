// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "uwp_os.h"

#include "core/io/marshalls.h"
#include "core/project_settings.h"
#include "drivers/gles2/rasterizer_gles2.h"
#include "drivers/gles3/rasterizer_gles3.h"
#include "drivers/network/default_ip.h"
#include "drivers/network/default_net_socket.h"
#include "main/main.h"
#include "platforms/windows/windows_dir_access.h"
#include "platforms/windows/windows_file_access.h"
#include "platforms/windows/windows_terminal_logger.h"
#include "servers/audio_server.h"
#include "servers/visual/visual_server_raster.h"
#include "servers/visual/visual_server_wrap_mt.h"

#include <ppltasks.h>
#include <wrl.h>

using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Popups;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Microsoft::WRL;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Devices::Input;
using namespace Windows::Devices::Sensors;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace concurrency;

int UwpOS::get_video_driver_count() const {
    return 2;
}

Size2 UwpOS::get_window_size() const {
    Size2 size;
    size.width  = video_mode.width;
    size.height = video_mode.height;
    return size;
}

int UwpOS::get_current_video_driver() const {
    return video_driver_index;
}

void UwpOS::set_window_size(const Size2 p_size) {
    Windows::Foundation::Size new_size;
    new_size.Width  = p_size.width;
    new_size.Height = p_size.height;

    ApplicationView ^ view = ApplicationView::GetForCurrentView();

    if (view->TryResizeView(new_size)) {
        video_mode.width  = p_size.width;
        video_mode.height = p_size.height;
    }
}

void UwpOS::set_window_fullscreen(bool p_enabled) {
    ApplicationView ^ view = ApplicationView::GetForCurrentView();

    video_mode.fullscreen = view->IsFullScreenMode;

    if (video_mode.fullscreen == p_enabled) {
        return;
    }

    if (p_enabled) {
        video_mode.fullscreen = view->TryEnterFullScreenMode();

    } else {
        view->ExitFullScreenMode();
        video_mode.fullscreen = false;
    }
}

bool UwpOS::is_window_fullscreen() const {
    return ApplicationView::GetForCurrentView()->IsFullScreenMode;
}

void UwpOS::set_keep_screen_on(bool p_enabled) {
    if (is_keep_screen_on() == p_enabled) {
        return;
    }

    if (p_enabled) {
        display_request->RequestActive();
    } else {
        display_request->RequestRelease();
    }

    OS::set_keep_screen_on(p_enabled);
}

void UwpOS::initialize_core() {
    last_button_state = 0;

    // RedirectIOToConsole();

    FileAccess::make_default<FileAccessWindows>(FileAccess::ACCESS_RESOURCES);
    FileAccess::make_default<FileAccessWindows>(FileAccess::ACCESS_USERDATA);
    FileAccess::make_default<FileAccessWindows>(FileAccess::ACCESS_FILESYSTEM);
    DirAccess::make_default<DirAccessWindows>(DirAccess::ACCESS_RESOURCES);
    DirAccess::make_default<DirAccessWindows>(DirAccess::ACCESS_USERDATA);
    DirAccess::make_default<DirAccessWindows>(DirAccess::ACCESS_FILESYSTEM);

    DefaultNetSocket::make_default();

    // We need to know how often the clock is updated
    if (!QueryPerformanceFrequency((LARGE_INTEGER*)&ticks_per_second)) {
        ticks_per_second = 1000;
    }
    // If timeAtGameStart is 0 then we get the time since
    // the start of the computer when we call GetGameTime()
    ticks_start = 0;
    ticks_start = get_ticks_usec();

    DefaultIP::make_default();

    cursor_shape = CURSOR_ARROW;
}

bool UwpOS::can_draw() const {
    return !minimized;
};

void UwpOS::set_window(Windows::UI::Core::CoreWindow ^ p_window) {
    window = p_window;
}

void UwpOS::screen_size_changed() {
    gl_context->reset();
};

Error UwpOS::initialize(
    const VideoMode& p_desired,
    int p_video_driver,
    int p_audio_driver
) {
    main_loop = NULL;
    outside   = true;

    ContextEGL_UWP::Driver opengl_api_type = ContextEGL_UWP::GLES_2_0;

    if (p_video_driver == VIDEO_DRIVER_GLES2) {
        opengl_api_type = ContextEGL_UWP::GLES_2_0;
    }

    bool gl_initialization_error = false;

    gl_context = NULL;
    while (!gl_context) {
        gl_context = memnew(ContextEGL_UWP(window, opengl_api_type));

        if (gl_context->initialize() != OK) {
            memdelete(gl_context);
            gl_context = NULL;

            if (GLOBAL_GET("rendering/quality/driver/fallback_to_gles2")) {
                if (p_video_driver == VIDEO_DRIVER_GLES2) {
                    gl_initialization_error = true;
                    break;
                }

                p_video_driver  = VIDEO_DRIVER_GLES2;
                opengl_api_type = ContextEGL_UWP::GLES_2_0;
            } else {
                gl_initialization_error = true;
                break;
            }
        }
    }

    while (true) {
        if (opengl_api_type == ContextEGL_UWP::GLES_3_0) {
            if (RasterizerGLES3::is_viable() == OK) {
                RasterizerGLES3::register_config();
                RasterizerGLES3::make_current();
                break;
            } else {
                if (GLOBAL_GET("rendering/quality/driver/fallback_to_gles2")) {
                    p_video_driver  = VIDEO_DRIVER_GLES2;
                    opengl_api_type = ContextEGL_UWP::GLES_2_0;
                    continue;
                } else {
                    gl_initialization_error = true;
                    break;
                }
            }
        }

        if (opengl_api_type == ContextEGL_UWP::GLES_2_0) {
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
            "Your video card driver does not support any of the supported "
            "OpenGL versions.\n"
            "Please update your drivers or if you have a very old or "
            "integrated GPU, upgrade it.",
            "Unable to initialize Video driver"
        );
        return ERR_UNAVAILABLE;
    }

    video_driver_index = p_video_driver;
    gl_context->make_current();
    gl_context->set_use_vsync(video_mode.use_vsync);

    VideoMode vm;
    vm.width     = gl_context->get_window_width();
    vm.height    = gl_context->get_window_height();
    vm.resizable = false;

    ApplicationView ^ view = ApplicationView::GetForCurrentView();
    vm.fullscreen          = view->IsFullScreenMode;

    view->SetDesiredBoundsMode(ApplicationViewBoundsMode::UseVisible);
    view->PreferredLaunchWindowingMode =
        ApplicationViewWindowingMode::PreferredLaunchViewSize;

    if (p_desired.fullscreen != view->IsFullScreenMode) {
        if (p_desired.fullscreen) {
            vm.fullscreen = view->TryEnterFullScreenMode();

        } else {
            view->ExitFullScreenMode();
            vm.fullscreen = false;
        }
    }

    Windows::Foundation::Size desired;
    desired.Width  = p_desired.width;
    desired.Height = p_desired.height;

    view->PreferredLaunchViewSize = desired;

    if (view->TryResizeView(desired)) {
        vm.width  = view->VisibleBounds.Width;
        vm.height = view->VisibleBounds.Height;
    }

    set_video_mode(vm);

    visual_server = memnew(VisualServerRaster);
    // FIXME: Reimplement threaded rendering
    if (get_render_thread_mode() != RENDER_THREAD_UNSAFE) {
        visual_server = memnew(VisualServerWrapMT(visual_server, false));
    }

    visual_server->init();

    input = memnew(InputDefault);

    joypad = ref new JoypadUWP(input);
    joypad->register_events();

    AudioDriverManager::initialize(p_audio_driver);

    power_manager = memnew(PowerUWP);

    managed_object->update_clipboard();

    Clipboard::ContentChanged += ref new EventHandler<Platform::Object ^>(
        managed_object,
        &ManagedType::on_clipboard_changed
    );

    accelerometer = Accelerometer::GetDefault();
    if (accelerometer != nullptr) {
        // 60 FPS
        accelerometer->ReportInterval  = (1.0f / 60.0f) * 1000;
        accelerometer->ReadingChanged += ref new TypedEventHandler<
            Accelerometer ^,
            AccelerometerReadingChangedEventArgs ^>(
            managed_object,
            &ManagedType::on_accelerometer_reading_changed
        );
    }

    magnetometer = Magnetometer::GetDefault();
    if (magnetometer != nullptr) {
        // 60 FPS
        magnetometer->ReportInterval  = (1.0f / 60.0f) * 1000;
        magnetometer->ReadingChanged += ref new TypedEventHandler<
            Magnetometer ^,
            MagnetometerReadingChangedEventArgs ^>(
            managed_object,
            &ManagedType::on_magnetometer_reading_changed
        );
    }

    gyrometer = Gyrometer::GetDefault();
    if (gyrometer != nullptr) {
        // 60 FPS
        gyrometer->ReportInterval  = (1.0f / 60.0f) * 1000;
        gyrometer->ReadingChanged += ref new TypedEventHandler<
            Gyrometer ^,
            GyrometerReadingChangedEventArgs ^>(
            managed_object,
            &ManagedType::on_gyroscope_reading_changed
        );
    }

    if (is_keep_screen_on()) {
        display_request->RequestActive();
    }

    set_keep_screen_on(
        GLOBAL_DEF("display/window/energy_saving/keep_screen_on", true)
    );

    return OK;
}

void UwpOS::set_clipboard(const String& p_text) {
    DataPackage ^ clip       = ref new DataPackage();
    clip->RequestedOperation = DataPackageOperation::Copy;
    clip->SetText(ref new Platform::String((const wchar_t*)p_text.c_str()));

    Clipboard::SetContent(clip);
};

String UwpOS::get_clipboard() const {
    if (managed_object->clipboard != nullptr) {
        return managed_object->clipboard->Data();
    } else {
        return "";
    }
};

void UwpOS::input_event(const Ref<InputEvent>& p_event) {
    input->parse_input_event(p_event);
};

void UwpOS::delete_main_loop() {
    if (main_loop) {
        memdelete(main_loop);
    }
    main_loop = NULL;
}

void UwpOS::set_main_loop(MainLoop* p_main_loop) {
    input->set_main_loop(p_main_loop);
    main_loop = p_main_loop;
}

void UwpOS::finalize() {
    if (main_loop) {
        memdelete(main_loop);
    }

    main_loop = NULL;

    visual_server->finish();
    memdelete(visual_server);
#ifdef OPENGL_ENABLED
    if (gl_context) {
        memdelete(gl_context);
    }
#endif

    memdelete(input);

    joypad = nullptr;
}

void UwpOS::finalize_core() {
    DefaultNetSocket::cleanup();
}

void UwpOS::alert(const String& p_alert, const String& p_title) {
    Platform::String ^ alert = ref new Platform::String(p_alert.c_str());
    Platform::String ^ title = ref new Platform::String(p_title.c_str());

    MessageDialog ^ msg = ref new MessageDialog(alert, title);

    UICommand ^ close = ref new UICommand(
        "Close",
        ref new UICommandInvokedHandler(
            managed_object,
            &UwpOS::ManagedType::alert_close
        )
    );
    msg->Commands->Append(close);
    msg->DefaultCommandIndex = 0;

    managed_object->alert_close_handle = true;

    msg->ShowAsync();
}

void UwpOS::ManagedType::alert_close(IUICommand ^ command) {
    alert_close_handle = false;
}

void UwpOS::ManagedType::on_clipboard_changed(
    Platform::Object ^ sender,
    Platform::Object ^ ev
) {
    update_clipboard();
}

void UwpOS::ManagedType::update_clipboard() {
    DataPackageView ^ data = Clipboard::GetContent();

    if (data->Contains(StandardDataFormats::Text)) {
        create_task(data->GetTextAsync())
            .then([this](Platform::String ^ clipboard_content) {
                this->clipboard = clipboard_content;
            });
    }
}

void UwpOS::ManagedType::on_accelerometer_reading_changed(
    Accelerometer ^ sender,
    AccelerometerReadingChangedEventArgs ^ args
) {
    AccelerometerReading ^ reading = args->Reading;

    os->input->set_accelerometer(Vector3(
        reading->AccelerationX,
        reading->AccelerationY,
        reading->AccelerationZ
    ));
}

void UwpOS::ManagedType::on_magnetometer_reading_changed(
    Magnetometer ^ sender,
    MagnetometerReadingChangedEventArgs ^ args
) {
    MagnetometerReading ^ reading = args->Reading;

    os->input->set_magnetometer(Vector3(
        reading->MagneticFieldX,
        reading->MagneticFieldY,
        reading->MagneticFieldZ
    ));
}

void UwpOS::ManagedType::on_gyroscope_reading_changed(
    Gyrometer ^ sender,
    GyrometerReadingChangedEventArgs ^ args
) {
    GyrometerReading ^ reading = args->Reading;

    os->input->set_magnetometer(Vector3(
        reading->AngularVelocityX,
        reading->AngularVelocityY,
        reading->AngularVelocityZ
    ));
}

void UwpOS::set_mouse_mode(MouseMode p_mode) {
    if (p_mode == MouseMode::MOUSE_MODE_CAPTURED) {
        CoreWindow::GetForCurrentThread()->SetPointerCapture();

    } else {
        CoreWindow::GetForCurrentThread()->ReleasePointerCapture();
    }

    if (p_mode == MouseMode::MOUSE_MODE_CAPTURED
        || p_mode == MouseMode::MOUSE_MODE_HIDDEN) {
        CoreWindow::GetForCurrentThread()->PointerCursor = nullptr;

    } else {
        CoreWindow::GetForCurrentThread()->PointerCursor =
            ref new CoreCursor(CoreCursorType::Arrow, 0);
    }

    mouse_mode = p_mode;

    SetEvent(mouse_mode_changed);
}

UwpOS::MouseMode UwpOS::get_mouse_mode() const {
    return mouse_mode;
}

Point2 UwpOS::get_mouse_position() const {
    return Point2(old_x, old_y);
}

int UwpOS::get_mouse_button_state() const {
    return last_button_state;
}

void UwpOS::set_window_title(const String& p_title) {}

void UwpOS::set_video_mode(const VideoMode& p_video_mode, int p_screen) {
    video_mode = p_video_mode;
}

OS::VideoMode UwpOS::get_video_mode(int p_screen) const {
    return video_mode;
}

void UwpOS::get_fullscreen_mode_list(List<VideoMode>* p_list, int p_screen)
    const {}

String UwpOS::get_name() const {
    return "UWP";
}

OS::Date UwpOS::get_date(bool utc) const {
    SYSTEMTIME systemtime;
    if (utc) {
        GetSystemTime(&systemtime);
    } else {
        GetLocalTime(&systemtime);
    }

    Date date;
    date.day     = systemtime.wDay;
    date.month   = Month(systemtime.wMonth);
    date.weekday = Weekday(systemtime.wDayOfWeek);
    date.year    = systemtime.wYear;
    date.dst     = false;
    return date;
}

OS::Time UwpOS::get_time(bool utc) const {
    SYSTEMTIME systemtime;
    if (utc) {
        GetSystemTime(&systemtime);
    } else {
        GetLocalTime(&systemtime);
    }

    Time time;
    time.hour = systemtime.wHour;
    time.min  = systemtime.wMinute;
    time.sec  = systemtime.wSecond;
    return time;
}

OS::TimeZoneInfo UwpOS::get_time_zone_info() const {
    TIME_ZONE_INFORMATION info;
    bool daylight = false;
    if (GetTimeZoneInformation(&info) == TIME_ZONE_ID_DAYLIGHT) {
        daylight = true;
    }

    TimeZoneInfo ret;
    if (daylight) {
        ret.name = info.DaylightName;
    } else {
        ret.name = info.StandardName;
    }

    // Bias value returned by GetTimeZoneInformation is inverted of what we
    // expect For example on GMT-3 GetTimeZoneInformation return a Bias of 180,
    // so invert the value to get -180
    ret.bias = -info.Bias;
    return ret;
}

uint64_t UwpOS::get_unix_time() const {
    FILETIME ft;
    SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);

    SYSTEMTIME ep;
    ep.wYear         = 1970;
    ep.wMonth        = 1;
    ep.wDayOfWeek    = 4;
    ep.wDay          = 1;
    ep.wHour         = 0;
    ep.wMinute       = 0;
    ep.wSecond       = 0;
    ep.wMilliseconds = 0;
    FILETIME fep;
    SystemTimeToFileTime(&ep, &fep);

    return (*(uint64_t*)&ft - *(uint64_t*)&fep) / 10000000;
};

void UwpOS::delay_usec(uint32_t p_usec) const {
    int msec = p_usec < 1000 ? 1 : p_usec / 1000;

    // no Sleep()
    WaitForSingleObjectEx(GetCurrentThread(), msec, false);
}

uint64_t UwpOS::get_ticks_usec() const {
    uint64_t ticks;

    // This is the number of clock ticks since start
    QueryPerformanceCounter((LARGE_INTEGER*)&ticks);

    // Divide by frequency to get the time in seconds
    // original calculation shown below is subject to overflow
    // with high ticks_per_second and a number of days since the last reboot.
    // time = ticks * 1000000L / ticks_per_second;

    // we can prevent this by either using 128 bit math
    // or separating into a calculation for seconds, and the fraction
    uint64_t seconds = ticks / ticks_per_second;

    // compiler will optimize these two into one divide
    uint64_t leftover = ticks % ticks_per_second;

    // remainder
    uint64_t time = (leftover * 1000000L) / ticks_per_second;

    // seconds
    time += seconds * 1000000L;

    // Subtract the time at game start to get
    // the time since the game started
    time -= ticks_start;
    return time;
}

void UwpOS::process_events() {
    joypad->process_controllers();
    process_key_events();
    input->flush_buffered_events();
}

void UwpOS::process_key_events() {
    for (int i = 0; i < key_event_pos; i++) {
        KeyEvent& kev = key_event_buffer[i];

        Ref<InputEventKey> key_event;
        key_event.instance();
        key_event->set_alt(kev.alt);
        key_event->set_shift(kev.shift);
        key_event->set_control(kev.control);
        key_event->set_echo(kev.echo);
        key_event->set_scancode(kev.scancode);
        key_event->set_physical_scancode(kev.physical_scancode);
        key_event->set_unicode(kev.unicode);
        key_event->set_pressed(kev.pressed);

        input_event(key_event);
    }
    key_event_pos = 0;
}

void UwpOS::queue_key_event(KeyEvent& p_event) {
    // This merges Char events with the previous Key event, so
    // the unicode can be retrieved without sending duplicate events.
    if (p_event.type == KeyEvent::MessageType::CHAR_EVENT_MESSAGE
        && key_event_pos > 0) {
        KeyEvent& old = key_event_buffer[key_event_pos - 1];
        ERR_FAIL_COND(old.type != KeyEvent::MessageType::KEY_EVENT_MESSAGE);

        key_event_buffer[key_event_pos - 1].unicode = p_event.unicode;
        return;
    }

    ERR_FAIL_COND(key_event_pos >= KEY_EVENT_BUFFER_SIZE);

    key_event_buffer[key_event_pos++] = p_event;
}

void UwpOS::set_cursor_shape(CursorShape p_shape) {
    ERR_FAIL_INDEX(p_shape, CURSOR_MAX);

    if (cursor_shape == p_shape) {
        return;
    }

    static const CoreCursorType uwp_cursors[CURSOR_MAX] = {
        CoreCursorType::Arrow,
        CoreCursorType::IBeam,
        CoreCursorType::Hand,
        CoreCursorType::Cross,
        CoreCursorType::Wait,
        CoreCursorType::Wait,
        CoreCursorType::Arrow,
        CoreCursorType::Arrow,
        CoreCursorType::UniversalNo,
        CoreCursorType::SizeNorthSouth,
        CoreCursorType::SizeWestEast,
        CoreCursorType::SizeNortheastSouthwest,
        CoreCursorType::SizeNorthwestSoutheast,
        CoreCursorType::SizeAll,
        CoreCursorType::SizeNorthSouth,
        CoreCursorType::SizeWestEast,
        CoreCursorType::Help
    };

    CoreWindow::GetForCurrentThread()->PointerCursor =
        ref new CoreCursor(uwp_cursors[p_shape], 0);

    cursor_shape = p_shape;
}

OS::CursorShape UwpOS::get_cursor_shape() const {
    return cursor_shape;
}

void UwpOS::set_custom_mouse_cursor(
    const RES& p_cursor,
    CursorShape p_shape,
    const Vector2& p_hotspot
) {
    // TODO
}

Error UwpOS::execute(
    const String& p_path,
    const List<String>& p_arguments,
    bool p_blocking,
    ProcessID* r_child_id,
    String* r_pipe,
    int* r_exitcode,
    bool read_stderr,
    Mutex* p_pipe_mutex
) {
    return FAILED;
};

Error UwpOS::kill(const ProcessID& p_pid) {
    return FAILED;
};

Error UwpOS::set_cwd(const String& p_cwd) {
    return FAILED;
}

String UwpOS::get_executable_path() const {
    return "";
}

void UwpOS::set_icon(const Ref<Image>& p_icon) {}

bool UwpOS::has_environment(const String& p_var) const {
    return false;
};

String UwpOS::get_environment(const String& p_var) const {
    return "";
};

bool UwpOS::set_environment(const String& p_var, const String& p_value) const {
    return false;
}

String UwpOS::get_stdin_string(bool p_block) {
    return String();
}

void UwpOS::move_window_to_foreground() {}

Error UwpOS::shell_open(String p_uri) {
    return FAILED;
}

String UwpOS::get_locale() const {
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP // this should work on phone 8.1,
                                             // but it doesn't
    return "en";
#else
    Platform::String ^ language =
        Windows::Globalization::Language::CurrentInputMethodLanguageTag;
    return String(language->Data()).replace("-", "_");
#endif
}

void UwpOS::release_rendering_thread() {
    gl_context->release_current();
}

void UwpOS::make_rendering_thread() {
    gl_context->make_current();
}

void UwpOS::swap_buffers() {
    gl_context->swap_buffers();
}

bool UwpOS::has_touchscreen_ui_hint() const {
    TouchCapabilities ^ tc = ref new TouchCapabilities();
    return tc->TouchPresent != 0
        || UIViewSettings::GetForCurrentView()->UserInteractionMode
               == UserInteractionMode::Touch;
}

bool UwpOS::has_virtual_keyboard() const {
    return UIViewSettings::GetForCurrentView()->UserInteractionMode
        == UserInteractionMode::Touch;
}

void UwpOS::show_virtual_keyboard(
    const String& p_existing_text,
    const Rect2& p_screen_rect,
    bool p_multiline,
    int p_max_input_length,
    int p_cursor_start,
    int p_cursor_end
) {
    InputPane ^ pane = InputPane::GetForCurrentView();
    pane->TryShow();
}

void UwpOS::hide_virtual_keyboard() {
    InputPane ^ pane = InputPane::GetForCurrentView();
    pane->TryHide();
}

static String format_error_message(DWORD id) {
    LPWSTR messageBuffer = NULL;
    size_t size          = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        id,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&messageBuffer,
        0,
        NULL
    );

    String msg = "Error " + itos(id) + ": " + String(messageBuffer, size);

    LocalFree(messageBuffer);

    return msg;
}

Error UwpOS::open_dynamic_library(
    const String p_path,
    void*& p_library_handle,
    bool p_also_set_library_path
) {
    String full_path = "game/" + p_path;
    p_library_handle = (void*)LoadPackagedLibrary(full_path.c_str(), 0);
    ERR_FAIL_COND_V_MSG(
        !p_library_handle,
        ERR_CANT_OPEN,
        "Can't open dynamic library: " + full_path
            + ", error: " + format_error_message(GetLastError()) + "."
    );
    return OK;
}

Error UwpOS::close_dynamic_library(void* p_library_handle) {
    if (!FreeLibrary((HMODULE)p_library_handle)) {
        return FAILED;
    }
    return OK;
}

Error UwpOS::get_dynamic_library_symbol_handle(
    void* p_library_handle,
    const String p_name,
    void*& p_symbol_handle,
    bool p_optional
) {
    p_symbol_handle = (void*)
        GetProcAddress((HMODULE)p_library_handle, p_name.utf8().get_data());
    if (!p_symbol_handle) {
        if (!p_optional) {
            ERR_FAIL_V_MSG(
                ERR_CANT_RESOLVE,
                "Can't resolve symbol " + p_name
                    + ", error: " + String::num(GetLastError()) + "."
            );
        } else {
            return ERR_CANT_RESOLVE;
        }
    }
    return OK;
}

void UwpOS::run() {
    if (!main_loop) {
        return;
    }

    main_loop->init();

    uint64_t last_ticks = get_ticks_usec();

    int frames     = 0;
    uint64_t frame = 0;

    while (!force_quit) {
        CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(
            CoreProcessEventsOption::ProcessAllIfPresent
        );
        if (managed_object->alert_close_handle) {
            continue;
        }
        process_events(); // get rid of pending events
        if (Main::iteration()) {
            break;
        }
    };

    main_loop->finish();
}

MainLoop* UwpOS::get_main_loop() const {
    return main_loop;
}

String UwpOS::get_user_data_dir() const {
    Windows::Storage::StorageFolder ^ data_folder =
        Windows::Storage::ApplicationData::Current->LocalFolder;

    return String(data_folder->Path->Data()).replace("\\", "/");
}

bool UwpOS::_check_internal_feature_support(const String& p_feature) {
    return p_feature == "pc";
}

OS::PowerState UwpOS::get_power_state() {
    return power_manager->get_power_state();
}

int UwpOS::get_power_seconds_left() {
    return power_manager->get_power_seconds_left();
}

int UwpOS::get_power_percent_left() {
    return power_manager->get_power_percent_left();
}

UwpOS::UwpOS() {
    key_event_pos = 0;
    force_quit    = false;
    alt_mem       = false;
    gr_mem        = false;
    shift_mem     = false;
    control_mem   = false;
    meta_mem      = false;
    minimized     = false;

    pressrc     = 0;
    old_invalid = true;
    mouse_mode  = MOUSE_MODE_VISIBLE;
#ifdef STDOUT_FILE
    stdo = fopen("stdout.txt", "wb");
#endif

    gl_context = NULL;

    display_request = ref new Windows::System::Display::DisplayRequest();

    managed_object     = ref new ManagedType;
    managed_object->os = this;

    mouse_mode_changed =
        CreateEvent(NULL, TRUE, FALSE, L"os_mouse_mode_changed");

    AudioDriverManager::add_driver(&audio_driver);

    Vector<Logger*> loggers;
    loggers.push_back(memnew(WindowsTerminalLogger));
    _set_logger(memnew(CompositeLogger(loggers)));
}

UwpOS::~UwpOS() {
#ifdef STDOUT_FILE
    fclose(stdo);
#endif
}
