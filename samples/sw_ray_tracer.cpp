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

#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <ranges>
#include <set>
#include <vector>

#include <boost/poly_collection/base_collection.hpp>
#include <boost/qvm/all.hpp>

#include "SDL2pp/argb8888.h"
#include "SDL2pp/color.h"
#include "SDL2pp/event_queue.h"
#include "SDL2pp/event.h"
#include "SDL2pp/image.h"
#include "SDL2pp/keyboard_event.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/texture.h"
#include "SDL2pp/window.h"

#include "shared/math.h"
#include "shared/stopwatch.h"

using namespace std;
using namespace sdl2;

using vector3 = boost::qvm::vec<float, 3>;

inline
float degrees_to_radians(float degrees)
{
    return degrees * std::numbers::pi_v<float> / 180.0f;
}

inline
float distance(vector3 const& value1, vector3 const& value2)
{
    return boost::qvm::mag(value1 - value2);
}

using matrix4x4 = boost::qvm::mat<float, 4, 4>;

using quaternion = boost::qvm::quat<float>;

class rgb96f
{
public:
    static const rgb96f black;

    static const rgb96f white;

public:
    bool operator==(rgb96f const& other) const;

    rgb96f& operator+=(rgb96f const& other);

public:
    float r, g, b;
};

const rgb96f rgb96f::black { .r = 0.0, .g = 0.0, .b = 0.0 };

const rgb96f rgb96f::white { .r= 1.0, .g = 1.0, .b = 1.0 };

bool
rgb96f::operator==(rgb96f const& other) const
{
    return r == other.r && g == other.g && b == other.b;
}

rgb96f&
rgb96f::operator+=(rgb96f const& other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

rgb96f
operator+(rgb96f const& left, rgb96f const& right)
{
    return rgb96f {
        .r = left.r + right.r,
        .g = left.g + right.g,
        .b = left.b + right.b,
    };
}

rgb96f
operator*(rgb96f const& left, rgb96f const& right)
{
    return rgb96f {
        .r = left.r * right.r,
        .g = left.g * right.g,
        .b = left.b * right.b,
    };
}

rgb96f
operator*(rgb96f const& color, float scalar)
{
    return rgb96f {
        .r = color.r * scalar,
        .g = color.g * scalar,
        .b = color.b * scalar,
    };
}

class ray
{
public:
    static const float epsilon;

public:
    ray(vector3 const& origin, vector3 const& direction);

public:
    vector3 origin;

    vector3 direction;
};

const float ray::epsilon = 1e-2f;

ray::ray(vector3 const& origin, vector3 const& direction)
: origin(origin)
, direction(direction)
{ }

ray
transform_ray(ray const& ray, matrix4x4 const& matrix)
{
    auto transformed_origin = boost::qvm::transform_point(matrix, ray.origin);
    auto transformed_direction = boost::qvm::normalized(
        boost::qvm::transform_vector(matrix, ray.direction)
    );
    return ::ray(transformed_origin, transformed_direction);
}

struct frustum
{
    float left, right, bottom, top, near, far;

    float width() const;

    float height() const;
};

float
frustum::width() const
{
    return right - left;
}

float
frustum::height() const
{
    return top - bottom;
}

class perspective_camera
{
public:
    vector3 position = boost::qvm::zero_vec<float, 3>();

    quaternion orientation = boost::qvm::identity_quat<float>();

    frustum frustum = {
        .left = -4.0f / 3.0f,
        .right = 4.0f / 3.0f,
        .bottom = -1.0f,
        .top = 1.0f,
        .near = std::numeric_limits<float>::epsilon(),
        .far = std::numeric_limits<float>::infinity()
    };

    float field_of_view = 90.0f;
};

void
rotate_x(perspective_camera &camera, float degrees)
{
    boost::qvm::rotate_x(camera.orientation, degrees_to_radians(degrees));
}

void
rotate_y(perspective_camera &camera, float degrees)
{
    boost::qvm::rotate_y(camera.orientation, degrees_to_radians(degrees));
}

void
rotate_z(perspective_camera &camera, float degrees)
{
    boost::qvm::rotate_z(camera.orientation, degrees_to_radians(degrees));
}

void
move_to(perspective_camera &camera, vector3 const& position)
{
    camera.position = position;
}

void
move_forward(perspective_camera &camera, float distance)
{
    auto forward_vector = vector3 { 0.0f, 0.0f, 1.0f };
    auto longitudinal_axis = camera.orientation * forward_vector;
    camera.position += longitudinal_axis * distance;
}

void
move_backward(perspective_camera &camera, float distance)
{
    move_forward(camera, -distance);
}

void
move_right(perspective_camera &camera, float distance)
{
    auto const right_vector = vector3 { 1.0f, 0.0f, 0.0f };
    auto const lateral_axis = camera.orientation * right_vector;
    camera.position += lateral_axis * distance;
}

void
pitch(perspective_camera &camera, float degrees)
{
    auto const right_vector = vector3 { 1.0f, 0.0f, 0.0f };
    auto const pitch_axis = camera.orientation * right_vector;
    boost::qvm::rotate(camera.orientation, pitch_axis, degrees_to_radians(degrees));
}

void
roll(perspective_camera &camera, float degrees)
{
    auto const down_vector = vector3 { 0.0f, -1.0f, 0.0f };
    auto const yaw_axis = camera.orientation * down_vector;
    boost::qvm::rotate(camera.orientation, yaw_axis, degrees_to_radians(degrees));
}

void
move_left(perspective_camera &camera, float distance)
{
    move_right(camera, -distance);
}

matrix4x4
get_view_matrix(perspective_camera &camera)
{
    return boost::qvm::convert_to<matrix4x4>(
        boost::qvm::inverse(camera.orientation)
    ) * boost::qvm::translation_mat(-camera.position);
}

float
get_focal_length(perspective_camera const& camera, length<std::int32_t> width, length<std::int32_t> height)
{
    return quantity_cast<float>(width)
         / quantity_cast<float>(height)
         / std::tan(camera.field_of_view * std::numbers::pi_v<float> / 180.0f / 2.0f);
}

class point_light
{
public:
    point_light(vector3 const& position, rgb96f const& color);

    vector3 position;

    rgb96f color;
};

point_light::point_light(vector3 const& position, rgb96f const& color)
: position(position), color(color)
{ }

class primitive
{
protected:
    primitive(rgb96f const& diffuse_color);

public:
    virtual vector3 normal_at(vector3 const& point) const = 0;

    virtual optional<float> intersects_at(ray const& ray) const = 0;

public:
    float ambient_coefficient = 1.0;

    float diffuse_coefficient = 1.0;

    rgb96f diffuse_color = rgb96f::black;
};

primitive::primitive(rgb96f const& diffuse_color)
: diffuse_color(diffuse_color)
{ }

class sphere : public primitive
{
public:
    sphere(vector3 const& position, float radius, rgb96f const& diffuse_color);

    vector3 normal_at(vector3 const& point) const;

    optional<float> intersects_at(ray const& ray) const;

public:
    vector3 position;

    float radius;
};

sphere::sphere(vector3 const& position, float radius, rgb96f const& diffuse_color)
: primitive(diffuse_color)
, position(position)
, radius(radius)
{ }

vector3
sphere::normal_at(vector3 const& point) const
{
    return boost::qvm::normalized((point - position) / radius);
}

optional<float>
sphere::intersects_at(ray const& ray) const
{
    auto v = position - ray.origin;
    auto b = boost::qvm::dot(v, ray.direction);
    auto discriminant = b * b - boost::qvm::dot(v, v) + radius * radius;
    if (discriminant <= 0.0)
    {
        return nullopt;
    }

    discriminant = std::sqrt(discriminant);

    auto t2 = b + discriminant;
    if (t2 <= ray::epsilon)
    {
        return nullopt;
    }

    auto t1 = b - discriminant;
    if (t1 > ray::epsilon)
    {
        return t1;
    }

    return t2;
}

class plane : public primitive
{
public:
    plane(vector3 const& position, vector3 const& normal, rgb96f const& diffuse_color);

    vector3 normal_at(vector3 const& point) const;

    optional<float> intersects_at(ray const& ray) const;

public:
    vector3 position;

    vector3 normal;
};

plane::plane(vector3 const& position, vector3 const& normal, rgb96f const& diffuse_color)
: primitive(diffuse_color)
, position(position)
, normal(boost::qvm::normalized(normal))
{ }

vector3
plane::normal_at(vector3 const& point) const
{
    return normal;
}

optional<float>
plane::intersects_at(ray const& ray) const
{
    auto denominator = boost::qvm::dot(normal, ray.direction);
    if (denominator == 0.0)
    {
        return nullopt;
    }

    auto numerator = -boost::qvm::dot(normal, ray.origin + position);
    auto t = numerator / denominator;
    if (t <= ray::epsilon)
    {
        return nullopt;
    }

    return t;
}

class world
{
public:
    rgb96f ambient;

    boost::base_collection<primitive> objects;

    std::vector<point_light> lights;
};

struct intersection
{
    primitive const& hit;

    float hit_at;

    ray const& hit_by;
};

optional<intersection>
nearest_intersection(ray const& ray, world const& world)
{
    auto nearest_distance = std::numeric_limits<float>::infinity();
    auto nearest = world.objects.end();
    for (auto object_iterator = world.objects.begin(); object_iterator != world.objects.end(); ++object_iterator)
    {
        auto distance = object_iterator->intersects_at(ray);
        if (distance && distance < nearest_distance)
        {
            nearest_distance = *distance;
            nearest = object_iterator;
        }
    }

    if (nearest == world.objects.end())
    {
        return nullopt;
    }
    return intersection { *nearest, nearest_distance, ray };
}

float
shadow(ray const& ray, world const& world, float max_distance)
{
    auto nearest = nearest_intersection(ray, world);
    if (!nearest || nearest->hit_at > (max_distance - ::ray::epsilon))
    {
        return 1.0f;
    }
    return 0.0f;
}

rgb96f shade(world const& world, intersection const& intersection, int level, float weight)
{
    auto color = rgb96f::black;
    auto& object = intersection.hit;
    auto surface_point = intersection.hit_by.origin + intersection.hit_by.direction * intersection.hit_at;
    auto surface_normal = intersection.hit.normal_at(surface_point);

    for (auto& light : world.lights)
    {
        auto light_vector = boost::qvm::normalized(light.position - surface_point);
        auto illumination = boost::qvm::dot(surface_normal, light_vector);
        auto shadow_ray = ray(surface_point, light_vector);
        auto visibility = shadow(shadow_ray, world, std::abs(distance(surface_point, light.position)));
        if (illumination > 0.0f && visibility > 0.0f)
        {
            auto ambient_coefficient = object.ambient_coefficient;
            auto diffuse_coefficient = object.diffuse_coefficient;
            color += world.ambient * ambient_coefficient + light.color * diffuse_coefficient * object.diffuse_color * illumination;
        }
    }
    return color;
}

rgb96f trace(ray const& ray, world const& world, int level, float weight)
{
    auto nearest = nearest_intersection(ray, world);
    if (nearest)
    {
        return shade(world, *nearest, level, weight);
    }
    return rgb96f::black;
}

argb8888 to_argb8888(rgb96f const& color)
{
    return argb8888(
        255_a8,
        static_cast<r8>(
            static_cast<std::uint8_t>(
                std::round(
                    std::clamp(color.r, 0.0f, 1.0f) * 255.0f
                )
            )
        ),
        static_cast<g8>(
            static_cast<std::uint8_t>(
                std::round(
                    std::clamp(color.g, 0.0f, 1.0f) * 255.0f
                )
            )
        ),
        static_cast<b8>(
            static_cast<std::uint8_t>(
                std::round(
                    std::clamp(color.b, 0.0f, 1.0f) * 255.0f
                )
            )
        )
    );
}

int main()
{
    auto window = ::window("Software Ray Tracer", 640*px, 480*px, window_flags::shown | window_flags::resizable);
    auto renderer = ::renderer(window, renderer_flags::accelerated | renderer_flags::present_vsync);
    auto texture = ::texture<argb8888>(renderer, texture_access::streaming_access, renderer.output_size());
    auto event_queue = ::event_queue();
    
    auto world = ::world
    {
        .ambient = rgb96f { .r = 0.55f, .g = 0.44f, .b = 0.47f }
    };

    // Key light    
    world.lights.push_back(
        point_light(
            vector3 { -300.0f, 350.0f, 10.0f },
            rgb96f { 0.70f, 0.689f, 0.6885f }
        )
    );

    // Backdrop plane
    world.objects.insert(
        plane(
            vector3 { 0.0f, 0.0f, 0.0f },
            vector3 { 0.0f, 1.0f, 0.0f },
            rgb96f { 0.5f, 0.5f, 0.5f }
        )
    );

    // Large center orange sphere
    world.objects.insert(
        sphere(
            vector3 { 0.0f, 5.25f, 0.0f },
            10.5f / 2.0f,
            rgb96f { 0.89f, 0.48f, 0.42f }
        )
    );

    // Small center yellow sphere
    world.objects.insert(
        sphere(
            vector3 { -3.5f, 1.6f, -6.7f },
            3.2f / 2.0f,
            rgb96f { 0.95f, 0.93f, 0.31f }
        )
    );
    // Large back right pink sphere
    world.objects.insert(
        sphere(
            vector3 { 14.0f, 7.0f, 6.5f },
            14.0f / 2.0f,
            rgb96f { 1.0f, 0.44f, 0.64f }
        )
    );

    // Small front right orange sphere
    world.objects.insert(
        sphere(
            vector3 { 8.2f, 3.5f, -6.5f },
            7.0f / 2.0f,
            rgb96f { 0.89f, 0.48f, 0.42f }
        )
    );

    // Large back left pink sphere
    world.objects.insert(
        sphere(
            vector3 { -16.6f, 6.5f, 0.0f },
            13.0f / 2.0f,
            rgb96f { 1.0f, 0.44f, 0.64f }
        )
    );

    // Medium front back left pink sphere
    world.objects.insert(
        sphere(
            vector3 { -9.5f, 3.0f, -6.0f },
            6.0f / 2.0f,
            rgb96f { 1.0f, 0.44f, 0.64f }
        )
    );

    // Back left yellow sphere
    world.objects.insert(
        sphere(
            vector3 { -15.0f, 3.0f, 12.0f },
            6.0f / 2.0f,
            rgb96f { 0.95f, 0.93f, 0.31f }
        )
    );

    // Far back right blue sphere
    world.objects.insert(
        sphere(
            vector3 { 40.0f, 10.0f, 175.0f },
            20.0f / 2.0f,
            rgb96f { 0.18f, 0.31f, 0.68f }
        )
    );

    // Default camera
    auto camera = perspective_camera();
    move_to(camera, vector3 { 0.0f, 8.5f, -32.0f });
    rotate_z(camera, 180.0f);
    rotate_x(camera, 6.0f);

    auto stopwatch = stopwatch::start_now();
    auto running = true;
    while (running)
    {
        ::event event;
        if (event_queue.poll(event))
        {
            switch (event.type())
            {
                case event_type::quit:
                    running = false;
                    break;

                case event_type::key_down:
                    auto key_event = event.as<keyboard_event>();
                    switch (key_event.scan_code())
                    {
                        case scan_code::w:
                            move_forward(camera, 1.0f);
                            break;
                        case scan_code::s:
                            move_backward(camera, 1.0f);
                            break;
                        case scan_code::a:
                            move_left(camera, 1.0f);
                            break;
                        case scan_code::d:
                            move_right(camera, 1.0f);
                            break;
                        case scan_code::up:
                            pitch(camera, -1.0f);
                            break;
                        case scan_code::down:
                            pitch(camera, 1.0f);
                            break;
                        case scan_code::left:
                            roll(camera, -1.0f);
                            break;
                        case scan_code::right:
                            roll(camera, 1.0f);
                            break;
                    }
                    break;
            }
        }
        else
        {
            texture.with_lock(
                [&stopwatch, &world, &camera](image<argb8888> &screen)
                {
                    auto const screen_width = screen.width();
                    auto const screen_height = screen.height();
                    auto const camera_focal_length = get_focal_length(camera, screen_width, screen_height);
                    auto const camera_to_world_matrix = get_view_matrix(camera);
                    auto const& camera_ray_origin = boost::qvm::zero_vec<float, 3>();

                    auto const bottom_left = vector3
                    {
                        camera.frustum.left,
                        camera.frustum.bottom,
                        -camera_focal_length
                    };
                    
                    auto const delta_x = vector3
                    {
                        camera.frustum.width() / quantity_cast<float>(screen_width),
                        0.0f,
                        0.0f
                    };

                    auto const delta_y = vector3
                    {
                        0.0f,
                        camera.frustum.height() / quantity_cast<float>(screen_height),
                        0.0f
                    };

                    for (auto y = 0*px; y < screen_height; y += 1*px)
                    {
                        for (auto x = 0*px; x < screen_width; x += 1*px)
                        {
                            auto const camera_ray_direction = boost::qvm::normalized(
                                bottom_left
                                + delta_x * (0.5f + quantity_cast<float>(x))
                                + delta_y * (0.5f + quantity_cast<float>(y))
                            );

                            auto const primary_ray = ray(camera_ray_origin, camera_ray_direction);

                            screen(x, y) = to_argb8888(
                                trace(
                                    transform_ray(primary_ray, camera_to_world_matrix), world, 0, 1.0f
                                )
                            );
                        }
                    }
                }
            );

            renderer.draw_blend_mode(blend_mode::none);
            renderer.draw_color(color::black);
            renderer.clear();
            renderer.copy(texture);
            renderer.present();
        }
    }

    return 0;
}
