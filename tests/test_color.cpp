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

#include <boost/test/unit_test.hpp>

#include <SDL2/SDL_pixels.h>

#include "SDL2pp/color.h"

BOOST_AUTO_TEST_CASE(test_color_default_constructor)
{
    sdl2::color test_color;

    BOOST_TEST(test_color.r == 0);
    BOOST_TEST(test_color.g == 0);
    BOOST_TEST(test_color.b == 0);
    BOOST_TEST(test_color.a == 0);
}

BOOST_AUTO_TEST_CASE(test_color_component_constructor)
{
    sdl2::color test_color(0xDE, 0xAD, 0xBE, 0xEF);

    BOOST_TEST(test_color.r == 0xDE);
    BOOST_TEST(test_color.g == 0xAD);
    BOOST_TEST(test_color.b == 0xBE);
    BOOST_TEST(test_color.a == 0xEF);
}

BOOST_AUTO_TEST_CASE(test_color_compatibility)
{
    sdl2::color test_color(0xDE, 0xAD, 0xBE, 0xEF);

    SDL_Color* ri_test_color = reinterpret_cast<SDL_Color*>(&test_color);

    BOOST_TEST(ri_test_color->r == 0xDE);
    BOOST_TEST(ri_test_color->g == 0xAD);
    BOOST_TEST(ri_test_color->b == 0xBE);
    BOOST_TEST(ri_test_color->a == 0xEF);
}