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
sdl2::operator<<(std::ostream& stream, const sdl2::blend_mode& value)
{
    switch (value)
    {
        case sdl2::blend_mode::none: stream << "none"; break;
        case sdl2::blend_mode::blend: stream << "blend"; break;
        case sdl2::blend_mode::additive: stream << "additive"; break;
        case sdl2::blend_mode::modulate: stream << "modulate"; break;
        case sdl2::blend_mode::multiply: stream << "multiply"; break;
        case sdl2::blend_mode::invalid: stream << "invalid";  break;
    }

    return stream;
}
