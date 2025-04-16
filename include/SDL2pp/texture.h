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
#include <utility>

#include <SDL2/SDL_render.h>

#include "SDL2pp/error.h"
#include "SDL2pp/image.h"
#include "SDL2pp/pixel_format.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/size.h"

namespace sdl2
{
    enum class texture_access : std::int32_t
    {
        static_access = SDL_TEXTUREACCESS_STATIC,
        streaming_access = SDL_TEXTUREACCESS_STREAMING,
        target_access = SDL_TEXTUREACCESS_TARGET
    };

    class texture_base
    {
    protected:
        texture_base(renderer const& owner, pixel_format format, texture_access access, std::int32_t width, std::int32_t height);

        texture_base(texture_base const& other) = delete;

        ~texture_base();

        texture_base& operator=(texture_base const& other) = delete;

    public:
        SDL_Texture* native_handle();

    protected:
        SDL_Texture* _native_handle;
    };

    template<typename TPixelFormat>
    class texture : public texture_base
    {
    public:
        texture(renderer const& owner, texture_access access, std::int32_t width, std::int32_t height);

        texture(renderer const& owner, texture_access access, size const& size);

        texture(texture<TPixelFormat> const& other) = delete;

        texture(texture<TPixelFormat>&& other);

        texture<TPixelFormat>& operator=(texture<TPixelFormat> const& other) = delete;

        template<typename CallbackFunction>
        void with_lock(CallbackFunction callback);
    };

    template<typename TPixelFormat>
    texture<TPixelFormat>::texture(renderer const& owner, texture_access access, std::int32_t width, std::int32_t height)
    : texture_base(owner, pixel_format_traits<TPixelFormat>::format, access, width, height)
    { }

    template<typename TPixelFormat>
    texture<TPixelFormat>::texture(renderer const& owner, texture_access access, size const& size)
    : texture_base(owner, pixel_format_traits<TPixelFormat>::format, access, size.width, size.height)
    { }

    template<typename TPixelFormat>
    texture<TPixelFormat>::texture(sdl2::texture<TPixelFormat>&& other)
    : _native_handle(std::exchange(other._native_handle, nullptr))
    { }

    template<typename TPixelFormat> template <typename CallbackFunction>
    void
    texture<TPixelFormat>::with_lock(CallbackFunction callback)
    {
        void* pixels;
        std::int32_t pitch;
        throw_last_error(
            SDL_LockTexture(_native_handle, nullptr, &pixels, &pitch) < 0
        );

        std::int32_t width, height;
        throw_last_error(
            SDL_QueryTexture(_native_handle, nullptr, nullptr, &width, &height) < 0
        );

        image<TPixelFormat> locked_texture(
            static_cast<TPixelFormat*>(pixels),
            width,
            height,
            pitch / sizeof(TPixelFormat)
        );

        callback(locked_texture);

        SDL_UnlockTexture(_native_handle);
    }
}
