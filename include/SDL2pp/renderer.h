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

namespace sdl2
{
    class texture_base;
}

#include <cstdint>

#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>

#include <SDL2/SDL_render.h>

#include "blend_mode.h"
#include "color.h"
#include "error.h"
#include "pixel_format.h"
#include "renderer_flags.h"
#include "size.h"
#include "texture.h"
#include "window.h"

namespace sdl2
{
    class renderer
    {
    public:
        renderer(window const& owner);

        renderer(window const& owner, renderer_flags flags);

        renderer(renderer const& other) = delete;

        renderer(renderer&& other);

        ~renderer();

        renderer& operator=(renderer const& other) = delete;

        size_2d<std::int32_t> output_size() const;

        color draw_color() const;

        void draw_color(color const& draw_color);

        blend_mode draw_blend_mode() const;

        void draw_blend_mode(blend_mode mode);

        void clear();

        void present();

        void copy(texture_base const& texture);

        SDL_Renderer* native_handle();

    private:
        SDL_Renderer* _native_handle;
    };
}
