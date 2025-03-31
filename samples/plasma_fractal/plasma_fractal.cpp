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
#include <memory>
#include <random>
#include <vector>

#include "SDL2pp/sdl.h"
#include "SDL2pp/argb8888.h"
#include "SDL2pp/index8.h"

using namespace std;
using namespace sdl2;

namespace sdl2
{
    class palette
    {
    public:
        using size_type = typename vector<argb8888>::size_type;

    public:
        palette();

        argb8888& operator[](index8 index);

        const argb8888& operator[](index8 index) const;

        size_type size() const;

        void rotate_left();

        void rotate_right();

    private:
        vector<argb8888> _colors;

        size_type _offset;
    };

    palette::palette()
    : _colors(256)
    , _offset(0)
    { }

    argb8888&
    palette::operator[](index8 index)
    {
        return _colors[(_offset + index) % _colors.size()];
    }

    const argb8888&
    palette::operator[](index8 index) const
    {
        return _colors[(_offset + index) % _colors.size()];
    }

    palette::size_type
    palette::size() const
    {
        return _colors.size();
    }

    void
    palette::rotate_left()
    {
        --_offset;

        if (_offset < 0)
        {
            _offset += _colors.size();
        }
    }

    void
    palette::rotate_right()
    {
        ++_offset;
        
        if (_offset >= _colors.size())
        {
            _offset -= _colors.size();
        }
    }
}

random_device random_number_generator;

default_random_engine random_number_engine(random_number_generator());

palette generate_palette();

image<index8> generate_diamond_square_image(sdl2::size size);

image<index8> generate_diamond_square_image(int width, int height);

image<index8> generate_diamond_square_image(int size);

void generate_diamond(image<index8> &map, int centerX, int centerY, int distance, int randomness);

void generate_square(image<index8> &map, int center_x, int center_y, int distance, int randomness);

int next_power_of_two(int value);

int main()
{
    auto events_subsystem = sdl.init_events();

    auto video_subsystem = sdl.init_video();
    auto window = video_subsystem.create_window("Plasma Fractal", 640, 480, window_flags::shown | window_flags::resizable);
    auto renderer = window.create_renderer(renderer_flags::accelerated | renderer_flags::present_vsync);
    auto texture = renderer.create_texture<argb8888>(texture_access::streaming_access, renderer.output_size());

    auto palette = generate_palette();

    auto plasma_fractal_image = generate_diamond_square_image(renderer.output_size());

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

                case event_type::key_up:
                    plasma_fractal_image = generate_diamond_square_image(renderer.output_size());
                    break;
            }
        }
        else
        {
            texture.with_lock(
                [palette, plasma_fractal_image](sdl2::image<argb8888> &pixels)
                {
                    for (int y = 0; y < pixels.height(); ++y)
                    {
                        for (int x = 0; x < pixels.width(); ++x)
                        {
                            pixels(x, y) = palette[plasma_fractal_image(x, y)];
                        }
                    }
                });

            renderer.set_draw_blend_mode(blend_mode::none);
            renderer.set_draw_color(color::black);
            renderer.clear();
            renderer.copy(texture);
            renderer.present();

            palette.rotate_right();
        }
    }

    return 0;
}


image<index8> generate_diamond_square_image(sdl2::size size)
{
    return generate_diamond_square_image(size.width, size.height);
}

image<index8> generate_diamond_square_image(int width, int height)
{
    auto size = max(width, height);
    auto image = generate_diamond_square_image(size);
    return image;
}

image<index8> generate_diamond_square_image(int width_and_height)
{
    int size = next_power_of_two(width_and_height) + 1;
    image<index8> image(size, size);
    int randomness = 256;
    uniform_int_distribution<int> random_number_distribution(0, randomness - 1);

    image(0, 0) = static_cast<uint8_t>(random_number_distribution(random_number_engine));
    image(size - 1, 0) = static_cast<uint8_t>(random_number_distribution(random_number_engine));
    image(0, size - 1) = static_cast<uint8_t>(random_number_distribution(random_number_engine));
    image(size - 1, size - 1) = static_cast<uint8_t>(random_number_distribution(random_number_engine));

    randomness /= 2;

    for (int step_size = size - 1; step_size > 1; step_size /= 2)
    {
        int half_step_size = step_size / 2;

        for (int y = half_step_size; y < image.height(); y += step_size)
        {
            for (int x = half_step_size; x < image.width(); x += step_size)
            {
                generate_diamond(image, x, y, half_step_size, randomness);
            }
        }

        for (int y = 0; y < image.height(); y += half_step_size)
        {
            for (int x = (y % step_size) == 0 ? half_step_size : 0; x < image.width(); x += step_size)
            {
                generate_square(image, x, y, half_step_size, randomness);
            }
        }

        randomness /= 2;
    }
    return image;
}

void generate_diamond(image<index8> &map, int center_x, int center_y, int distance, int randomness)
{
    uniform_int_distribution<int> random_number_distribution(-randomness, randomness);

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
    int value = clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<uint8_t>(value);
}

void generate_square(image<index8> &map, int center_x, int center_y, int distance, int randomness)
{
    uniform_int_distribution<int> random_number_distribution(-randomness, randomness);

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
    int value = clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<uint8_t>(value);
}

palette generate_palette()
{
    palette result;

    for (index8 i = 0; i < 32; ++i)
    {
        uint8_t lo = i * 255 / 31;
        uint8_t hi = 255 - lo;
        result[i] = argb8888(255, lo, 0, 0);
        result[i + 32] = argb8888(255, hi, 0, 0);
        result[i + 64] = argb8888(255, 0, lo, 0);
        result[i + 96] = argb8888(255, 0, hi, 0);
        result[i + 128] = argb8888(255, 0, 0, lo);
        result[i + 160] = argb8888(255, 0, 0, hi);
        result[i + 192] = argb8888(255, lo, 0, lo);
        result[i + 224] = argb8888(255, hi, 0, hi);
    }

    return result;
}

int next_power_of_two(int value)
{
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;
    return value;
}
