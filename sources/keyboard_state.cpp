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

#include <cstdint>

#include "SDL2pp/keyboard_state.h"

sdl2::keyboard_state::keyboard_state(std::uint8_t const* key_states, std::int32_t key_count)
: _key_states(key_states)
, _key_count(key_count)
{}

bool
sdl2::keyboard_state::pressed(scan_code key) const
{
    return _key_states[static_cast<int>(key)] == 1;
}
