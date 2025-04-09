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

#include "SDL2pp/color.h"

const sdl2::color sdl2::color::black(0, 0, 0, 255);

const sdl2::color sdl2::color::white(255, 255, 255, 255);

sdl2::color::color()
: r(0), g(0), b(0), a(0)
{ }

sdl2::color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
: r(r), g(g), b(b), a(a)
{ }

bool
sdl2::operator==(const color& left, const color& right)
{
    return left.r == right.r && left.g == right.g && left.b == right.b && left.a == right.a;
}

bool
sdl2::operator!=(const color& left, const color& right)
{
    return !(left == right);
}

std::ostream&
sdl2::operator<<(std::ostream& stream, const color& value)
{
    return stream
        << static_cast<unsigned int>(value.r)
        << ','
        << static_cast<unsigned int>(value.g)
        << ','
        << static_cast<unsigned int>(value.b)
        << ','
        << static_cast<unsigned int>(value.a);
}
