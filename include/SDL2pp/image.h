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

#include "length.h"

namespace sdl2
{
    template<typename TPixelFormat>
    class image
    {
    public:
        image(length<std::int32_t> width, length<std::int32_t> height);

        image(TPixelFormat* pixels, length<std::int32_t> width, length<std::int32_t> height, std::int32_t pitch);

        length<std::int32_t> width() const;

        length<std::int32_t> height() const;

        TPixelFormat& operator()(offset<int32_t> x, offset<int32_t> y);

        TPixelFormat const& operator()(offset<int32_t> x, offset<int32_t> y) const;

    private:
        TPixelFormat* _pixels;

        length<std::int32_t> _width;

        length<std::int32_t> _height;

        std::int32_t _pitch;
    };

    template<typename TPixelFormat>
    image<TPixelFormat>::image(length<std::int32_t> width, length<std::int32_t> height)
    : image(
        new TPixelFormat[boost::units::quantity_cast<std::size_t>(height * width)],
        width,
        height,
        boost::units::quantity_cast<std::int32_t>(width)
    )
    {}

    template<typename TPixelFormat>
    image<TPixelFormat>::image(TPixelFormat* pixels, length<std::int32_t> width, length<std::int32_t> height, std::int32_t pitch)
    : _pixels(pixels)
    , _width(width)
    , _height(height)
    , _pitch(pitch)
    { }

    template<typename TPixelFormat>
    length<std::int32_t>
    image<TPixelFormat>::width() const
    {
        return _width;
    }

    template<typename TPixelFormat>
    length<std::int32_t>
    image<TPixelFormat>::height() const
    {
        return _height;
    }

    template<typename TPixelFormat>
    TPixelFormat&
    image<TPixelFormat>::operator()(offset<std::int32_t> x, offset<std::int32_t> y)
    {
        return _pixels[
            boost::units::quantity_cast<std::size_t>(y * _pitch + x)
        ];
    }

    template<typename TPixelFormat>
    TPixelFormat const&
    image<TPixelFormat>::operator()(offset<std::int32_t> x, offset<std::int32_t> y) const
    {
        return _pixels[
            boost::units::quantity_cast<std::size_t>(y * _pitch + x)
        ];
    }
}
