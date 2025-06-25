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

#include "SDL2pp/keyboard_event.h"

sdl2::keyboard_event::keyboard_event(SDL_Event const& native_handle)
: _native_handle(native_handle)
{ }

sdl2::key_event_type
sdl2::keyboard_event::type() const
{
    return static_cast<sdl2::key_event_type>(_native_handle.key.type);
}

std::uint32_t
sdl2::keyboard_event::timestamp() const
{
    return _native_handle.key.timestamp;
}

std::uint32_t
sdl2::keyboard_event::window_id() const
{
    return _native_handle.key.windowID;
}

sdl2::key_state
sdl2::keyboard_event::state() const
{
    return static_cast<sdl2::key_state>(_native_handle.key.state);
}

std::uint8_t
sdl2::keyboard_event::repeat() const
{
    return _native_handle.key.repeat;
}


sdl2::scan_code
sdl2::keyboard_event::scan_code() const
{
    return static_cast<sdl2::scan_code>(_native_handle.key.keysym.scancode);
}        

sdl2::key_code
sdl2::keyboard_event::key_code() const
{
    return static_cast<sdl2::key_code>(_native_handle.key.keysym.sym);
}

sdl2::key_modifier_set
sdl2::keyboard_event::key_modifiers() const
{
    return key_modifier_set(static_cast<sdl2::key_modifier>(_native_handle.key.keysym.mod));
}
