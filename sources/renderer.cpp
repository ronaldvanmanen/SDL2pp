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
#include "SDL2pp/texture.h"

sdl2::renderer::renderer(const sdl2::window& owner)
: _wrappee(SDL_CreateRenderer(owner.wrappee(), -1, 0))
{ }

sdl2::renderer::renderer(const sdl2::window& owner, sdl2::renderer_flags flags)
: _wrappee(SDL_CreateRenderer(owner.wrappee(), -1, static_cast<uint32_t>(flags)))
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
sdl2::renderer::output_size() const
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
    uint8_t r, g, b, a;
    throw_last_error(
        SDL_GetRenderDrawColor(_wrappee, &r, &g, &b, &a) < 0
    );
    return sdl2::color(r, g, b, a);
}

void
sdl2::renderer::set_draw_color(const sdl2::color& draw_color)
{
    throw_last_error(
        SDL_SetRenderDrawColor(
            _wrappee, draw_color.r, draw_color.g, draw_color.b, draw_color.a
        ) < 0
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

void
sdl2::renderer::copy(sdl2::texture_base const& texture)
{
    throw_last_error(
        SDL_RenderCopy(_wrappee, texture.wrappee(), nullptr, nullptr) < 0
    );
}

SDL_Renderer* sdl2::renderer::wrappee() const
{
    return _wrappee;
}

sdl2::renderer_flags sdl2::operator|(sdl2::renderer_flags left, sdl2::renderer_flags right)
{
    return static_cast<sdl2::renderer_flags>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}
