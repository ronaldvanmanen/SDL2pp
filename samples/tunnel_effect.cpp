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

#include <boost/units/cmath.hpp>

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

using namespace std;
using namespace sdl2;

struct displacement
{
    displacement()
    : displacement(0, 0)
    {}

    displacement(length<int32_t> x, length<int32_t> y)
    : x(x), y(y)
    { }

    length<int32_t> x;

    length<int32_t> y;
};

length<double> calc_distance(length<double> width, length<double> height, offset<double> x, offset<double> y)
{
    // distance = int(ratio * texHeight / sqrt((x - w / 2.0) * (x - w / 2.0) + (y - h / 2.0) * (y - h / 2.0))) % texHeight;
    const double ratio = 32;
    return length<double>::from_value(
        fmod(
            ratio * height / boost::units::sqrt(
                boost::units::pow<2>(x - width / 2.0) +
                boost::units::pow<2>(y - height / 2.0)
            ),
            boost::units::quantity_cast<double>(height)
        )
    );
}

length<double> calc_angle(length<double> width, length<double> height, offset<double> x, offset<double> y)
{
    // angle = (unsigned int)(0.5 * texWidth * atan2(y - h / 2.0, x - w / 2.0) / 3.1416);
    return length<double>::from_value(
        boost::units::quantity_cast<double>(
            0.5 * width * boost::units::atan2(y - height / 2.0, x - width / 2.0) / numbers::pi
        )
    );
}

image<displacement> generate_displacement_image(length<int32_t> square_size)
{
    length<int32_t> actual_size = next_power_of_two(square_size);

    image<displacement> displacement_image(actual_size, actual_size);

    for (offset<int32_t> y = 0; y < actual_size; y += 1.0*px)
    {
        for (offset<int32_t> x = 0; x < actual_size; x += 1.0*px)
        {
            auto displace_x = length<int32_t>(calc_distance(actual_size, actual_size, x, y));
            auto displace_y = length<int32_t>(calc_angle(actual_size, actual_size, x, y));
            displacement_image(x, y) = displacement(displace_x, displace_y);
        }
    }

    return displacement_image;
}

image<displacement> generate_displacement_image(length<int32_t> width, length<int32_t> height)
{
    return generate_displacement_image(
        max(width, height)
    );
}

image<displacement> generate_displacement_image(size_2d<int32_t> size)
{
    return generate_displacement_image(size.width, size.height);
}

image<argb8888> generate_xor_image(length<int32_t> square_size)
{
    length<int32_t> actual_size = next_power_of_two(square_size);

    image<argb8888> xor_image(actual_size, actual_size);

    for (offset<int32_t> y = 0; y < actual_size; y += 1*px)
    {
        for (offset<int32_t> x = 0; x < actual_size; x += 1*px)
        {
            xor_image(x, y) = argb8888(
                a8(0xFF),
                r8(0x00),
                g8(0x00),
                b8((x * 256 / actual_size) ^ (y * 256 / actual_size))
            );
        }
    }

    return xor_image;
}

image<argb8888> generate_xor_image(length<int32_t> width, length<int32_t> height)
{
    return generate_xor_image(max(width, height));
}

image<argb8888> generate_xor_image(size_2d<int32_t> const& size)
{
    return generate_xor_image(size.width, size.height);
}

length<int32_t> power_of_two_mod(length<int32_t> x, length<int32_t> y)
{
    const auto x_value = boost::units::quantity_cast<int32_t>(x);
    const auto y_value = boost::units::quantity_cast<int32_t>(y);
    return length<int32_t>::from_value(x_value & (y_value - 1));
}

int main()
{
    auto main_window = window("Tunnel Effect", 800*px, 600*px, window_flags::shown | window_flags::resizable);
    auto screen_renderer = renderer(main_window, renderer_flags::accelerated | renderer_flags::present_vsync);
    auto screen_texture = texture<argb8888>(screen_renderer, texture_access::streaming_access, screen_renderer.output_size());
    auto main_event_queue = event_queue();

    auto source_image = generate_xor_image(screen_renderer.output_size());
    auto displacement_table = generate_displacement_image(screen_renderer.output_size());
    
    auto stopwatch = stopwatch::start_now();
    auto running = true;
    while (running)
    {
        event poll_event;
        if (main_event_queue.poll(poll_event))
        {
            switch (poll_event.type())
            {
                case event_type::quit:
                    running = false;
                    break;
            }
        }
        else
        {
            screen_texture.with_lock(
                [&source_image, &displacement_table, &stopwatch](image<argb8888> &screen_image)
                {
                    const auto screen_width = screen_image.width();
                    const auto screen_height = screen_image.height();
                    const auto source_width = source_image.width();
                    const auto source_height = source_image.height();
 
                    const auto time = elapsed_time(stopwatch);
                    const auto speed_x = length<double>(source_width) / second;
                    const auto speed_y = length<double>(source_height) * 0.25 / second;
                    const auto shift_x = length<int32_t>(speed_x * time);
                    const auto shift_y = length<int32_t>(speed_y * time);
                    const auto offset_x = (source_width - screen_width) / 2;
                    const auto offset_y = (source_height - screen_height) / 2;
    
                    for (auto screen_y = 0*px; screen_y < screen_height; screen_y += 1*px)
                    {
                        for (auto screen_x = 0*px; screen_x < screen_width; screen_x += 1*px)
                        {
                            const auto displacement = displacement_table(screen_x + offset_x, screen_y + offset_y);
                            const auto source_x = power_of_two_mod(displacement.x + shift_x, source_width);
                            const auto source_y = power_of_two_mod(displacement.y + shift_y, source_height);
                            screen_image(screen_x, screen_y) = source_image(source_x, source_y);
                        }
                    }
                }
            );

            screen_renderer.draw_blend_mode(blend_mode::none);
            screen_renderer.draw_color(color::black);
            screen_renderer.clear();
            screen_renderer.copy(screen_texture);
            screen_renderer.present();
        }
    }

    return 0;
}
