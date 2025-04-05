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

#include <stdint.h>

#include "pixel_format.h"

namespace sdl2
{
    class alignas(alignof(uint32_t)) argb8888
    {
    public:
        static constexpr pixel_format format = pixel_format::argb8888;

    public:
        argb8888();

        argb8888(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

        argb8888(const argb8888 &other);

        argb8888& operator=(const argb8888 &other);

    public:
        uint8_t b, g, r, a;
    };
}