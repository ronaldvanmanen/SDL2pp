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
#include <iostream>
#include <vector>

#include "SDL2/SDL_pixels.h"

#include "color.h"
#include "pixel_format_details.h"

namespace sdl2
{
    class palette
    {
    public:
        class indexed_color;

        class const_indexed_color;

    public:
        palette(std::size_t size);

        palette(pixel_format_details & owner);

        palette(std::initializer_list<color> colors);

        palette(std::vector<color> const& colors);

        palette(palette const& other);

        palette(palette && other);

        palette(SDL_Palette *native_handle, bool free_handle);

        ~palette();

        palette& operator=(std::initializer_list<color> colors);

        palette& operator=(std::vector<color> const& colors);

        palette& operator=(palette const& other) = delete;

        indexed_color operator[](std::size_t index);

        const_indexed_color operator[](std::size_t index) const;

        std::size_t size() const;

        SDL_Palette* native_handle();

    private:
        SDL_Palette* _native_handle;

        bool _free_handle;
    };

    class palette::indexed_color
    {
    public:
        indexed_color(palette * owner, std::size_t index);

        indexed_color(indexed_color const& other);

        indexed_color(indexed_color && other);

        indexed_color& operator=(color const & value);

        color const& get() const;

        operator color const&() const;

    private:
        palette * _owner;

        std::size_t _index;
    };

    class palette::const_indexed_color
    {
    public:
        const_indexed_color(palette const* owner, std::size_t index);

        const_indexed_color(const_indexed_color const& other);

        color const& get() const;

        operator color const&() const;

    private:
        palette const * _owner;

        std::size_t _index;
    };

    bool operator==(palette::indexed_color const& left, palette::indexed_color const& right);

    bool operator!=(palette::indexed_color const& left, palette::indexed_color const& right);

    bool operator==(palette::indexed_color const& left, color const& right);

    bool operator!=(palette::indexed_color const& left, color const& right);

    bool operator==(color const& left, palette::indexed_color const& right);

    bool operator!=(color const& left, palette::indexed_color const& right);

    std::ostream& operator<<(std::ostream& stream, palette::indexed_color const& value);
}
