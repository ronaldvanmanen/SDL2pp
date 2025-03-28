#include <boost/test/unit_test.hpp>

#include "SDL2pp/sdl.h"
#include "SDL2pp/argb8888.h"

using namespace sdl2;

BOOST_AUTO_TEST_CASE(test_texture_argb8888)
{
    boost::unit_test::test_case const& current_test_case = boost::unit_test::framework::current_test_case();

    auto video_subsystem = sdl.init_video();
    auto window = video_subsystem.create_window(current_test_case.p_name, 640, 480, window_flags::hidden);
    auto renderer = window.create_renderer(renderer_flags::software);
    auto texture = renderer.create_texture<argb8888>(texture_access::streaming_access, renderer.output_size());
    texture.with_lock(
        [](sdl2::image<argb8888> &pixels)
        {
            for (int y = 0; y < pixels.height(); ++y)
            {
                for (int x = 0; x < pixels.width(); ++x)
                {
                    pixels(x, y) = argb8888(255, 255, 255, 255);
                }
            }
        });
    renderer.copy(texture);
    renderer.present();
}