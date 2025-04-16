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
#include <array>
#include <cstdint>
#include <random>

#include "SDL2pp/argb8888.h"
#include "SDL2pp/color.h"
#include "SDL2pp/event_queue.h"
#include "SDL2pp/event.h"
#include "SDL2pp/image.h"
#include "SDL2pp/index8.h"
#include "SDL2pp/keyboard_event.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/texture.h"
#include "SDL2pp/window.h"

namespace sdl2
{
    template<typename PixelFormat, size_t Size>
    class palette
    {
    public:
        using size_type = std::array<PixelFormat, Size>::size_type;

    public:
        palette();

        PixelFormat& operator[](size_type index);

        PixelFormat const& operator[](size_type index) const;

        size_type size() const;
        
        void rotate_left();

        void rotate_right();

    private:
        std::array<PixelFormat, Size> _colors;

        size_type _offset;
    };

    class diamond_square_image_generator
    {
    public:
        diamond_square_image_generator();

        image<index8> generate(sdl2::size size);

        image<index8> generate(int width, int height);
        
        image<index8> generate(int size);
        
    private:
        void generate_diamond(image<index8> &map, int centerX, int centerY, int distance, int randomness);
        
        void generate_square(image<index8> &map, int center_x, int center_y, int distance, int randomness);

    private:
        std::random_device _random_number_generator;

        std::default_random_engine _random_number_engine;
    };

    int next_power_of_two(int value);
}

sdl2::diamond_square_image_generator::diamond_square_image_generator()
: _random_number_engine(_random_number_generator())
{ }

sdl2::image<sdl2::index8>
sdl2::diamond_square_image_generator::generate(sdl2::size size)
{
    return generate(size.width, size.height);
}

sdl2::image<sdl2::index8>
sdl2::diamond_square_image_generator::generate(int width, int height)
{
    auto size = std::max(width, height);
    auto image = generate(size);
    return image;
}

sdl2::image<sdl2::index8>
sdl2::diamond_square_image_generator::generate(int width_and_height)
{
    int size = sdl2::next_power_of_two(width_and_height) + 1;
    sdl2::image<index8> image(size, size);
    int randomness = 256;
    std::uniform_int_distribution<int> random_number_distribution(0, randomness - 1);

    image(0, 0) = static_cast<uint8_t>(random_number_distribution(_random_number_engine));
    image(size - 1, 0) = static_cast<uint8_t>(random_number_distribution(_random_number_engine));
    image(0, size - 1) = static_cast<uint8_t>(random_number_distribution(_random_number_engine));
    image(size - 1, size - 1) = static_cast<uint8_t>(random_number_distribution(_random_number_engine));

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

void
sdl2::diamond_square_image_generator::generate_diamond(sdl2::image<sdl2::index8> &map, int center_x, int center_y, int distance, int randomness)
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
    int random = random_number_distribution(_random_number_engine);
    int value = std::clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<uint8_t>(value);
}

void
sdl2::diamond_square_image_generator::generate_square(sdl2::image<index8> &map, int center_x, int center_y, int distance, int randomness)
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
    int random = random_number_distribution(_random_number_engine);
    int value = std::clamp(average + random, 0, 255);

    map(center_x, center_y) = static_cast<uint8_t>(value);
}

template<typename PixelFormat, size_t Size>
sdl2::palette<PixelFormat, Size>::palette()
: _offset(0)
{ }

template<typename PixelFormat, size_t Size>
PixelFormat&
sdl2::palette<PixelFormat, Size>::operator[](
    sdl2::palette<PixelFormat, Size>::size_type index
)
{
    return _colors[(_offset + index) % _colors.size()];
}

template<typename PixelFormat, size_t Size>
PixelFormat const&
sdl2::palette<PixelFormat, Size>::operator[](
    sdl2::palette<PixelFormat, Size>::size_type index
) const
{
    return _colors[(_offset + index) % _colors.size()];
}

template<typename PixelFormat, size_t Size>
sdl2::palette<PixelFormat, Size>::size_type
sdl2::palette<PixelFormat, Size>::size() const
{
    return _colors.size();
}

template<typename PixelFormat, size_t Size>
void
sdl2::palette<PixelFormat, Size>::rotate_left()
{
    --_offset;

    if (_offset < 0)
    {
        _offset += _colors.size();
    }
}

template<typename PixelFormat, size_t Size>
void
sdl2::palette<PixelFormat, Size>::rotate_right()
{
    ++_offset;
    
    if (_offset >= _colors.size())
    {
        _offset -= _colors.size();
    }
}

int sdl2::next_power_of_two(int value)
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

sdl2::palette<sdl2::argb8888, 256> generate_palette()
{
    sdl2::palette<sdl2::argb8888, 256> palette;

    for (std::uint8_t i = 0; i < 32; ++i)
    {
        std::uint8_t lo = i * 255 / 31;
        std::uint8_t hi = 255 - lo;
        palette[i] = sdl2::argb8888(255, lo, 0, 0);
        palette[i + 32] = sdl2::argb8888(255, hi, 0, 0);
        palette[i + 64] = sdl2::argb8888(255, 0, lo, 0);
        palette[i + 96] = sdl2::argb8888(255, 0, hi, 0);
        palette[i + 128] = sdl2::argb8888(255, 0, 0, lo);
        palette[i + 160] = sdl2::argb8888(255, 0, 0, hi);
        palette[i + 192] = sdl2::argb8888(255, lo, 0, lo);
        palette[i + 224] = sdl2::argb8888(255, hi, 0, hi);
    }

    return palette;
}

sdl2::image<sdl2::index8> generate_diamond_square_image(sdl2::size size)
{
    sdl2::diamond_square_image_generator generator;
    sdl2::image<sdl2::index8> image = generator.generate(size);
    return image;
}

int main()
{
    sdl2::window window("Plasma Fractal", 640, 480, sdl2::window_flags::shown | sdl2::window_flags::resizable);
    sdl2::renderer renderer(window, sdl2::renderer_flags::accelerated | sdl2::renderer_flags::present_vsync);
    sdl2::texture<sdl2::argb8888> texture(renderer, sdl2::texture_access::streaming_access, renderer.output_size());
    sdl2::image<sdl2::index8> image = generate_diamond_square_image(renderer.output_size());
    sdl2::palette<sdl2::argb8888, 256> palette = generate_palette();

    sdl2::event_queue event_queue;

    bool reverse_rotation = false;

    bool running = true;
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
            texture.with_lock(
                [palette, image](sdl2::image<sdl2::argb8888> &pixels)
                {
                    for (std::int32_t y = 0; y < pixels.height(); ++y)
                    {
                        for (std::int32_t x = 0; x < pixels.width(); ++x)
                        {
                            pixels(x, y) = palette[image(x, y)];
                        }
                    }
                }
            );

            renderer.set_draw_blend_mode(sdl2::blend_mode::none);
            renderer.set_draw_color(sdl2::color::black);
            renderer.clear();
            renderer.copy(texture);
            renderer.present();

            if (reverse_rotation)
            {
                palette.rotate_left();
            }
            else
            {
                palette.rotate_right();
            }
        }
    }

    return 0;
}
