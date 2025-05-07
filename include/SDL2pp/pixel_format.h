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

#include <SDL2/SDL_pixels.h>

namespace sdl2
{
    enum class pixel_format : std::uint32_t
    {
        unknown = SDL_PIXELFORMAT_UNKNOWN,
        index1lsb = SDL_PIXELFORMAT_INDEX1LSB,
        index1msb = SDL_PIXELFORMAT_INDEX1MSB,
        index4lsb = SDL_PIXELFORMAT_INDEX4LSB,
        index4msb = SDL_PIXELFORMAT_INDEX4MSB,
        index8 = SDL_PIXELFORMAT_INDEX8,
        rgb332 = SDL_PIXELFORMAT_RGB332,
        xrgb4444 = SDL_PIXELFORMAT_XRGB4444,
        rgb444 = SDL_PIXELFORMAT_RGB444,
        xbgr4444 = SDL_PIXELFORMAT_XBGR4444,
        bgr444 = SDL_PIXELFORMAT_BGR444,
        xrgb1555 = SDL_PIXELFORMAT_XRGB1555,
        rgb555 = SDL_PIXELFORMAT_RGB555,
        xbgr1555 = SDL_PIXELFORMAT_XBGR1555,
        bgr555 = SDL_PIXELFORMAT_BGR555,
        argb4444 = SDL_PIXELFORMAT_ARGB4444,
        rgba4444 = SDL_PIXELFORMAT_RGBA4444,
        abgr4444 = SDL_PIXELFORMAT_ABGR4444,
        bgra4444 = SDL_PIXELFORMAT_BGRA4444,
        argb1555 = SDL_PIXELFORMAT_ARGB1555,
        rgba5551 = SDL_PIXELFORMAT_RGBA5551,
        abgr1555 = SDL_PIXELFORMAT_ABGR1555,
        bgra5551 = SDL_PIXELFORMAT_BGRA5551,
        rgb565 = SDL_PIXELFORMAT_RGB565,
        bgr565 = SDL_PIXELFORMAT_BGR565,
        rgb24 = SDL_PIXELFORMAT_RGB24,
        bgr24 = SDL_PIXELFORMAT_BGR24,
        xrgb8888 = SDL_PIXELFORMAT_XRGB8888,
        rgb888 = SDL_PIXELFORMAT_RGB888,
        rgbx8888 = SDL_PIXELFORMAT_RGBX8888,
        xbgr8888 = SDL_PIXELFORMAT_XBGR8888,
        bgr888 = SDL_PIXELFORMAT_BGR888,
        bgrx8888 = SDL_PIXELFORMAT_BGRX8888,
        argb8888 = SDL_PIXELFORMAT_ARGB8888,
        rgba8888 = SDL_PIXELFORMAT_RGBA8888,
        abgr8888 = SDL_PIXELFORMAT_ABGR8888,
        bgra8888 = SDL_PIXELFORMAT_BGRA8888,
        argb2101010 = SDL_PIXELFORMAT_ARGB2101010,
        rgba32 = SDL_PIXELFORMAT_RGBA32,
        argb32 = SDL_PIXELFORMAT_ARGB32,
        bgra32 = SDL_PIXELFORMAT_BGRA32,
        abgr32 = SDL_PIXELFORMAT_ABGR32,
        yv12 = SDL_PIXELFORMAT_YV12,
        iyuv = SDL_PIXELFORMAT_IYUV,
        yuy2 = SDL_PIXELFORMAT_YUY2,
        uyvy = SDL_PIXELFORMAT_UYVY,
        yvyu = SDL_PIXELFORMAT_YVYU,
        nv12 = SDL_PIXELFORMAT_NV12,
        nv21 = SDL_PIXELFORMAT_NV21,
        external_oes = SDL_PIXELFORMAT_EXTERNAL_OES
    };

    std::ostream& operator<<(std::ostream& stream, pixel_format const& value);
}
