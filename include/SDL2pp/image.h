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

#include <stdint.h>

#include "pixel_format.h"
#include "pixel_format_traits.h"

namespace sdl2
{
    template<typename TPixelFormat>
    class image
    {
    public:
        static constexpr pixel_format format = pixel_format_traits<TPixelFormat>::format;

    public:
        image(int width, int height);

        image(TPixelFormat* pixels, int width, int height, int pitch);

        int width() const;

        int height() const;

        TPixelFormat& operator()(int x, int y);

        TPixelFormat const& operator()(int x, int y) const;

    private:
        TPixelFormat* _pixels;

        int _pitch;

        int _width;

        int _height;
    };

    template<typename TPixelFormat>
    image<TPixelFormat>::image(int width, int height)
    : image(new TPixelFormat[height * width], width, height, width)
    {}

    template<typename TPixelFormat>
    image<TPixelFormat>::image(TPixelFormat* pixels, int width, int height, int pitch)
    : _pixels(pixels)
    , _width(width)
    , _height(height)
    , _pitch(pitch)
    { }


    template<typename TPixelFormat>
    int
    image<TPixelFormat>::width() const
    {
        return _width;
    }

    template<typename TPixelFormat>
    int
    image<TPixelFormat>::height() const
    {
        return _height;
    }

    template<typename TPixelFormat>
    TPixelFormat&
    image<TPixelFormat>::operator()(int x, int y)
    {
        return _pixels[y * _pitch + x];
    }

    template<typename TPixelFormat>
    TPixelFormat const&
    image<TPixelFormat>::operator()(int x, int y) const
    {
        return _pixels[y * _pitch + x];
    }
}
