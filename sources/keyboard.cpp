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

#include <SDL2/SDL_keyboard.h>

#include "SDL2pp/keyboard.h"

sdl2::keyboard_state
sdl2::keyboard::state()
{
    int key_count;
    auto key_states = SDL_GetKeyboardState(&key_count);
    return sdl2::keyboard_state(key_states, key_count);
}

sdl2::key_modifier_set
sdl2::keyboard::modifier_state()
{
    return key_modifier_set(static_cast<sdl2::key_modifier>(SDL_GetModState()));
}
