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

#include <cstdint>

#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/frequency.hpp>

#include <SDL2/SDL_video.h>

#include "length.h"
#include "pixel_format.h"
#include "window.h"

namespace sdl2
{
    typedef boost::units::quantity<
        boost::units::si::frequency, std::int32_t
    > display_rate;

    class display_mode
    {
    public:
        display_mode(window & target);
        
        pixel_format format() const;
        
        length<std::int32_t> width() const;

        length<std::int32_t> height() const;

        display_rate refresh_rate() const;
    
    private:
        SDL_DisplayMode _native_handle;
    };
}
