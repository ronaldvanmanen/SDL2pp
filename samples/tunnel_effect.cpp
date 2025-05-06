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

#include <chrono>
#include <cmath>
#include <cstdint>
#include <numbers>

#include "SDL2pp/argb8888.h"
#include "SDL2pp/color.h"
#include "SDL2pp/event_queue.h"
#include "SDL2pp/event.h"
#include "SDL2pp/image.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/texture.h"
#include "SDL2pp/window.h"

#include "shared/math.h"
#include "shared/stopwatch.h"

namespace sdl2
{
    struct transform
    {
        transform();

        transform(std::int32_t angle, std::int32_t distance);

        std::int32_t angle;

        std::int32_t distance;
    };
}

sdl2::transform::transform()
: sdl2::transform(0, 0)
{}

sdl2::transform::transform(std::int32_t angle, std::int32_t distance)
: angle(angle), distance(distance)
{ }

sdl2::image<sdl2::transform> generate_transform_image(int square_size)
{
    const double ratio = 32;

    std::int32_t actual_size = sdl2::next_power_of_two(square_size);

    sdl2::image<sdl2::transform> transform_image(actual_size, actual_size);

    for (std::int32_t y = 0; y < actual_size; y++)
    {
        for (std::int32_t x = 0; x < actual_size; x++)
        {
            auto angle = static_cast<std::int32_t>(0.5 * actual_size * std::atan2(y - actual_size / 2.0, x - actual_size / 2.0) / std::numbers::pi);
            auto distance = static_cast<std::int32_t>(ratio * actual_size / std::sqrt((x - actual_size / 2.0) * (x - actual_size / 2.0) + (y - actual_size / 2.0) * (y - actual_size / 2.0))) % actual_size;
            transform_image(x, y) = sdl2::transform(angle, distance);
        }
    }

    return transform_image;
}

sdl2::image<sdl2::transform> generate_transform_image(std::int32_t width, std::int32_t height)
{
    return generate_transform_image(std::max(width, height));
}

sdl2::image<sdl2::transform> generate_transform_image(sdl2::size size)
{
    return generate_transform_image(size.width, size.height);
}

sdl2::image<sdl2::argb8888> generate_xor_image(std::int32_t square_size)
{
    std::int32_t actual_size = sdl2::next_power_of_two(square_size);

    sdl2::image<sdl2::argb8888> xor_image(actual_size, actual_size);

    for (std::int32_t y = 0; y < actual_size; y++)
    {
        for (std::int32_t x = 0; x < actual_size; x++)
        {
            xor_image(x, y) = sdl2::argb8888(
                0xFF,
                0x00,
                0x00,
                (x * 256 / actual_size) ^ (y * 256 / actual_size)
            );
        }
    }

    return xor_image;
}

sdl2::image<sdl2::argb8888> generate_xor_image(std::int32_t width, std::int32_t height)
{
    return generate_xor_image(std::max(width, height));
}

sdl2::image<sdl2::argb8888> generate_xor_image(sdl2::size const& size)
{
    return generate_xor_image(size.width, size.height);
}

int main()
{
    auto window = sdl2::window("Tunnel Effect", 640, 480, sdl2::window_flags::shown | sdl2::window_flags::resizable);
    auto renderer = sdl2::renderer(window, sdl2::renderer_flags::accelerated | sdl2::renderer_flags::present_vsync);
    auto texture = sdl2::texture<sdl2::argb8888>(renderer, sdl2::texture_access::streaming_access, renderer.output_size());
    auto event_queue = sdl2::event_queue();

    auto source_image = generate_xor_image(renderer.output_size());
    auto transform_table = generate_transform_image(renderer.output_size());
    
    auto stopwatch = sdl2::stopwatch::start_now();
    auto running = true;
    while (running)
    {
        sdl2::event event;
        if (event_queue.poll(event))
        {
            switch (event.type())
            {
                case sdl2::event_type::quit:
                    running = false;
                    break;
            }
        }
        else
        {
            texture.with_lock(
                [&source_image, &transform_table, &stopwatch](sdl2::image<sdl2::argb8888> &screen_image)
                {
                    const auto screen_width = screen_image.width();
                    const auto screen_height = screen_image.height();
                    const auto source_width = source_image.width();
                    const auto source_height = source_image.height();
 
                    const auto elapsed_time = stopwatch.elapsed_seconds();
                    const auto shift_x = static_cast<std::int32_t>(screen_width * elapsed_time);
                    const auto shift_y = static_cast<std::int32_t>(screen_height * elapsed_time / 4);
                    const auto look_x = (source_width - screen_width) / 2;
                    const auto look_y = (source_height - screen_height) / 2;
                    const auto shift_look_x = shift_x + look_x;
                    const auto shift_look_y = shift_y + look_y;
    
                    for (auto y = 0; y < screen_image.height(); ++y)
                    {
                        for (auto x = 0; x < screen_image.width(); ++x)
                        {
                            const auto transform = transform_table(x + look_x, y + look_y);
                            const auto source_x = (transform.distance + shift_look_x) & (source_width - 1);
                            const auto source_y = (transform.angle + shift_look_y) & (source_height - 1);
    
                            screen_image(x, y) = source_image(source_x, source_y);
                        }
                    }
                }
            );

            renderer.draw_blend_mode(sdl2::blend_mode::none);
            renderer.draw_color(sdl2::color::black);
            renderer.clear();
            renderer.copy(texture);
            renderer.present();
        }
    }

    return 0;
}
