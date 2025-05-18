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

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <random>
#include <vector>

#include <boost/units/cmath.hpp>

#include "SDL2pp/argb8888.h"
#include "SDL2pp/color.h"
#include "SDL2pp/event_queue.h"
#include "SDL2pp/event.h"
#include "SDL2pp/image.h"
#include "SDL2pp/index8.h"
#include "SDL2pp/keyboard_event.h"
#include "SDL2pp/palette.h"
#include "SDL2pp/pixel_format_details.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/size.h"
#include "SDL2pp/surface.h"
#include "SDL2pp/window.h"

#include "shared/math.h"
#include "shared/stopwatch.h"

using namespace std;
using namespace sdl2;

void
diamond_step(
    image<index8> &map,
    default_random_engine &random_number_engine,
    offset<int32_t> center_x,
    offset<int32_t> center_y,
    length<int32_t> distance,
    int randomness
)
{
    auto random_number_distribution = uniform_int_distribution<int>(-randomness, randomness);
    auto sum = 0;
    auto count = 0;
    auto top = center_y - distance;
    if (top >= 0*px && top < map.height())
    {
        auto left = center_x - distance;
        if (left >= 0*px && left < map.width())
        {
            sum += map(left, top);
            ++count;
        }

        auto right = center_x + distance;
        if (right >= 0*px && right < map.width())
        {
            sum += map(right, top);
            ++count;
        }
    }

    auto bottom = center_y + distance;
    if (bottom >= 0*px && bottom < map.height())
    {
        auto left = center_x - distance;
        if (left >= 0*px && left < map.width())
        {
            sum += map(left, bottom);
            ++count;
        }

        auto right = center_x + distance;
        if (right >= 0*px && right < map.width())
        {
            sum += map(right, bottom);
            ++count;
        }
    }

    int average = sum / count;
    int random = random_number_distribution(random_number_engine);
    int value = clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<index8>(value);
}

void
square_step(
    image<index8> &map,
    default_random_engine &random_number_engine,
    offset<int32_t> center_x,
    offset<int32_t> center_y,
    length<int32_t> distance,
    int randomness
)
{
    auto random_number_distribution = uniform_int_distribution<int>(-randomness, randomness);
    auto sum = 0;
    auto count = 0;
    auto top = center_y - distance;
    if (top >= 0*px && top < map.height())
    {
        sum += map(center_x, top);
        ++count;
    }

    auto left = center_x - distance;
    if (left >= 0*px && left < map.width())
    {
        sum += map(left, center_y);
        ++count;
    }

    auto bottom = center_y + distance;
    if (bottom >= 0*px && bottom < map.height())
    {
        sum += map(center_x, bottom);
        ++count;
    }

    auto right = center_x + distance;
    if (right >= 0*px && right < map.width())
    {
        sum += map(right, center_y);
        ++count;
    }

    int average = sum / count;
    int random = random_number_distribution(random_number_engine);
    int value = clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<index8>(value);
}

surface<index8>
generate_diamond_square_image(default_random_engine &random_number_engine, length<int32_t> square_size)
{
    auto actual_size = size_2d<int32_t>(1*px + next_power_of_two(square_size));
    auto actual_surface = surface<index8>(actual_size);
    actual_surface.with_lock([&random_number_engine](image<index8> & image)
    {
        const auto image_width = image.width();
        const auto image_height = image.height();
        const auto initial_randomness = 256;
        
        auto random_number_distribution = uniform_int_distribution<int>(0, initial_randomness - 1);

        image(0*px,                 0*px)                   = index8(random_number_distribution(random_number_engine));
        image(image_width - 1*px,   0*px)                   = index8(random_number_distribution(random_number_engine));
        image(0*px,                 image_height - 1*px)    = index8(random_number_distribution(random_number_engine));
        image(image_width - 1*px,   image_height - 1*px)    = index8(random_number_distribution(random_number_engine));

        auto randomness = initial_randomness / 2;

        for (auto step_size = image_height - 1*px; step_size > 1*px; step_size /= 2)
        {
            auto half_step_size = step_size / 2;

            for (auto y = half_step_size; y < image_height; y += step_size)
            {
                for (auto x = half_step_size; x < image_width; x += step_size)
                {
                    diamond_step(image, random_number_engine, x, y, half_step_size, randomness);
                }
            }

            for (auto y = 0*px; y < image_height; y += half_step_size)
            {
                for (auto x = boost::units::fmod(y, step_size) == 0*px ? half_step_size : 0*px; x < image_width; x += step_size)
                {
                    square_step(image, random_number_engine, x, y, half_step_size, randomness);
                }
            }

            randomness /= 2;
        }
    });

    return actual_surface;
}

surface<index8>
generate_diamond_square_image(default_random_engine &random_number_engine, length<int32_t> width, length<int32_t> height)
{
    return generate_diamond_square_image(random_number_engine, max(width, height));
}

surface<index8>
generate_diamond_square_image(default_random_engine &random_number_engine, size_2d<int32_t> size)
{
    return generate_diamond_square_image(random_number_engine, size.width, size.height);
}

void rotate_left(vector<color> & palette)
{
    rotate(palette.begin(), palette.begin() + 1, palette.end());
}

void rotate_right(vector<color> & palette)
{
    rotate(palette.begin(), palette.end() - 1, palette.end());
}

int main()
{
    auto main_window = window("Plasma Fractal", 640*px, 480*px, window_flags::shown);
    auto main_event_queue = event_queue();

    auto random_number_engine = default_random_engine(0);
    auto plasma_surface = generate_diamond_square_image(random_number_engine, main_window.size());
    auto plasma_surface_format = pixel_format_details(plasma_surface);
    auto plasma_palette = palette(plasma_surface_format);
    auto plasma_colors = vector<color>(256);

    for (uint8_t i = 0; i < 32; ++i)
    {
        uint8_t lo = i * 255 / 31;
        uint8_t hi = 255 - lo;
        plasma_colors[i] = color(lo, 0, 0, 255);
        plasma_colors[i + 32] = color(hi, 0, 0, 255);
        plasma_colors[i + 64] = color(0, lo, 0, 255);
        plasma_colors[i + 96] = color(0, hi, 0, 255);
        plasma_colors[i + 128] = color(0, 0, lo, 255);
        plasma_colors[i + 160] = color(0, 0, hi, 255);
        plasma_colors[i + 192] = color(lo, 0, lo, 255);
        plasma_colors[i + 224] = color(hi, 0, hi, 255);
    }

    plasma_palette = plasma_colors;

    auto stopwatch = stopwatch::start_now();
    auto reverse_rotation = false;
    auto running = true;
    while (running)
    {
        event polled_event;
        if (main_event_queue.poll(polled_event))
        {
            switch (polled_event.type())
            {
                case event_type::quit:
                    running = false;
                    break;

                case event_type::key_up:
                    auto key_event = polled_event.as<keyboard_event>();
                    switch (key_event.scan_code())
                    {
                        case scan_code::r:
                            reverse_rotation = !reverse_rotation;
                            break;
                    }
                    break;
            }
        }
        else
        {
            auto main_surface = surface<argb8888>(main_window);
            main_surface.blit(plasma_surface);
            main_window.update_surface();

            static const auto refresh_rate = fractional_seconds(1.0 / 60.0);
            const auto elapsed = elapsed_seconds(stopwatch);
            if (elapsed < refresh_rate)
            {
                continue;
            }

            stopwatch.reset();

            if (reverse_rotation)
            {
                rotate_right(plasma_colors);
            }
            else
            {
                rotate_left(plasma_colors);
            }

            plasma_palette = plasma_colors;
        }
    }

    return 0;
}
