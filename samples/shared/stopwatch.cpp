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

#include "stopwatch.h"

sdl2::stopwatch
sdl2::stopwatch::start_now()
{
    sdl2::stopwatch stopwatch;
    stopwatch.start();
    return stopwatch;
}

sdl2::stopwatch::stopwatch()
: _start_time(time_point::min())
, _elapsed_time(duration::zero())
, _running(false)
{ }

void
sdl2::stopwatch::start()
{
    if (_running) return;
    _start_time = clock::now();
    _running = true;
}

void
sdl2::stopwatch::stop()
{
    if (!_running) return;
    
    auto end_time = clock::now();
    auto lap_time = end_time - _start_time;
    _elapsed_time += lap_time;
    _running = false;
}

void
sdl2::stopwatch::reset()
{
    _start_time = clock::now();
    _elapsed_time = duration::zero();
}

sdl2::stopwatch::duration
sdl2::stopwatch::elapsed()
{
    auto elapsed_time = _elapsed_time;
    if (_running)
    {
        auto now = clock::now();
        auto lap_time = now - _start_time;
        elapsed_time += lap_time;
    }
    return elapsed_time;
}

sdl2::fractional_seconds
sdl2::elapsed_seconds(sdl2::stopwatch &stopwatch)
{
    return std::chrono::duration_cast<
        sdl2::fractional_seconds
    >(stopwatch.elapsed());
}

sdl2::time<double>
sdl2::elapsed_time(sdl2::stopwatch &s)
{
    return sdl2::time<double>(sdl2::elapsed_seconds(s).count() * sdl2::seconds);
}