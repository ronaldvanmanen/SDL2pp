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

#include <boost/operators.hpp>

#include "length.h"

namespace sdl2
{
    template<typename Y>
    struct size_2d : boost::equality_comparable<size_2d<Y>>
    {
    public:
        explicit size_2d(length<Y> width_and_height);

        size_2d(length<Y> width, length<Y> height);

        bool operator==(size_2d<Y> const& other) const;

        length<Y> width;
        
        length<Y> height;
    };

    template<typename Y>
    size_2d<Y>::size_2d(length<Y> width_and_height)
    : width(width_and_height)
    , height(width_and_height)
    { }

    template<typename Y>
    size_2d<Y>::size_2d(length<Y> width, length<Y> height)
    : width(width)
    , height(height)
    { }

    template<typename Y>
    bool
    size_2d<Y>::operator==(size_2d<Y> const& other) const
    {
        return width == other.width && height == other.height;
    }
}
