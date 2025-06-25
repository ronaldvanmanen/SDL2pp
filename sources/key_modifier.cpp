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

#include "SDL2pp/key_modifier.h"

sdl2::key_modifier_set::key_modifier_set(std::uint16_t values)
: _values(values)
{ }

sdl2::key_modifier_set::key_modifier_set()
: sdl2::key_modifier_set(static_cast<std::uint16_t>(sdl2::key_modifier::none))
{ }

sdl2::key_modifier_set::key_modifier_set(sdl2::key_modifier value)
: _values(static_cast<std::uint16_t>(value))
{ }

sdl2::key_modifier_set&
sdl2::key_modifier_set::operator|=(sdl2::key_modifier_set const& other)
{
    _values |= other._values;
    return *this;
}

sdl2::key_modifier_set&
sdl2::key_modifier_set::operator&=(sdl2::key_modifier_set const& other)
{
    _values &= other._values;
    return *this;
}

sdl2::key_modifier_set&
sdl2::key_modifier_set::operator^=(sdl2::key_modifier_set const& other)
{
    _values ^= other._values;
    return *this;
}

sdl2::key_modifier_set&
sdl2::key_modifier_set::operator|=(sdl2::key_modifier const& value)
{
    _values |= static_cast<std::uint16_t>(value);
    return *this;
}

sdl2::key_modifier_set&
sdl2::key_modifier_set::operator&=(sdl2::key_modifier const& value)
{
    _values &= static_cast<std::uint16_t>(value);
    return *this;
}

sdl2::key_modifier_set&
sdl2::key_modifier_set::operator^=(sdl2::key_modifier const& value)
{
    _values ^= static_cast<std::uint16_t>(value);
    return *this;
}

sdl2::key_modifier_set
sdl2::key_modifier_set::operator~() const
{
    return sdl2::key_modifier_set(~_values);
}

bool
sdl2::key_modifier_set::operator==(sdl2::key_modifier_set const& other) const
{
    return _values == other._values;
}

bool
sdl2::key_modifier_set::operator==(sdl2::key_modifier const& value) const
{
    return _values == static_cast<std::uint16_t>(value);
}

bool
sdl2::key_modifier_set::test(sdl2::key_modifier value) const
{
    return _values & static_cast<std::uint16_t>(value);
}

sdl2::key_modifier_set
sdl2::operator|(sdl2::key_modifier left, sdl2::key_modifier right)
{
    return sdl2::key_modifier_set(
        static_cast<std::uint16_t>(left) | static_cast<std::uint16_t>(right)
    );
}

sdl2::key_modifier_set
sdl2::operator&(sdl2::key_modifier left, sdl2::key_modifier right)
{
    return sdl2::key_modifier_set(
        static_cast<std::uint16_t>(left) & static_cast<std::uint16_t>(right)
    );
}

sdl2::key_modifier_set
sdl2::operator^(sdl2::key_modifier left, sdl2::key_modifier right)
{
    return sdl2::key_modifier_set(
        static_cast<std::uint16_t>(left) ^ static_cast<std::uint16_t>(right)
    );
}

sdl2::key_modifier_set
sdl2::operator~(sdl2::key_modifier value)
{
    return sdl2::key_modifier_set(~static_cast<uint16_t>(value));
}
