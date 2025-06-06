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

#include "SDL2pp/key_modifier.h"

sdl2::key_modifier sdl2::operator&(sdl2::key_modifier left, sdl2::key_modifier right)
{
    return static_cast<sdl2::key_modifier>(static_cast<std::uint16_t>(left) & static_cast<std::uint16_t>(right));
}

sdl2::key_modifier sdl2::operator|(sdl2::key_modifier left, sdl2::key_modifier right)
{
    return static_cast<sdl2::key_modifier>(static_cast<std::uint16_t>(left) | static_cast<std::uint16_t>(right));
}
