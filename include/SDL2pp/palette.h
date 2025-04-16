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

#include <cstdint>
#include <vector>

#include "SDL2/SDL_pixels.h"

#include "color.h"

namespace sdl2
{
    class palette
    {
    public:
        palette(SDL_Palette *native_handle, bool free_handle);

        palette(std::size_t size);

        palette(std::initializer_list<sdl2::color> colors);

        palette(palette const& other) = delete;

        palette(palette && other);

        ~palette();

        palette operator=(palette const& other) = delete;

        color const& operator[](std::size_t index) const;

        void update(std::vector<sdl2::color> const& colors);

        std::size_t size() const;

    private:
        SDL_Palette* _native_handle;

        bool _free_handle;
    };
}
