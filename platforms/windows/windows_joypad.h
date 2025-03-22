// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef WINDOWS_JOYPAD_H
#define WINDOWS_JOYPAD_H

#include "windows_os.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h> // on unix the file is called "xinput.h", on windows I'm sure it won't mind

#ifndef SAFE_RELEASE // when Windows Media Device M? is not present
#define SAFE_RELEASE(x)                                                        \
    if (x != NULL) {                                                           \
        x->Release();                                                          \
        x = NULL;                                                              \
    }
#endif

#ifndef XUSER_MAX_COUNT
#define XUSER_MAX_COUNT 4
#endif

class WindowsJoypad {
public:
    WindowsJoypad();
    WindowsJoypad(InputDefault* _input, HWND* hwnd);
    ~WindowsJoypad();

    void probe_joypads();
    void process_joypads();

private:
    enum {
        JOYPADS_MAX           = 16,
        JOY_AXIS_COUNT        = 6,
        MIN_JOY_AXIS          = 10,
        MAX_JOY_AXIS          = 32768,
        MAX_JOY_BUTTONS       = 128,
        KEY_EVENT_BUFFER_SIZE = 512,
        MAX_TRIGGER           = 255
    };

    struct dinput_gamepad {
        int id;
        bool attached;
        bool confirmed;
        bool last_buttons[MAX_JOY_BUTTONS];
        DWORD last_pad;

        LPDIRECTINPUTDEVICE8 di_joy;
        List<LONG> joy_axis;
        GUID guid;

        dinput_gamepad() {
            id        = -1;
            last_pad  = -1;
            attached  = false;
            confirmed = false;

            for (int i = 0; i < MAX_JOY_BUTTONS; i++) {
                last_buttons[i] = false;
            }
        }
    };

    struct xinput_gamepad {
        int id;
        bool attached;
        bool vibrating;
        DWORD last_packet;
        XINPUT_STATE state;
        uint64_t ff_timestamp;
        uint64_t ff_end_timestamp;

        xinput_gamepad() {
            attached         = false;
            vibrating        = false;
            ff_timestamp     = 0;
            ff_end_timestamp = 0;
            last_packet      = 0;
        }
    };

    typedef DWORD(WINAPI* XInputGetState_t)(
        DWORD dwUserIndex,
        XINPUT_STATE* pState
    );
    typedef DWORD(WINAPI* XInputSetState_t)(
        DWORD dwUserIndex,
        XINPUT_VIBRATION* pVibration
    );

    HWND* hWnd;
    HANDLE xinput_dll;
    LPDIRECTINPUT8 dinput;
    InputDefault* input;

    int id_to_change;
    int slider_count;
    int joypad_count;
    bool attached_joypads[JOYPADS_MAX];
    dinput_gamepad d_joypads[JOYPADS_MAX];
    xinput_gamepad x_joypads[XUSER_MAX_COUNT];

    static BOOL CALLBACK
    enumCallback(const DIDEVICEINSTANCE* p_instance, void* p_context);
    static BOOL CALLBACK
    objectsCallback(const DIDEVICEOBJECTINSTANCE* instance, void* context);

    void setup_joypad_object(const DIDEVICEOBJECTINSTANCE* ob, int p_joy_id);
    void close_joypad(int id = -1);
    void load_xinput();
    void unload_xinput();

    void post_hat(int p_device, DWORD p_dpad);

    bool have_device(const GUID& p_guid);
    bool is_xinput_device(const GUID* p_guid);
    bool setup_dinput_joypad(const DIDEVICEINSTANCE* instance);
    void joypad_vibration_start_xinput(
        int p_device,
        float p_weak_magnitude,
        float p_strong_magnitude,
        float p_duration,
        uint64_t p_timestamp
    );
    void joypad_vibration_stop_xinput(int p_device, uint64_t p_timestamp);

    InputDefault::JoyAxis axis_correct(
        int p_val,
        bool p_xinput  = false,
        bool p_trigger = false,
        bool p_negate  = false
    ) const;
    XInputGetState_t xinput_get_state;
    XInputSetState_t xinput_set_state;
};

#endif // WINDOWS_JOYPAD_H
