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

#include "SDL2pp/pixel_format.h"

std::ostream&
sdl2::operator<<(std::ostream& stream, sdl2::pixel_format const& value)
{
    #define RETURN(X) return stream << #X;
    #define CASE(X) case X: RETURN(X)
    #define DEFAULT(X) default: RETURN(X);

    switch (value)
    {
        CASE(sdl2::pixel_format::unknown)
        CASE(sdl2::pixel_format::index1lsb)
        CASE(sdl2::pixel_format::index1msb)
        CASE(sdl2::pixel_format::index4lsb)
        CASE(sdl2::pixel_format::index4msb)
        CASE(sdl2::pixel_format::index8)
        CASE(sdl2::pixel_format::rgb332)
        CASE(sdl2::pixel_format::xrgb4444)
        // CASE(sdl2::pixel_format::rgb444)
        CASE(sdl2::pixel_format::xbgr4444)
        // CASE(sdl2::pixel_format::bgr444)
        CASE(sdl2::pixel_format::xrgb1555)
        // CASE(sdl2::pixel_format::rgb555)
        CASE(sdl2::pixel_format::xbgr1555)
        // CASE(sdl2::pixel_format::bgr555)
        CASE(sdl2::pixel_format::argb4444)
        CASE(sdl2::pixel_format::rgba4444)
        CASE(sdl2::pixel_format::abgr4444)
        CASE(sdl2::pixel_format::bgra4444)
        CASE(sdl2::pixel_format::argb1555)
        CASE(sdl2::pixel_format::rgba5551)
        CASE(sdl2::pixel_format::abgr1555)
        CASE(sdl2::pixel_format::bgra5551)
        CASE(sdl2::pixel_format::rgb565)
        CASE(sdl2::pixel_format::bgr565)
        CASE(sdl2::pixel_format::rgb24)
        CASE(sdl2::pixel_format::bgr24)
        CASE(sdl2::pixel_format::xrgb8888)
        // CASE(sdl2::pixel_format::rgb888)
        CASE(sdl2::pixel_format::rgbx8888)
        CASE(sdl2::pixel_format::xbgr8888)
        // CASE(sdl2::pixel_format::bgr888)
        CASE(sdl2::pixel_format::bgrx8888)
        CASE(sdl2::pixel_format::argb8888)
        CASE(sdl2::pixel_format::rgba8888)
        CASE(sdl2::pixel_format::abgr8888)
        CASE(sdl2::pixel_format::bgra8888)
        CASE(sdl2::pixel_format::argb2101010)
        // CASE(sdl2::pixel_format::rgba32)
        // CASE(sdl2::pixel_format::argb32)
        // CASE(sdl2::pixel_format::bgra32)
        // CASE(sdl2::pixel_format::abgr32)
        CASE(sdl2::pixel_format::yv12)
        CASE(sdl2::pixel_format::iyuv)
        CASE(sdl2::pixel_format::yuy2)
        CASE(sdl2::pixel_format::uyvy)
        CASE(sdl2::pixel_format::yvyu)
        CASE(sdl2::pixel_format::nv12)
        CASE(sdl2::pixel_format::nv21)
        CASE(sdl2::pixel_format::external_oes)
        DEFAULT(sdl2::pixel_format::unknown)
    }
}
