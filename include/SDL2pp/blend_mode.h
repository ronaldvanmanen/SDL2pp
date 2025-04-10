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

#include <iostream>
#include <stdint.h>
#include <SDL2/SDL_blendmode.h>

namespace sdl2
{
    enum class blend_mode : int32_t
    {
        none = SDL_BLENDMODE_NONE,
        blend = SDL_BLENDMODE_BLEND,
        additive = SDL_BLENDMODE_ADD,
        modulate = SDL_BLENDMODE_MOD,
        multiply = SDL_BLENDMODE_MUL,
        invalid = SDL_BLENDMODE_INVALID
    };

    std::ostream& operator<<(std::ostream& stream, const blend_mode& value);
}
