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

namespace sdl2
{
    SDL_Renderer* create_renderer(sdl2::window const& owner, std::int32_t index, sdl2::renderer_flags flags)
    {
        SDL_Renderer* native_handle = SDL_CreateRenderer(
            const_cast<sdl2::window&>(owner).native_handle(),
            index,
            static_cast<std::uint32_t>(flags)
        );
        throw_last_error(native_handle == nullptr);
        return native_handle;
    }
}

sdl2::renderer::renderer(sdl2::window const& owner)
: _native_handle(sdl2::create_renderer(owner, -1, sdl2::renderer_flags::none))
{ }

sdl2::renderer::renderer(sdl2::window const& owner, sdl2::renderer_flags flags)
: _native_handle(sdl2::create_renderer(owner, -1, flags))
{ }

sdl2::renderer::renderer(renderer&& other)
: _native_handle(std::exchange(other._native_handle, nullptr))
{}

sdl2::renderer::~renderer()
{
    if (_native_handle != nullptr)
    {
        SDL_DestroyRenderer(_native_handle);
    }
}

sdl2::size_2d<std::int32_t>
sdl2::renderer::output_size() const
{
    int width, height;
    sdl2::throw_last_error(
        SDL_GetRendererOutputSize(_native_handle, &width, &height) < 0
    );
    return sdl2::size_2d<std::int32_t>(width * px, height * px);
}

sdl2::color
sdl2::renderer::draw_color() const
{
    std::uint8_t r, g, b, a;
    throw_last_error(
        SDL_GetRenderDrawColor(_native_handle, &r, &g, &b, &a) < 0
    );
    return sdl2::color(r, g, b, a);
}

void
sdl2::renderer::draw_color(sdl2::color const& draw_color)
{
    throw_last_error(
        SDL_SetRenderDrawColor(
            _native_handle, draw_color.r, draw_color.g, draw_color.b, draw_color.a
        ) < 0
    );
}

sdl2::blend_mode
sdl2::renderer::draw_blend_mode() const
{
    SDL_BlendMode mode;
    throw_last_error(
        SDL_GetRenderDrawBlendMode(_native_handle, &mode) < 0
    );
    return static_cast<sdl2::blend_mode>(mode);
}

void
sdl2::renderer::draw_blend_mode(sdl2::blend_mode mode)
{
    throw_last_error(
        SDL_SetRenderDrawBlendMode(_native_handle, static_cast<SDL_BlendMode>(mode)) < 0
    );
}

void
sdl2::renderer::clear()
{
    sdl2::throw_last_error(
        SDL_RenderClear(_native_handle) < 0
    );
}

void
sdl2::renderer::present()
{
    SDL_RenderPresent(_native_handle);
}

void
sdl2::renderer::copy(sdl2::texture_base const& texture)
{
    sdl2::throw_last_error(
        SDL_RenderCopy(
            _native_handle,
            const_cast<sdl2::texture_base&>(texture).native_handle(),
            nullptr,
            nullptr) < 0
    );
}

SDL_Renderer* sdl2::renderer::native_handle()
{
    return _native_handle;
}
