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

#include <boost/operators.hpp>

#include "a8.h"
#include "b8.h"
#include "g8.h"
#include "pixel_format.h"
#include "r8.h"

namespace sdl2
{
    class alignas(alignof(std::uint32_t)) argb8888
    : public boost::equality_comparable<argb8888>
    {
    public:
        static constexpr pixel_format format = pixel_format::argb8888;

        static const argb8888 black;

        static const argb8888 white;

    public:
        argb8888();

        argb8888(sdl2::a8 a, sdl2::r8 r, sdl2::g8 g, sdl2::b8 b);

        argb8888(argb8888 const& other);

        argb8888& operator=(argb8888 const& other);

        bool operator==(argb8888 const& other) const;

    public:
        sdl2::b8 b;
        sdl2::g8 g;
        sdl2::r8 r;
        sdl2::a8 a;
    };

    std::ostream& operator<<(std::ostream& stream, argb8888 const& value);
}
