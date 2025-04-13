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

#include "SDL2pp/event.h"

sdl2::event::event()
: _native_handle(SDL_Event())
{
    SDL_zero(_native_handle);
}

sdl2::event_type
sdl2::event::type() const
{
    return static_cast<sdl2::event_type>(_native_handle.type);
}

SDL_Event&
sdl2::event::native_handle() &
{
    return _native_handle;
}

SDL_Event const&
sdl2::event::native_handle() const&
{
    return _native_handle;
}
