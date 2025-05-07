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

#include "SDL2pp/palette.h"

BOOST_AUTO_TEST_CASE(test_palette_size_constructor)
{
    sdl2::palette test_palette(256);

    BOOST_TEST(256 == test_palette.size());
}

BOOST_AUTO_TEST_CASE(test_palette_initializer_list_constructor)
{
    sdl2::palette test_palette { sdl2::color::white, sdl2::color::black };

    BOOST_TEST(2 == test_palette.size());
    BOOST_TEST(test_palette[0] == sdl2::color::white);
    BOOST_TEST(test_palette[1] == sdl2::color::black);
}

BOOST_AUTO_TEST_CASE(test_palette_subscript_operator)
{
    sdl2::palette test_palette(2);

    test_palette[0] = sdl2::color::white;
    test_palette[1] = sdl2::color::black;

    BOOST_REQUIRE_THROW(test_palette[2], std::out_of_range);
    
    BOOST_TEST(test_palette[0] == sdl2::color::white);
    BOOST_TEST(test_palette[1] == sdl2::color::black);
}
