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

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include "SDL2pp/events_subsystem.h"

sdl2::events_subsystem::events_subsystem()
{
    SDL_InitSubSystem(SDL_INIT_EVENTS);
}

sdl2::events_subsystem::~events_subsystem()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

boost::optional<sdl2::event>
sdl2::events_subsystem::poll_event()
{
    SDL_Event event;
    if (!SDL_PollEvent(&event))
    {
        return boost::none;
    }
    return sdl2::event(event);
}
