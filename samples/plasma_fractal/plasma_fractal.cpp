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

#include <memory>

#include "SDL2pp/sdl.h"
#include "SDL2pp/argb8888.h"

using namespace sdl2;

int main()
{
    auto events_subsystem = sdl.init_events();

    auto video_subsystem = sdl.init_video();
    auto window = video_subsystem.create_window("Plasma Fractal", 640, 480, window_flags::shown | window_flags::resizable);
    auto renderer = window.create_renderer(renderer_flags::accelerated | renderer_flags::present_vsync);
    auto renderer_output_size = renderer.output_size();
    auto texture = renderer.create_texture<argb8888>(texture_access::streaming_access, renderer_output_size.width, renderer_output_size.height);

    auto running = true;
    while (running)
    {
        auto event = events_subsystem.poll_event();
        if (event)
        {
            switch (event->type())
            {
                case event_type::quit:
                    running = false;
                    break;
            }
        }
    }

    return 0;
}
