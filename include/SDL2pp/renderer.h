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

#include <string>

#include <SDL2/SDL_render.h>

#include "blend_mode.h"
#include "color.h"
#include "error.h"
#include "pixel_format.h"
#include "pixel_format_traits.h"
#include "texture_access.h"
#include "texture.h"
#include "size.h"

namespace sdl2
{
    class renderer
    {
    public:
        renderer(SDL_Renderer* wrappee);

        renderer(renderer&& other);

        ~renderer();

        size output_size();

        void set_draw_color(color draw_color);

        color get_draw_color() const;

        void set_draw_blend_mode(blend_mode mode);

        blend_mode get_draw_blend_mode() const;

        void clear();

        void present();

        template<typename TPixelFormat>
        texture<TPixelFormat> create_texture(texture_access access, int width, int height);

        template<typename TPixelFormat>
        texture<TPixelFormat> create_texture(texture_access access, size size);

        template<typename TPixelFormat>
        void copy(texture<TPixelFormat> const& texture);
    
    private:
        SDL_Renderer* _wrappee;
    };

    template<typename TPixelFormat>
    texture<TPixelFormat>
    renderer::create_texture(texture_access access, int width, int height)
    {
        auto pixel_format = pixel_format_traits<TPixelFormat>::format;
        auto wrappee = SDL_CreateTexture(_wrappee, static_cast<uint32_t>(pixel_format), static_cast<uint32_t>(access), width, height);
        throw_last_error(wrappee == nullptr);
        return texture<TPixelFormat>(wrappee);
    }
    
    template<typename TPixelFormat>
    inline
    texture<TPixelFormat>
    renderer::create_texture(texture_access access, size size)
    {
        return create_texture(access, size.width, size.height);
    }

    template<typename TPixelFormat>
    void
    renderer::copy(texture<TPixelFormat> const& texture)
    {
        throw_last_error(
            SDL_RenderCopy(_wrappee, texture.wrappee(), nullptr, nullptr) < 0
        );
    }

}
