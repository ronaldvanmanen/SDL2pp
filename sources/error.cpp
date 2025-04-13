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

#include <SDL2/SDL_error.h>

#include "SDL2pp/error.h"

sdl2::error::error(std::string const& what_arg)
: std::runtime_error(what_arg)
{ }

sdl2::error::error(const char* what_arg)
: std::runtime_error(what_arg)
{ }

sdl2::error::error(sdl2::error const& other)
: std::runtime_error(other)
{ }

sdl2::error&
sdl2::error::operator=(error const& other)
{
    if (this != &other)
    {
        std::runtime_error::operator= (other);
    }
    return *this;
}

void
sdl2::throw_last_error(bool condition)
{
    if (condition)
    {
        throw sdl2::error(SDL_GetError());
    }
}
