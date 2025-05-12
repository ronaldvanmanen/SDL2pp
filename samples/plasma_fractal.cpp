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

using sdl2::operator""_px;

void
diamond_step(sdl2::image<sdl2::index8> &map, std::default_random_engine &random_number_engine, int center_x, int center_y, int distance, int randomness)
{
    std::uniform_int_distribution<int> random_number_distribution(-randomness, randomness);

    int sum = 0;
    int count = 0;
    int top = center_y - distance;
    if (top >= 0 && top < map.height())
    {
        int left = center_x - distance;
        if (left >= 0 && left < map.width())
        {
            sum += map(left, top);
            ++count;
        }

        int right = center_x + distance;
        if (right >= 0 && right < map.width())
        {
            sum += map(right, top);
            ++count;
        }
    }

    int bottom = center_y + distance;
    if (bottom >= 0 && bottom < map.height())
    {
        int left = center_x - distance;
        if (left >= 0 && left < map.width())
        {
            sum += map(left, bottom);
            ++count;
        }

        int right = center_x + distance;
        if (right >= 0 && right < map.width())
        {
            sum += map(right, bottom);
            ++count;
        }
    }

    int average = sum / count;
    int random = random_number_distribution(random_number_engine);
    int value = std::clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<uint8_t>(value);
}

void
square_step(sdl2::image<sdl2::index8> &map, std::default_random_engine &random_number_engine, int center_x, int center_y, int distance, int randomness)
{
    std::uniform_int_distribution<int> random_number_distribution(-randomness, randomness);

    int sum = 0;
    int count = 0;
    int top = center_y - distance;
    if (top >= 0 && top < map.height())
    {
        sum += map(center_x, top);
        ++count;
    }

    int left = center_x - distance;
    if (left >= 0 && left < map.width())
    {
        sum += map(left, center_y);
        ++count;
    }

    int bottom = center_y + distance;
    if (bottom >= 0 && bottom < map.height())
    {
        sum += map(center_x, bottom);
        ++count;
    }

    int right = center_x + distance;
    if (right >= 0 && right < map.width())
    {
        sum += map(right, center_y);
        ++count;
    }

    int average = sum / count;
    int random = random_number_distribution(random_number_engine);
    int value = std::clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<uint8_t>(value);
}

sdl2::surface<sdl2::index8>
generate_diamond_square_image(std::default_random_engine &random_number_engine, int square_size)
{
    auto actual_size = sdl2::size(sdl2::next_power_of_two(square_size) + 1);
    auto surface = sdl2::surface<sdl2::index8>(actual_size);
    surface.with_lock([&random_number_engine](sdl2::image<sdl2::index8> & image)
    {
        const auto image_width = image.width();
        const auto image_height = image.height();
        int randomness = 256;
        
        std::uniform_int_distribution<int> random_number_distribution(0, randomness - 1);

        image(0, 0) = static_cast<uint8_t>(random_number_distribution(random_number_engine));
        image(image_width - 1, 0) = static_cast<uint8_t>(random_number_distribution(random_number_engine));
        image(0, image_height - 1) = static_cast<uint8_t>(random_number_distribution(random_number_engine));
        image(image_width - 1, image_height - 1) = static_cast<uint8_t>(random_number_distribution(random_number_engine));

        randomness /= 2;

        for (int step_size = image_height - 1; step_size > 1; step_size /= 2)
        {
            int half_step_size = step_size / 2;

            for (int y = half_step_size; y < image_height; y += step_size)
            {
                for (int x = half_step_size; x < image_width; x += step_size)
                {
                    diamond_step(image, random_number_engine, x, y, half_step_size, randomness);
                }
            }

            for (int y = 0; y < image_height; y += half_step_size)
            {
                for (int x = (y % step_size) == 0 ? half_step_size : 0; x < image_width; x += step_size)
                {
                    square_step(image, random_number_engine, x, y, half_step_size, randomness);
                }
            }

            randomness /= 2;
        }
    });

    return surface;
}

sdl2::surface<sdl2::index8>
generate_diamond_square_image(std::default_random_engine &random_number_engine, int width, int height)
{
    return generate_diamond_square_image(random_number_engine, std::max(width, height));
}

sdl2::surface<sdl2::index8>
generate_diamond_square_image(std::default_random_engine &random_number_engine, sdl2::size size)
{
    return generate_diamond_square_image(random_number_engine, size.width, size.height);
}

void rotate_left(std::vector<sdl2::color> & palette)
{
    std::rotate(palette.begin(), palette.begin() + 1, palette.end());
}

void rotate_right(std::vector<sdl2::color> & palette)
{
    std::rotate(palette.begin(), palette.end() - 1, palette.end());
}

int main()
{
    auto window = sdl2::window("Plasma Fractal", 640_px, 480_px, sdl2::window_flags::shown);

    auto event_queue = sdl2::event_queue();

    auto random_device = std::random_device();
    auto random_number_engine = std::default_random_engine(0);
    auto plasma_surface = generate_diamond_square_image(random_number_engine, window.size());
    auto plasma_surface_format = sdl2::pixel_format_details(plasma_surface);
    auto plasma_palette = sdl2::palette(plasma_surface_format);
    auto plasma_colors = std::vector<sdl2::color>(256);

    for (std::uint8_t i = 0; i < 32; ++i)
    {
        std::uint8_t lo = i * 255 / 31;
        std::uint8_t hi = 255 - lo;
        plasma_colors[i] = sdl2::color(lo, 0, 0, 255);
        plasma_colors[i + 32] = sdl2::color(hi, 0, 0, 255);
        plasma_colors[i + 64] = sdl2::color(0, lo, 0, 255);
        plasma_colors[i + 96] = sdl2::color(0, hi, 0, 255);
        plasma_colors[i + 128] = sdl2::color(0, 0, lo, 255);
        plasma_colors[i + 160] = sdl2::color(0, 0, hi, 255);
        plasma_colors[i + 192] = sdl2::color(lo, 0, lo, 255);
        plasma_colors[i + 224] = sdl2::color(hi, 0, hi, 255);
    }

    plasma_palette = plasma_colors;

    auto stopwatch = sdl2::stopwatch::start_now();
    auto reverse_rotation = false;
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

                case sdl2::event_type::key_up:
                    auto key_event = event.as<sdl2::keyboard_event>();
                    switch (key_event.scan_code())
                    {
                        case sdl2::scan_code::r:
                            reverse_rotation = !reverse_rotation;
                            break;
                    }
                    break;
            }
        }
        else
        {
            auto window_surface = sdl2::surface<sdl2::argb8888>(window);
            window_surface.blit(plasma_surface);
            window.update_surface();

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
