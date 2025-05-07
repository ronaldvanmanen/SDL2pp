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
#include "SDL2pp/palette.h"

sdl2::palette::palette(SDL_Palette *native_handle, bool free_handle)
: _native_handle(native_handle)
, _free_handle(free_handle)
{ }

sdl2::palette::palette(std::size_t size)
: _native_handle(SDL_AllocPalette(static_cast<int>(size)))
, _free_handle(true)
{ }

sdl2::palette::palette(sdl2::pixel_format_details & owner)
: _native_handle(owner.native_handle()->palette)
, _free_handle(false)
{ }

sdl2::palette::palette(std::initializer_list<color> colors)
: _native_handle(SDL_AllocPalette(static_cast<int>(colors.size())))
, _free_handle(true)
{
    SDL_SetPaletteColors(
        _native_handle,
        reinterpret_cast<SDL_Color const*>(colors.begin()),
        0,
        static_cast<int>(colors.size()));
}

sdl2::palette::palette(sdl2::palette && other)
: _native_handle(std::exchange(other._native_handle, nullptr))
, _free_handle(std::exchange(other._free_handle, false))
{ }

sdl2::palette::~palette()
{
    if (_free_handle && _native_handle != nullptr)
    {
        SDL_FreePalette(_native_handle);
    }
} 

sdl2::color const&
sdl2::palette::operator[](std::size_t index) const
{
    return reinterpret_cast<sdl2::color*>(_native_handle->colors)[index];
}

std::size_t
sdl2::palette::size() const
{
    return _native_handle->ncolors;
}

void
sdl2::palette::update(std::vector<sdl2::color> const& colors)
{
    throw_last_error(
        SDL_SetPaletteColors(
            _native_handle,
            reinterpret_cast<SDL_Color const*>(&colors[0]),
            0,
            static_cast<int>(colors.size())
        ) < 0
    );
}
