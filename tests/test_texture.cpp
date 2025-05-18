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

#include "SDL2pp/argb8888.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/texture.h"
#include "SDL2pp/window.h"

using namespace sdl2;

BOOST_AUTO_TEST_CASE(test_texture_constructors)
{
    window test_window(
        "test_renderer_constructors",
        640*px,
        480*px,
        window_flags::hidden
    );

    renderer test_renderer(test_window);

    BOOST_REQUIRE_NO_THROW(
        texture<argb8888> test_texture(
            test_renderer,
            texture_access::streaming_access,
            test_renderer.output_size()
        )
    );
}
