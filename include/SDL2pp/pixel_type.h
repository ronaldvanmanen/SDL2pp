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

#include <SDL2/SDL_pixels.h>

namespace sdl2
{
    enum class pixel_type : std::uint32_t
    {
        unknown = SDL_PIXELTYPE_UNKNOWN,
        index1 = SDL_PIXELTYPE_INDEX1,
        index2 = SDL_PIXELTYPE_INDEX2,
        index4 = SDL_PIXELTYPE_INDEX4,
        index8 = SDL_PIXELTYPE_INDEX8,
        packed8 = SDL_PIXELTYPE_PACKED8,
        packed16 = SDL_PIXELTYPE_PACKED16,
        packed32 = SDL_PIXELTYPE_PACKED32,
        array_u8 = SDL_PIXELTYPE_ARRAYU8,
        array_u16 = SDL_PIXELTYPE_ARRAYU16,
        array_u32 = SDL_PIXELTYPE_ARRAYU32,
        array_f16 = SDL_PIXELTYPE_ARRAYF16,
        array_f32 = SDL_PIXELTYPE_ARRAYF32,
    };
}
