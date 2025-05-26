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

#include <cstdint>

#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>

#include <SDL2/SDL_surface.h>

#include "error.h"
#include "image.h"
#include "length.h"
#include "pixel_format.h"
#include "pixel_format_traits.h"
#include "size.h"
#include "window.h"

namespace sdl2
{
    class surface_base
    {
    protected:
        surface_base(length<std::int32_t> width, length<std::int32_t> height, pixel_bit_depth depth);

        surface_base(size_2d<std::int32_t> const& size, sdl2::pixel_bit_depth depth);

        surface_base(window & owner);

        surface_base(surface_base const& other);

        surface_base(SDL_Surface * native_handle, bool free_handle);

    public:
        ~surface_base();

        surface_base& operator=(surface_base const& other) = delete;

        SDL_Surface* native_handle();

        void blit(surface_base & source);

    private:
        SDL_Surface* _native_handle;

        bool _free_handle;
    };

    template<typename TPixelFormat>
    class surface : public surface_base
    {
    public:
        surface(length<std::int32_t> width, length<std::int32_t> height);

        surface(size_2d<std::int32_t> const& size);

        surface(window & owner);

        surface(surface<TPixelFormat> const& other);

        surface<TPixelFormat>& operator=(surface<TPixelFormat> const& other) = delete;

        template<typename CallbackFunction>
        void with_lock(CallbackFunction callback);
    };

    template<typename TPixelFormat>
    surface<TPixelFormat>::surface(length<std::int32_t> width, length<std::int32_t> height)
    : surface_base(width, height, pixel_format_traits<TPixelFormat>::bits_per_pixel())
    { }

    template<typename TPixelFormat>
    surface<TPixelFormat>::surface(size_2d<std::int32_t> const& size)
    : surface_base(size, pixel_format_traits<TPixelFormat>::bits_per_pixel())
    { }

    template<typename TPixelFormat>
    surface<TPixelFormat>::surface(window & owner)
    : surface_base(owner)
    { }

    template<typename TPixelFormat>
    surface<TPixelFormat>::surface(surface<TPixelFormat> const& other)
    : surface_base(other)
    { }

    template<typename TPixelFormat> template <typename CallbackFunction>
    void
    surface<TPixelFormat>::with_lock(CallbackFunction callback)
    {
        auto native_handle = this->native_handle();

        const bool must_lock = SDL_MUSTLOCK(native_handle);

        if (must_lock)
        {
            throw_last_error(
                SDL_LockSurface(native_handle) < 0
            );
        }

        image<TPixelFormat> locked_texture(
            static_cast<TPixelFormat*>(native_handle->pixels),
            native_handle->w * px,
            native_handle->h * px,
            native_handle->pitch / sizeof(TPixelFormat)
        );

        callback(locked_texture);

        if (must_lock)
        {
            SDL_UnlockSurface(native_handle);
        }
    }
}
