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

#include <functional>
#include <string>

#include <SDL2/SDL_render.h>

#include "SDL2pp/error.h"
#include "SDL2pp/image.h"

namespace sdl2
{
    template<typename TPixelFormat>
    class texture
    {
    public:
        texture(SDL_Texture* wrappee);

        texture(texture<TPixelFormat>&& other);

        ~texture();

        int width() const;

        int height() const;

        template<typename CallbackFunction>
        void with_lock(CallbackFunction fun);

        SDL_Texture* wrappee() const;

    private:
        SDL_Texture* _wrappee;
    };

    template<typename TPixelFormat>
    texture<TPixelFormat>::texture(SDL_Texture* wrappee)
    : _wrappee(wrappee)
    { }
    
    template<typename TPixelFormat>
    texture<TPixelFormat>::texture(sdl2::texture<TPixelFormat>&& other)
    : _wrappee(std::exchange(other._wrappee, nullptr))
    { }
    
    template<typename TPixelFormat>
    texture<TPixelFormat>::~texture()
    {
        if (_wrappee != nullptr)
        {
            SDL_DestroyTexture(_wrappee);
        }
    }

    template<typename TPixelFormat>
    int
    texture<TPixelFormat>::width() const
    {
        int width;
        throw_last_error(
            SDL_QueryTexture(_wrappee, nullptr, nullptr, &width, nullptr) < 0
        );
        return width;
    }

    template<typename TPixelFormat>
    int
    texture<TPixelFormat>::height() const
    {
        int height;
        throw_last_error(
            SDL_QueryTexture(_wrappee, nullptr, nullptr, nullptr, &height) < 0
        );
        return height;
    }


    template<typename TPixelFormat> template <typename CallbackFunction>
    void
    texture<TPixelFormat>::with_lock(CallbackFunction callback)
    {
        void* pixels;
        int pitch;
        throw_last_error(
            SDL_LockTexture(_wrappee, nullptr, &pixels, &pitch) < 0
        );

        int width, height;
        throw_last_error(
            SDL_QueryTexture(_wrappee, nullptr, nullptr, &width, &height) < 0
        );

        image<TPixelFormat> locked_texture(
            static_cast<TPixelFormat*>(pixels),
            width,
            height,
            pitch / sizeof(TPixelFormat)
        );

        callback(locked_texture);

        SDL_UnlockTexture(_wrappee);
    }

    template<typename TPixelFormat>
    SDL_Texture* texture<TPixelFormat>::wrappee() const
    {
        return _wrappee;
    }
}