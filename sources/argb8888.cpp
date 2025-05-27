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

#include "SDL2pp/argb8888.h"

const sdl2::argb8888 sdl2::argb8888::black(255_a8, 0_r8, 0_g8, 0_b8);

const sdl2::argb8888 sdl2::argb8888::white(255_a8, 255_r8, 255_g8, 255_b8);

sdl2::argb8888::argb8888()
: argb8888(0_a8, 0_r8, 0_g8, 0_b8)
{ }

sdl2::argb8888::argb8888(sdl2::a8 a, sdl2::r8 r, sdl2::g8 g, sdl2::b8 b)
: b(b), r(r), g(g), a(a)
{ }

sdl2::argb8888::argb8888(argb8888 const& other)
: b(other.b), r(other.r), g(other.g), a(other.a)
{ }

sdl2::argb8888&
sdl2::argb8888::operator=(argb8888 const& other)
{
    if (this != &other)
    {
        b = other.b;
        r = other.r;
        g = other.g;
        a = other.a;
    }
    return *this;
}

bool
sdl2::argb8888::operator==(argb8888 const& other) const
{
    return b == other.b && r == other.r && g == other.g && a == other.a;
}

std::ostream&
sdl2::operator<<(std::ostream& stream, argb8888 const& value)
{
    return stream
        << static_cast<unsigned int>(value.a)
        << ','
        << static_cast<unsigned int>(value.r)
        << ','
        << static_cast<unsigned int>(value.g)
        << ','
        << static_cast<unsigned int>(value.b);
}
