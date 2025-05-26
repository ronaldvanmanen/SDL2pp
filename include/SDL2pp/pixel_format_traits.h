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

#include "pixel_bit_depth.h"
#include "pixel_byte_depth.h"
#include "pixel_format.h"
#include "pixel_type.h"

namespace sdl2
{
    template<typename TPixelFormat>
    class pixel_format_traits
    {
    public:
        static constexpr sdl2::pixel_format format = TPixelFormat::format;

        static constexpr sdl2::pixel_type pixel_type = static_cast<sdl2::pixel_type>(SDL_PIXELTYPE(static_cast<SDL_PixelFormatEnum>(format)));

        static constexpr sdl2::pixel_bit_depth bits_per_pixel() noexcept
        {
            return static_cast<sdl2::pixel_bit_depth>(
                SDL_BITSPERPIXEL(static_cast<SDL_PixelFormatEnum>(format))
            );
        }

        static constexpr sdl2::pixel_byte_depth bytes_per_pixel() noexcept
        {
            return static_cast<sdl2::pixel_byte_depth>(
                SDL_BYTESPERPIXEL(static_cast<SDL_PixelFormatEnum>(format))
            );
        }
    };
}
