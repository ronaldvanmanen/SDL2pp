// SDL2++
//
// Copyright (C) 2025 Ronald van Manen <rvanmanen@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include <cstdint>

#include <SDL2/SDL_events.h>

namespace sdl2
{
    enum class event_type : std::uint32_t
    {
        quit = SDL_QUIT,
        app_terminating = SDL_APP_TERMINATING,
        app_low_memory = SDL_APP_LOWMEMORY,
        app_will_enter_background = SDL_APP_WILLENTERBACKGROUND,
        app_did_enter_background = SDL_APP_DIDENTERBACKGROUND,
        app_will_enter_foreground = SDL_APP_WILLENTERFOREGROUND,
        app_did_enter_foreground = SDL_APP_DIDENTERFOREGROUND,
        locale_changed = SDL_LOCALECHANGED,
        display_event = SDL_DISPLAYEVENT,
        window_event = SDL_WINDOWEVENT,
        sys_wm_event = SDL_SYSWMEVENT,
        key_down = SDL_KEYDOWN,
        key_up = SDL_KEYUP,
        text_editing = SDL_TEXTEDITING,
        text_input = SDL_TEXTINPUT,
        keymap_changed = SDL_KEYMAPCHANGED,
        text_editing_ext = SDL_TEXTEDITING_EXT,
        mouse_motion = SDL_MOUSEMOTION,
        mouse_button_down = SDL_MOUSEBUTTONDOWN,
        mouse_button_up = SDL_MOUSEBUTTONUP,
        mouse_wheel = SDL_MOUSEWHEEL,
        joystick_axis_motion = SDL_JOYAXISMOTION,
        joystick_ball_motion = SDL_JOYBALLMOTION,
        joystick_hat_motion = SDL_JOYHATMOTION,
        joystick_button_down = SDL_JOYBUTTONDOWN,
        joystick_button_up = SDL_JOYBUTTONUP,
        joystick_device_added = SDL_JOYDEVICEADDED,
        joystick_device_removed = SDL_JOYDEVICEREMOVED,
        joystick_battery_updated = SDL_JOYBATTERYUPDATED,
        controller_axis_motion = SDL_CONTROLLERAXISMOTION,
        controller_button_down = SDL_CONTROLLERBUTTONDOWN,
        controller_button_up = SDL_CONTROLLERBUTTONUP,
        controller_device_added = SDL_CONTROLLERDEVICEADDED,
        controller_device_removed = SDL_CONTROLLERDEVICEREMOVED,
        controller_device_remapped = SDL_CONTROLLERDEVICEREMAPPED,
        controller_touchpad_down = SDL_CONTROLLERTOUCHPADDOWN,
        controller_touchpad_motion = SDL_CONTROLLERTOUCHPADMOTION,
        controller_touchpad_up = SDL_CONTROLLERTOUCHPADUP,
        controller_sensor_update = SDL_CONTROLLERSENSORUPDATE,
        finger_down = SDL_FINGERDOWN,
        finger_up = SDL_FINGERUP,
        finger_motion = SDL_FINGERMOTION,
        dollar_gesture = SDL_DOLLARGESTURE,
        dollar_record = SDL_DOLLARRECORD,
        multigesture = SDL_MULTIGESTURE,
        clipboard_update = SDL_CLIPBOARDUPDATE,
        drop_file = SDL_DROPFILE,
        drop_text = SDL_DROPTEXT,
        drop_begin = SDL_DROPBEGIN,
        drop_complete = SDL_DROPCOMPLETE,
        audio_device_added = SDL_AUDIODEVICEADDED,
        audio_device_removed = SDL_AUDIODEVICEREMOVED,
        sensor_update = SDL_SENSORUPDATE,
        render_targets_reset = SDL_RENDER_TARGETS_RESET,
        render_device_reset = SDL_RENDER_DEVICE_RESET,
        poll_sentinel = SDL_POLLSENTINEL,
        user_event = SDL_USEREVENT,
    };
}
