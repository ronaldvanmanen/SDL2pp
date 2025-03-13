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
#include "SDL2pp/renderer.h"

sdl2::renderer::renderer(SDL_Renderer* wrappee)
: _wrappee(wrappee)
{ }

sdl2::renderer::renderer(renderer&& other)
: _wrappee(std::exchange(other._wrappee, nullptr))
{}

sdl2::renderer::~renderer()
{
    if (_wrappee != nullptr)
    {
        SDL_DestroyRenderer(_wrappee);
    }
}

sdl2::size
sdl2::renderer::output_size()
{
    int width, height;
    sdl2::throw_last_error(
        SDL_GetRendererOutputSize(_wrappee, &width, &height) < 0
    );
    return sdl2::size(width, height);
}

sdl2::color
sdl2::renderer::get_draw_color() const
{
    sdl2::color draw_color;
    throw_last_error(
        SDL_GetRenderDrawColor(
            _wrappee, &draw_color.r, &draw_color.g, &draw_color.b, &draw_color.a
        ) < 0
    );
    return draw_color;
}

void
sdl2::renderer::set_draw_color(sdl2::color draw_color)
{
    throw_last_error(
        SDL_SetRenderDrawColor(
            _wrappee, draw_color.r, draw_color.g, draw_color.b, draw_color.a
        ) < 0
    );
}

sdl2::blend_mode
sdl2::renderer::get_draw_blend_mode() const
{
    SDL_BlendMode mode;
    throw_last_error(
        SDL_GetRenderDrawBlendMode(_wrappee, &mode) < 0
    );
    return (sdl2::blend_mode)mode;
}

void
sdl2::renderer::set_draw_blend_mode(sdl2::blend_mode mode)
{
    throw_last_error(
        SDL_SetRenderDrawBlendMode(_wrappee, static_cast<SDL_BlendMode>(mode)) < 0
    );
}

void
sdl2::renderer::clear()
{
    sdl2::throw_last_error(
        SDL_RenderClear(_wrappee) < 0
    );
}

void
sdl2::renderer::present()
{
    SDL_RenderPresent(_wrappee);
}
