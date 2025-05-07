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

#include <utility>

#include "SDL2pp/error.h"
#include "SDL2pp/palette.h"

namespace sdl2
{
    void set_palette_colors(SDL_Palette * palette, SDL_Color const* colors, int first_color, int num_colors)
    {
        throw_last_error(
            SDL_SetPaletteColors(palette, colors, first_color, num_colors) < 0
        );
    }

    void set_palette_colors(SDL_Palette * palette, color const* colors, std::size_t first_color, std::size_t num_colors)
    {
        set_palette_colors(
            palette,
            reinterpret_cast<SDL_Color const*>(colors),
            static_cast<int>(first_color),
            static_cast<int>(num_colors)
        );
    }
}

sdl2::palette::palette(std::size_t size)
: _native_handle(SDL_AllocPalette(static_cast<int>(size)))
, _free_handle(true)
{ }

sdl2::palette::palette(sdl2::pixel_format_details & owner)
: _native_handle(owner.native_handle()->palette)
, _free_handle(false)
{ }

sdl2::palette::palette(std::initializer_list<color> colors)
: _native_handle(SDL_AllocPalette(static_cast<int>(colors.size())))
, _free_handle(true)
{
    set_palette_colors(_native_handle, colors.begin(), 0, colors.size());
}

sdl2::palette::palette(std::vector<color> const& colors)
: _native_handle(SDL_AllocPalette(static_cast<int>(colors.size())))
, _free_handle(true)
{
    set_palette_colors(_native_handle, &colors[0], 0, colors.size());
}

sdl2::palette::palette(sdl2::palette const& other)
: _native_handle(SDL_AllocPalette(static_cast<int>(other.size())))
, _free_handle(true)
{
    set_palette_colors(_native_handle, other._native_handle->colors, 0, other._native_handle->ncolors);
}

sdl2::palette::palette(sdl2::palette && other)
: _native_handle(std::exchange(other._native_handle, nullptr))
, _free_handle(std::exchange(other._free_handle, false))
{ }

sdl2::palette::palette(SDL_Palette *native_handle, bool free_handle)
: _native_handle(native_handle)
, _free_handle(free_handle)
{ }

sdl2::palette::~palette()
{
    if (_free_handle && _native_handle != nullptr)
    {
        SDL_FreePalette(_native_handle);
    }
} 

sdl2::palette&
sdl2::palette::operator=(std::initializer_list<sdl2::color> colors)
{
    set_palette_colors(_native_handle, colors.begin(), 0, colors.size());

    return *this;
}

sdl2::palette&
sdl2::palette::operator=(std::vector<sdl2::color> const& colors)
{
    set_palette_colors(_native_handle, &colors[0], 0, colors.size());

    return *this;
}

sdl2::palette::indexed_color
sdl2::palette::operator[](std::size_t index)
{
    if (index >= size())
    {
        throw std::out_of_range("index must be less than size()");
    }
    return sdl2::palette::indexed_color(this, index);
}

sdl2::palette::const_indexed_color
sdl2::palette::operator[](std::size_t index) const
{
    if (index >= size())
    {
        throw std::out_of_range("index must be less than size()");
    }
    return sdl2::palette::const_indexed_color(this, index);
}

std::size_t
sdl2::palette::size() const
{
    return _native_handle->ncolors;
}

SDL_Palette*
sdl2::palette::native_handle()
{
    return _native_handle;
}

sdl2::palette::indexed_color::indexed_color(sdl2::palette * owner, std::size_t index)
: _owner(owner)
, _index(index)
{ }

sdl2::palette::indexed_color::indexed_color(sdl2::palette::indexed_color const& other)
: _owner(other._owner)
, _index(other._index)
{ }

sdl2::palette::indexed_color::indexed_color(sdl2::palette::indexed_color && other)
: _owner(std::exchange(other._owner, nullptr))
, _index(std::exchange(other._index, -1))
{ }

sdl2::palette::indexed_color&
sdl2::palette::indexed_color::operator=(sdl2::color const& value)
{
    sdl2::set_palette_colors(_owner->_native_handle, &value, _index, 1);

    return *this;
}

sdl2::color const&
sdl2::palette::indexed_color::get() const
{
    return reinterpret_cast<sdl2::color const&>(_owner->_native_handle->colors[_index]);
}

sdl2::palette::indexed_color::operator sdl2::color const&() const
{
    return get();
}

sdl2::palette::const_indexed_color::const_indexed_color(sdl2::palette const * owner, std::size_t index)
: _owner(owner)
, _index(index)
{ }

sdl2::palette::const_indexed_color::const_indexed_color(sdl2::palette::const_indexed_color const& other)
: _owner(other._owner)
, _index(other._index)
{ }

sdl2::color const&
sdl2::palette::const_indexed_color::get() const
{
    return reinterpret_cast<sdl2::color const&>(_owner->_native_handle->colors[_index]);
}

sdl2::palette::const_indexed_color::operator sdl2::color const&() const
{
    return get();
}

bool
sdl2::operator==(sdl2::palette::indexed_color const& left, sdl2::palette::indexed_color const& right)
{
    return left.get() == right.get();
}

bool
sdl2::operator!=(sdl2::palette::indexed_color const& left, sdl2::palette::indexed_color const& right)
{
    return !(left == right);
}

bool
sdl2::operator==(sdl2::palette::indexed_color const& left, sdl2::color const& right)
{
    return left.get() == right;
}

bool
sdl2::operator!=(sdl2::palette::indexed_color const& left, sdl2::color const& right)
{
    return !(left == right);
}

bool
sdl2::operator==(sdl2::color const& left, sdl2::palette::indexed_color const& right)
{
    return left == right.get();
}

bool
sdl2::operator!=(sdl2::color const& left, sdl2::palette::indexed_color const& right)
{
    return !(left == right);
}

std::ostream&
sdl2::operator<<(std::ostream& stream, sdl2::palette::indexed_color const& value)
{
    return stream << value.get();
}
