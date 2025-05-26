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
#include "SDL2pp/surface.h"

namespace sdl2
{
    SDL_Surface* create_surface(sdl2::length<std::int32_t> width, sdl2::length<std::int32_t> height, sdl2::pixel_bit_depth depth)
    {
        SDL_Surface* native_handle = SDL_CreateRGBSurface(
            0,
            boost::units::quantity_cast<std::int32_t>(width),
            boost::units::quantity_cast<std::int32_t>(height),
            depth,
            0,
            0,
            0,
            0
        );
        throw_last_error(native_handle == nullptr);
        return native_handle;
    }
}


sdl2::surface_base::surface_base(sdl2::length<std::int32_t> width, sdl2::length<std::int32_t> height, sdl2::pixel_bit_depth depth)
: _native_handle(sdl2::create_surface(width, height, depth))
, _free_handle(true)
{
    throw_last_error(_native_handle == nullptr);
}

sdl2::surface_base::surface_base(sdl2::size_2d<std::int32_t> const& size, sdl2::pixel_bit_depth depth)
: _native_handle(sdl2::create_surface(size.width, size.height, depth))
, _free_handle(true)
{
    throw_last_error(_native_handle == nullptr);
}

sdl2::surface_base::surface_base(sdl2::window & window)
: _native_handle(SDL_GetWindowSurface(window.native_handle()))
, _free_handle(false)
{ }

sdl2::surface_base::surface_base(SDL_Surface * native_handle, bool free_handle)
: _native_handle(native_handle)
, _free_handle(free_handle)
{
    throw_last_error(_native_handle == nullptr);
}

sdl2::surface_base::surface_base(sdl2::surface_base const& other)
: _native_handle(
    SDL_CreateRGBSurface(
        0,
        other._native_handle->w,
        other._native_handle->h,
        other._native_handle->pitch,
        other._native_handle->format->Rmask,
        other._native_handle->format->Gmask,
        other._native_handle->format->Bmask,
        other._native_handle->format->Amask
    )
)
, _free_handle(true)
{
    throw_last_error(
        SDL_BlitSurface(other._native_handle, nullptr, _native_handle, nullptr) < 0
    );
}

sdl2::surface_base::~surface_base()
{
    if (_free_handle && _native_handle != nullptr)
    {
        SDL_FreeSurface(_native_handle);
    }
}

SDL_Surface*
sdl2::surface_base::native_handle()
{
    return _native_handle;
}

void
sdl2::surface_base::blit(sdl2::surface_base & source)
{
    auto source_handle = source.native_handle();
    auto source_rect = SDL_Rect { 0, 0, source_handle->w, source_handle->h };
    auto target_handle = this->native_handle();
    auto target_rect = SDL_Rect { 0, 0, target_handle->w, target_handle->h };
    throw_last_error(
        SDL_BlitSurface(source_handle, &source_rect, target_handle, &target_rect) < 0
    );
}
