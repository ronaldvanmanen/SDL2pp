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

#include "SDL2pp/texture.h"

namespace sdl2
{
    SDL_Texture* create_texture(
        sdl2::renderer const& owner,
        sdl2::pixel_format format,
        sdl2::texture_access access,
        sdl2::length<std::int32_t> width,
        sdl2::length<std::int32_t> height
    )
    {
        SDL_Texture* native_handle = SDL_CreateTexture(
            const_cast<sdl2::renderer&>(owner).native_handle(),
            static_cast<std::uint32_t>(format),
            static_cast<std::int32_t>(access),
            quantity_cast<std::int32_t>(width),
            quantity_cast<std::int32_t>(height)
        );
        throw_last_error(native_handle == nullptr);
        return native_handle;
    }
}

sdl2::texture_base::texture_base(
    sdl2::renderer const& owner,
    sdl2::pixel_format format,
    sdl2::texture_access access,
    sdl2::length<std::int32_t> width,
    sdl2::length<std::int32_t> height
)
: _native_handle(sdl2::create_texture(owner, format, access, width, height))
{ }

sdl2::texture_base::~texture_base()
{
    if (_native_handle != nullptr)
    {
        SDL_DestroyTexture(_native_handle);
    }
}

SDL_Texture*
sdl2::texture_base::native_handle()
{
    return _native_handle;
}
