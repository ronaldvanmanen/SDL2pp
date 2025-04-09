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

#include <SDL2/SDL_render.h>

namespace sdl2
{
    class texture_base;
}

#include "color.h"
#include "error.h"
#include "pixel_format.h"
#include "size.h"
#include "window.h"

namespace sdl2
{
    enum class renderer_flags
    {
        none = 0,
        software = SDL_RENDERER_SOFTWARE,
        accelerated = SDL_RENDERER_ACCELERATED,
        present_vsync = SDL_RENDERER_PRESENTVSYNC,
        target_texture = SDL_RENDERER_TARGETTEXTURE,
    };

    class renderer
    {
    public:
        renderer(const window& owner);

        renderer(const window& owner, renderer_flags flags);

        renderer(const renderer& other) = delete;

        renderer(renderer&& other);

        ~renderer();

        renderer& operator=(const renderer& other) = delete;

        size output_size() const;

        color get_draw_color() const;

        void set_draw_color(const color& draw_color);

        void clear();

        void present();

        void copy(texture_base const& texture);

        SDL_Renderer* wrappee() const;

    private:
        SDL_Renderer* _wrappee;
    };

    renderer_flags operator|(renderer_flags left, renderer_flags right);
}
