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

namespace sdl2
{
    SDL_Window* create_window(std::string const& title, sdl2::length<std::int32_t> width, sdl2::length<std::int32_t> height, sdl2::window_flags flags)
    {
        SDL_Window* native_handle =
            SDL_CreateWindow(
                title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                quantity_cast<std::int32_t>(width),
                quantity_cast<std::int32_t>(height),
                static_cast<std::uint32_t>(flags)
            );
        sdl2::throw_last_error(native_handle == nullptr);        
        return native_handle;
    }
}

sdl2::window::window(std::string const& title, sdl2::length<std::int32_t> width, sdl2::length<std::int32_t> height)
: _native_handle(sdl2::create_window(title, width, height, sdl2::window_flags::none))
{ }

sdl2::window::window(std::string const& title, sdl2::length<std::int32_t> width, sdl2::length<std::int32_t> height, sdl2::window_flags flags)
: _native_handle(sdl2::create_window(title, width, height, flags))
{ }

sdl2::window::window(sdl2::window&& other)
: _native_handle(std::exchange(other._native_handle, nullptr))
{ }

sdl2::window::~window()
{
    if (_native_handle != nullptr)
    {
        SDL_DestroyWindow(_native_handle);
    }
}

sdl2::size_2d<std::int32_t>
sdl2::window::size() const
{
    int width, height;
    SDL_GetWindowSize(_native_handle, &width, &height);
    return sdl2::size_2d<std::int32_t>(
        width * px,
        height * px
    );
}

void
sdl2::window::raise()
{
    SDL_RaiseWindow(_native_handle);
}

void
sdl2::window::update_surface()
{
    throw_last_error(SDL_UpdateWindowSurface(_native_handle) < 0);
}

SDL_Window*
sdl2::window::native_handle()
{
    return _native_handle;
}
