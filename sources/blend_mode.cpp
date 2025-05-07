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

#include "SDL2pp/blend_mode.h"

std::ostream&
sdl2::operator<<(std::ostream& stream, sdl2::blend_mode const& value)
{
    #define RETURN(X) return stream << #X;
    #define CASE(X) case X: RETURN(X)
    #define DEFAULT(X) default: RETURN(X);

    switch (value)
    {
        CASE(sdl2::blend_mode::none)
        CASE(sdl2::blend_mode::blend)
        CASE(sdl2::blend_mode::additive)
        CASE(sdl2::blend_mode::modulate)
        CASE(sdl2::blend_mode::multiply)
        CASE(sdl2::blend_mode::invalid)
        DEFAULT(sdl2::blend_mode::invalid)
    }
}
