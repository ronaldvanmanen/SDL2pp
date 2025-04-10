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

#include "SDL2pp/argb8888.h"
#include "SDL2pp/color.h"
#include "SDL2pp/event_queue.h"
#include "SDL2pp/event.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/texture.h"
#include "SDL2pp/window.h"

int main()
{
    sdl2::window window("Plasma Fractal", 640, 480, sdl2::window_flags::shown | sdl2::window_flags::resizable);
    sdl2::renderer renderer(window, sdl2::renderer_flags::accelerated | sdl2::renderer_flags::present_vsync);
    sdl2::texture<sdl2::argb8888> texture(renderer, sdl2::texture_access::streaming_access, renderer.output_size());
    sdl2::event_queue event_queue;

    auto running = true;
    while (running)
    {
        auto event = event_queue.poll_event();
        if (event)
        {
            switch (event->type())
            {
                case sdl2::event_type::quit:
                    running = false;
                    break;
            }
        }
        else
        {
            renderer.set_draw_blend_mode(sdl2::blend_mode::none);
            renderer.set_draw_color(sdl2::color::white);
            renderer.clear();
            renderer.copy(texture);
            renderer.present();
        }

    }

    return 0;
}
