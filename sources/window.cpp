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

#include "SDL2pp/error.h"
#include "SDL2pp/window.h"

sdl2::window::window(const std::string& title, int width, int height)
: _wrappee(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0))
{
    sdl2::throw_last_error(_wrappee == nullptr);
}

sdl2::window::window(const std::string& title, int width, int height, sdl2::window_flags flags)
: _wrappee(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, static_cast<std::uint32_t>(flags)))
{
    sdl2::throw_last_error(_wrappee == nullptr);
}

sdl2::window::window(sdl2::window&& other)
: _wrappee(std::exchange(other._wrappee, nullptr))
{ }

sdl2::window::~window()
{
    if (_wrappee != nullptr)
    {
        SDL_DestroyWindow(_wrappee);
    }
}

SDL_Window*
sdl2::window::wrappee() const
{
    return _wrappee;
}

sdl2::window_flags sdl2::operator|(sdl2::window_flags left, sdl2::window_flags right)
{
    return static_cast<sdl2::window_flags>(static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right));
}
