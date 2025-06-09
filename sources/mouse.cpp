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

#include <SDL2/SDL_mouse.h>

#include "SDL2pp/error.h"
#include "SDL2pp/mouse.h"

bool
sdl2::mouse::relative_mode()
{
    return SDL_TRUE == SDL_GetRelativeMouseMode();
}

void
sdl2::mouse::relative_mode(bool enabled)
{
    sdl2::throw_last_error(
        SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) < 0
    );
}

sdl2::mouse_state
sdl2::mouse::relative_state()
{
    std::int32_t x, y;
    std::uint32_t buttons = SDL_GetRelativeMouseState(&x, &y);
    return mouse_state { x, y, buttons };
}

bool
sdl2::mouse_state::pressed(std::uint32_t which) const
{
    return ((this->buttons & which) == which);
}