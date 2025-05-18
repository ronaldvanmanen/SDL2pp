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

#include <boost/units/base_unit.hpp>
#include <boost/units/physical_dimensions/time.hpp>

namespace sdl2
{
    struct second_base_unit : public boost::units::base_unit<second_base_unit, boost::units::time_dimension, 2>
    {
        static std::string name()
        {
            return("second");
        }

        static std::string symbol()
        {
            return("s");
        }
    };
}
