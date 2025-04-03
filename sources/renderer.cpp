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
