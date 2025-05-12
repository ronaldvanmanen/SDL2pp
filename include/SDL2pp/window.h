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
#include <string>

#include <SDL2/SDL_video.h>

#include "length.h"
#include "size.h"
#include "window_flags.h"

namespace sdl2
{
    class window
    {
    public:
        window(std::string const& title, length width, length height);

        window(std::string const& title, length width, length height, window_flags flags);

        window(window const& other) = delete;

        window(window&& other);

        ~window();

        window& operator=(window const& other) = delete;

        size size() const;

        void update_surface();

        SDL_Window* native_handle();

    private:
        SDL_Window* _native_handle;
    };
}
