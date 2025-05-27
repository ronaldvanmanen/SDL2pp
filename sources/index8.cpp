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

#pragma once

#include "SDL2pp/index8.h"

sdl2::index8::index8() { }

sdl2::index8::index8(std::uint8_t value)
: _value(value)
{ }

sdl2::index8::index8(sdl2::index8 const& other)
: _value(other._value)
{ }

sdl2::index8&
sdl2::index8::operator=(sdl2::index8 const& other)
{
    if (this != &other)
    {
        _value = other._value;
    }
    return *this;    
}

sdl2::index8&
sdl2::index8::operator+=(sdl2::index8 const& other)
{
    _value += other._value;
    return *this;    
}

sdl2::index8&
sdl2::index8::operator-=(sdl2::index8 const& other)
{
    _value += other._value;
    return *this;    
}

sdl2::index8&
sdl2::index8::operator++()
{
    ++_value;
    return *this;
}

sdl2::index8&
sdl2::index8::operator--()
{
    --_value;
    return *this;
}

bool
sdl2::index8::operator==(sdl2::index8 const& other) const
{
    return _value == other._value;
}

bool
sdl2::index8::operator<(sdl2::index8 const& other) const
{
    return _value < other._value;
}

sdl2::index8::operator std::uint8_t() const
{
    return _value;
}