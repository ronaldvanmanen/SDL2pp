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

#include "SDL2pp/argb8888.h"

sdl2::argb8888::argb8888() { }

sdl2::argb8888::argb8888(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
: a(a), r(r), g(g), b(b)
{ }

sdl2::argb8888::argb8888(const sdl2::argb8888 &other)
: a(other.a), r(other.r), g(other.g), b(other.b)
{ }

sdl2::argb8888&
sdl2::argb8888::operator=(const sdl2::argb8888 &other)
{
    if (this != &other)
    {
        a = other.a;
        r = other.r;
        g = other.g;
        b = other.b;
    }
    return *this;    
}
