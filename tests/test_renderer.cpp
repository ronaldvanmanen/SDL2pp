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

#include "SDL2pp/renderer.h"

BOOST_AUTO_TEST_CASE(test_renderer_constructors)
{
    sdl2::window test_window("test_renderer_constructors", 640, 480, sdl2::window_flags::hidden);

    BOOST_REQUIRE_NO_THROW(
        sdl2::renderer test_renderer(test_window);
    );

    BOOST_REQUIRE_NO_THROW(
        sdl2::renderer test_renderer(test_window, sdl2::renderer_flags::accelerated | sdl2::renderer_flags::present_vsync)
    );
}

BOOST_AUTO_TEST_CASE(test_renderer_output_size)
{
    sdl2::window test_window("test_renderer_constructors", 640, 480, sdl2::window_flags::hidden);
    sdl2::renderer test_renderer(test_window);
    sdl2::size test_renderer_output_size = test_renderer.output_size();
    BOOST_TEST(test_renderer_output_size.width == 640);
    BOOST_TEST(test_renderer_output_size.height == 480);
}

BOOST_AUTO_TEST_CASE(test_renderer_set_draw_color)
{
    sdl2::window test_window("test_renderer_constructors", 640, 480, sdl2::window_flags::hidden);
    sdl2::renderer test_renderer(test_window);
    sdl2::color test_color(0xDE, 0xAD, 0xBE, 0xEF);
    
    test_renderer.set_draw_color(test_color);

    BOOST_TEST(test_color == test_renderer.get_draw_color());
}
