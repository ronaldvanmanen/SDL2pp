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

#include <SDL2/SDL_events.h>

#include "key_code.h"
#include "key_event_type.h"
#include "key_modifier.h"
#include "key_state.h"
#include "scan_code.h"

namespace sdl2
{
    class keyboard_event
    {
    public:
        keyboard_event(SDL_Event const& native_handle);

        key_event_type type() const;

        std::uint32_t timestamp() const;
        
        std::uint32_t window_id() const;

        key_state state() const;

        std::uint8_t repeat() const;

        scan_code scan_code() const;
        
        key_code key_code() const;

        key_modifier_set key_modifiers() const;
    
    private:
        SDL_Event _native_handle;
    };
}
