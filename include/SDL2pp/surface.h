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

#include <SDL2/SDL_surface.h>

#include "error.h"
#include "image.h"
#include "palette.h"
#include "pixel_format.h"
#include "pixel_format_traits.h"
#include "size.h"

namespace sdl2
{
    class surface_base
    {
    public:
        surface_base(int width, int height, int depth);

        surface_base(size const& size, int depth);

        surface_base(SDL_Surface * native_handle, bool free_handle);

        surface_base(surface_base const& other);

        ~surface_base();

        surface_base& operator=(surface_base const& other) = delete;

        palette palette();

        SDL_Surface* native_handle();

    private:
        SDL_Surface* _native_handle;

        bool _free_handle;
    };

    void blit(surface_base & source, surface_base & target);

    template<typename TPixelFormat>
    class surface : public surface_base
    {
    public:
        surface(size const& size);

        surface(int width, int height);

        surface(surface_base const& other) = delete;

        surface& operator=(surface const& other) = delete;

        template<typename CallbackFunction>
        void with_lock(CallbackFunction callback);
    };

    template<typename TPixelFormat>
    surface<TPixelFormat>::surface(size const& size)
    : surface_base(size, pixel_format_traits<TPixelFormat>::bits_per_pixel)
    { }

    template<typename TPixelFormat>
    surface<TPixelFormat>::surface(int width, int height)
    : surface_base(width, height, pixel_format_traits<TPixelFormat>::bits_per_pixel)
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
            native_handle->w,
            native_handle->h,
            native_handle->pitch / sizeof(TPixelFormat)
        );

        callback(locked_texture);

        if (must_lock)
        {
            SDL_UnlockSurface(native_handle);
        }
    }
}
