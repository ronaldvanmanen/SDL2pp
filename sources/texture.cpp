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

sdl2::texture_base::texture_base(const renderer& owner, pixel_format format, texture_access access, int width, int height)
: _wrappee(
    SDL_CreateTexture(
        owner.wrappee(),
        static_cast<uint32_t>(format),
        static_cast<uint32_t>(access),
        width,
        height
    )
)
{
    throw_last_error(_wrappee == nullptr);
}

sdl2::texture_base::~texture_base()
{
    if (_wrappee != nullptr)
    {
        SDL_DestroyTexture(_wrappee);
    }
}

SDL_Texture*
sdl2::texture_base::wrappee() const
{
    return _wrappee;
}
