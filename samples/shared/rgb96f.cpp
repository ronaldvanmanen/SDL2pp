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

#include "rgb96f.h"

const sdl2::rgb96f sdl2::rgb96f::black { .r = 0.0, .g = 0.0, .b = 0.0 };

const sdl2::rgb96f sdl2::rgb96f::white { .r= 1.0, .g = 1.0, .b = 1.0 };

bool
sdl2::rgb96f::operator==(sdl2::rgb96f const& other) const
{
    return r == other.r && g == other.g && b == other.b;
}

sdl2::rgb96f&
sdl2::rgb96f::operator+=(sdl2::rgb96f const& other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

sdl2::rgb96f
sdl2::operator+(sdl2::rgb96f const& left, sdl2::rgb96f const& right)
{
    return sdl2::rgb96f {
        .r = left.r + right.r,
        .g = left.g + right.g,
        .b = left.b + right.b,
    };
}

sdl2::rgb96f
sdl2::operator*(sdl2::rgb96f const& left, sdl2::rgb96f const& right)
{
    return sdl2::rgb96f {
        .r = left.r * right.r,
        .g = left.g * right.g,
        .b = left.b * right.b,
    };
}

sdl2::rgb96f
sdl2::operator*(sdl2::rgb96f const& color, float scalar)
{
    return sdl2::rgb96f {
        .r = color.r * scalar,
        .g = color.g * scalar,
        .b = color.b * scalar,
    };
}

sdl2::rgb96f
sdl2::operator*(float scalar, sdl2::rgb96f const& color)
{
    return sdl2::rgb96f {
        .r = scalar * color.r,
        .g = scalar * color.g,
        .b = scalar * color.b,
    };
}
