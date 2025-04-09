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
: _wrappee(SDL_Event())
{
    SDL_zero(_wrappee);
}

sdl2::event::event(const SDL_Event& wrappee)
: _wrappee(wrappee)
{ }

sdl2::event_type
sdl2::event::type() const
{
    return static_cast<sdl2::event_type>(_wrappee.type);
}
