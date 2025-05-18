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

#include <boost/units/quantity.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/physical_dimensions/length.hpp>

#include "pixel_base_unit.h"
#include "system_of_units.h"

namespace sdl2
{
    typedef boost::units::unit<boost::units::length_dimension, system_of_units> unit_of_length;

    template<typename Y>
    using length = boost::units::quantity<unit_of_length, Y>;

    template<typename Y>
    using offset = boost::units::quantity<unit_of_length, Y>;

    BOOST_UNITS_STATIC_CONSTANT(px, unit_of_length);
    BOOST_UNITS_STATIC_CONSTANT(pixel, unit_of_length);
    BOOST_UNITS_STATIC_CONSTANT(pixels, unit_of_length);
}
