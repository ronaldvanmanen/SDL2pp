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
    class palette
    {
    public:
        using size_type = std::array<argb8888, 256>::size_type;

    public:
        palette();

        argb8888& operator[](size_type index);

        const argb8888& operator[](size_type index) const;

        size_type size() const;
        
        void rotate_left();

        void rotate_right();

    private:
        std::array<argb8888, 256> _colors;

        size_type _offset;
    };
}

sdl2::palette::palette()
: _offset(0)
{ }

sdl2::argb8888&
sdl2::palette::operator[](sdl2::palette::size_type index)
{
    return _colors[(_offset + index) % _colors.size()];
}

const sdl2::argb8888&
sdl2::palette::operator[](sdl2::palette::size_type index) const
{
    return _colors[(_offset + index) % _colors.size()];
}

sdl2::palette::size_type
sdl2::palette::size() const
{
    return _colors.size();
}

void
sdl2::palette::rotate_left()
{
    --_offset;

    if (_offset < 0)
    {
        _offset += _colors.size();
    }
}

void
sdl2::palette::rotate_right()
{
    ++_offset;
    
    if (_offset >= _colors.size())
    {
        _offset -= _colors.size();
    }
}

sdl2::palette generate_palette()
{
    sdl2::palette palette;
    
    assert(palette.size() == 256);

    for (std::uint8_t i = 0; i < 32; ++i)
    {
        uint8_t lo = i * 255 / 31;
        uint8_t hi = 255 - lo;
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

int main()
{
    sdl2::window window("Plasma Fractal", 640, 480, sdl2::window_flags::shown | sdl2::window_flags::resizable);
    sdl2::renderer renderer(window, sdl2::renderer_flags::accelerated | sdl2::renderer_flags::present_vsync);
    sdl2::texture<sdl2::argb8888> texture(renderer, sdl2::texture_access::streaming_access, renderer.output_size());
    sdl2::palette palette = generate_palette();

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
                [palette](sdl2::image<sdl2::argb8888> &pixels)
                {
                    for (std::uint32_t y = 0; y < pixels.height(); ++y)
                    {
                        for (std::uint32_t x = 0; x < pixels.width(); ++x)
                        {
                            pixels(x, y) = palette[(x + y) % palette.size()];
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
