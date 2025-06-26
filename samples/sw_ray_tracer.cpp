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
#include <ranges>
#include <set>
#include <utility>
#include <vector>

#include <boost/poly_collection/base_collection.hpp>
#include <boost/qvm/all.hpp>

#include "SDL2pp/argb8888.h"
#include "SDL2pp/color.h"
#include "SDL2pp/event_queue.h"
#include "SDL2pp/event.h"
#include "SDL2pp/image.h"
#include "SDL2pp/keyboard_event.h"
#include "SDL2pp/keyboard.h"
#include "SDL2pp/mouse_wheel_event.h"
#include "SDL2pp/mouse.h"
#include "SDL2pp/renderer.h"
#include "SDL2pp/texture.h"
#include "SDL2pp/window.h"

#include "shared/math.h"
#include "shared/rgb96f.h"
#include "shared/stopwatch.h"

using namespace std;
using namespace sdl2;

static int const max_level = 5;
static float const min_weight = 0.01f;

namespace boost { namespace qvm {

template <class A,class B>
BOOST_QVM_CONSTEXPR BOOST_QVM_INLINE_OPERATIONS
typename lazy_enable_if_c<
    is_vec<A>::value && vec_traits<A>::dim==3 && is_quat<B>::value, deduce_vec2<A,B,3>
>::type
operator*( A const & a, B const & b )
{
    typedef typename deduce_vec2<A,B,3>::type R;
    typedef typename vec_traits<A>::scalar_type TA;
    typedef typename quat_traits<B>::scalar_type TB;
    
    TA const x = vec_traits<A>::template read_element<0>(a);
    TA const y = vec_traits<A>::template read_element<1>(a);
    TA const z = vec_traits<A>::template read_element<2>(a);

    TB const qw = quat_traits<B>::template read_element<0>(b);
    TB const qx = quat_traits<B>::template read_element<1>(b);
    TB const qy = quat_traits<B>::template read_element<2>(b);
    TB const qz = quat_traits<B>::template read_element<3>(b);

    R r;
    write_vec_element<0>(r, x*(qx*qx+qw*qw-qy*qy- qz*qz) + y*(2*qx*qy- 2*qw*qz) + z*(2*qx*qz+ 2*qw*qy));
    write_vec_element<1>(r, x*(2*qw*qz + 2*qx*qy) + y*(qw*qw - qx*qx+ qy*qy - qz*qz)+ z*(-2*qw*qx+ 2*qy*qz));
    write_vec_element<2>(r, x*(-2*qw*qy+ 2*qx*qz) + y*(2*qw*qx+ 2*qy*qz)+ z*(qw*qw - qx*qx- qy*qy+ qz*qz));
    return r;
}

namespace
sfinae
    {
    using ::boost::qvm::operator*;
    }

} }

using vector3 = boost::qvm::vec<float, 3>;

auto const& zero_vector = boost::qvm::zero_vec<float, 3>();

auto const left_vector = vector3 { -1.0f, 0.0f, 0.0f };
auto const right_vector = vector3 { 1.0f, 0.0f, 0.0f };
auto const down_vector = vector3 { 0.0f, -1.0f, 0.0f };
auto const up_vector = vector3 { 0.0f, 1.0f, 0.0f };
auto const backward_vector = vector3 { 0.0f, 0.0f, -1.0f };
auto const forward_vector = vector3 { 0.0f, 0.0f, 1.0f };

template <typename T>
T sign(T val)
{
    auto const zero = static_cast<T>(0);
    auto const result = static_cast<T>((zero < val) - (val < zero));
    return result;
}

inline
float distance(vector3 const& a, vector3 const& b)
{
    return mag(a - b);
}

vector3
faceforward(vector3 const& n, vector3 const& i, vector3 const& n_ref)
{
    return sign(-dot(i, n_ref)) * n;
}

vector3
reflect(vector3 const& i, vector3 const& n)
{
    return i - 2.0f * dot(i, n) * n;
}

vector3
refract(vector3 const& i, vector3 const& n, float eta)
{
    float const i_dot_n = dot(i, n);
    float const k = 1 - eta * eta * (1 - i_dot_n * i_dot_n);
    return (k < 0.0f) ? vector3 { 0.0f, 0.0f, 0.0f } : eta * i - (eta * i_dot_n + sqrt(k)) * n;
}

using matrix4x4 = boost::qvm::mat<float, 4, 4>;

inline
matrix4x4
look_at_lh(vector3 const& eye, vector3 const& at, vector3 const& up)
{
    auto const zaxis = normalized(at - eye);
    auto const xaxis = normalized(cross(up, zaxis));
    auto const yaxis = cross(zaxis, xaxis);
    auto result = matrix4x4();

    write_mat_element<0, 0>(result, boost::qvm::vec_traits<vector3>::read_element<0>(xaxis));
    write_mat_element<0, 1>(result, boost::qvm::vec_traits<vector3>::read_element<0>(yaxis));
    write_mat_element<0, 2>(result, boost::qvm::vec_traits<vector3>::read_element<0>(zaxis));
    
    write_mat_element<1, 0>(result, boost::qvm::vec_traits<vector3>::read_element<1>(xaxis));
    write_mat_element<1, 1>(result, boost::qvm::vec_traits<vector3>::read_element<1>(yaxis));
    write_mat_element<1, 2>(result, boost::qvm::vec_traits<vector3>::read_element<1>(zaxis));
    
    write_mat_element<2, 0>(result, -boost::qvm::vec_traits<vector3>::read_element<2>(xaxis));
    write_mat_element<2, 1>(result, -boost::qvm::vec_traits<vector3>::read_element<2>(yaxis));
    write_mat_element<2, 2>(result, -boost::qvm::vec_traits<vector3>::read_element<2>(zaxis));

    write_mat_element<0, 3>(result, -dot(xaxis, eye));
    write_mat_element<1, 3>(result, -dot(yaxis, eye));
    write_mat_element<2, 3>(result, -dot(zaxis, eye));
    write_mat_element<3, 3>(result, 1.0f);

    return result;
}

inline
matrix4x4
look_at_rh(vector3 const& eye, vector3 const& at, vector3 const& up)
{
    auto const zaxis = normalized(eye - at);
    auto const xaxis = normalized(cross(up, zaxis));
    auto const yaxis = cross(zaxis, xaxis);
    auto result = matrix4x4();
    write_mat_element<0, 0>(result, boost::qvm::vec_traits<vector3>::read_element<0>(xaxis));
    write_mat_element<0, 1>(result, boost::qvm::vec_traits<vector3>::read_element<0>(yaxis));
    write_mat_element<0, 2>(result, boost::qvm::vec_traits<vector3>::read_element<0>(zaxis));
    
    write_mat_element<1, 0>(result, boost::qvm::vec_traits<vector3>::read_element<1>(xaxis));
    write_mat_element<1, 1>(result, boost::qvm::vec_traits<vector3>::read_element<1>(yaxis));
    write_mat_element<1, 2>(result, boost::qvm::vec_traits<vector3>::read_element<1>(zaxis));
    
    write_mat_element<2, 0>(result, -boost::qvm::vec_traits<vector3>::read_element<2>(xaxis));
    write_mat_element<2, 1>(result, -boost::qvm::vec_traits<vector3>::read_element<2>(yaxis));
    write_mat_element<2, 2>(result, -boost::qvm::vec_traits<vector3>::read_element<2>(zaxis));
    
    write_mat_element<0, 3>(result, -dot(xaxis, eye));
    write_mat_element<1, 3>(result, -dot(yaxis, eye));
    write_mat_element<2, 3>(result, -dot(zaxis, eye));
    write_mat_element<3, 3>(result, 1.0f);
    return result;
}

using quaternion = boost::qvm::quat<float>;

quaternion
rotation(matrix4x4 const& a)
{
    return normalized(convert_to<quaternion>(del_row_col<3, 3>(a)));
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
, direction(normalized(direction))
{ }

ray
transform_ray(ray const& ray, matrix4x4 const& matrix)
{
    auto const transformed_origin = transform_point(matrix, ray.origin);
    auto const transformed_direction = transform_vector(matrix, ray.direction);
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
        .near = numeric_limits<float>::epsilon(),
        .far = numeric_limits<float>::infinity()
    };

    float field_of_view = 90.0f;

    void look_at_lh(vector3 const& eye, vector3 const& at, vector3 const& up);

    void look_at_rh(vector3 const& eye, vector3 const& at, vector3 const& up);

    void pitch(float degrees);

    void yaw(float degrees);

    void roll(float degrees);

    void rotate(vector3 const& axis, float degrees);

    void move_to(vector3 const& position);

    void move(vector3 const& distance);

    void move_left(float distance);

    void move_right(float distance);

    void move_up(float distance);

    void move_down(float distance);

    void move_forward(float distance);

    void move_backward(float distance);

    void zoom_in(float amount);

    void zoom_out(float amount);

    void zoom(float amount);

    matrix4x4 view_matrix() const;
};

void
perspective_camera::look_at_lh(vector3 const& eye, vector3 const& at, vector3 const& up)
{
    auto const look_at_matrix = ::look_at_lh(eye, at, up);
    orientation = rotation(look_at_matrix);
    position = translation(look_at_matrix);
}

void
perspective_camera::look_at_rh(vector3 const& eye, vector3 const& at, vector3 const& up)
{
    auto const look_at_matrix = ::look_at_rh(eye, at, up);
    orientation = rotation(look_at_matrix);
    position = translation(look_at_matrix);
}

void
perspective_camera::pitch(float degrees)
{
    rotate(right_vector, degrees);
}

void
perspective_camera::yaw(float degrees)
{
    rotate(up_vector, degrees);
}

void
perspective_camera::roll(float degrees)
{
    rotate(forward_vector, degrees);
}

void
perspective_camera::rotate(vector3 const& axis, float degrees)
{
    orientation = rot_quat(axis, -degrees_to_radians(degrees)) * orientation;
}

void
perspective_camera::move_to(vector3 const& position)
{
    this->position = position;
}

void
perspective_camera::move(vector3 const& distance)
{
    position += inverse(orientation) * -distance;
}

void
perspective_camera::move_left(float distance)
{
    move(left_vector * distance);
}

void
perspective_camera::move_right(float distance)
{
    move(right_vector * distance);
}

void
perspective_camera::move_up(float distance)
{
    move(up_vector * distance);
}

void
perspective_camera::move_down(float distance)
{
    move(down_vector * distance);
}

void
perspective_camera::move_forward(float distance)
{
    move(forward_vector * distance);
}

void
perspective_camera::move_backward(float distance)
{
    move(backward_vector * distance);
}

void
perspective_camera::zoom_in(float amount)
{
    zoom(amount);
}

void
perspective_camera::zoom_out(float amount)
{
    zoom(-amount);
}

void
perspective_camera::zoom(float amount)
{
    field_of_view -= amount;
}

matrix4x4
perspective_camera::view_matrix() const
{
    return inverse(
        convert_to<matrix4x4>(orientation) * translation_mat(position)
    );
}

float
get_focal_length(perspective_camera const& camera, length<int32_t> width, length<int32_t> height)
{
    return quantity_cast<float>(width)
         / quantity_cast<float>(height)
         / tan(degrees_to_radians(camera.field_of_view / 2.0f));
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

struct surface
{
    float ambient_coefficient = 1.0f;

    float diffuse_coefficient = 1.0f;

    rgb96f color = rgb96f::black;

    float index_of_refraction = 0.0f;

    float reflective_coefficient = 0.0f;

    float transmissive_coefficient = 0.0f;

    float specular_coefficient = 0.0f;

    float specular_exponent = 0.0f;
};

class solid
{
protected:
    solid(surface const& surface);

public:
    virtual vector3 normal_at(vector3 const& point) const = 0;

    virtual optional<float> hit_test(ray const& ray) const = 0;

    float ambient_coefficient;

    float diffuse_coefficient;

    rgb96f surface_color;

    float index_of_refraction;

    float reflective_coefficient;

    float transmissive_coefficient;

    float specular_coefficient;

    float specular_exponent;
};

solid::solid(surface const& surface)
: ambient_coefficient(surface.ambient_coefficient)
, diffuse_coefficient(surface.diffuse_coefficient)
, surface_color(surface.color)
, index_of_refraction(surface.index_of_refraction)
, reflective_coefficient(surface.reflective_coefficient)
, transmissive_coefficient(surface.transmissive_coefficient)
, specular_coefficient(surface.specular_coefficient)
, specular_exponent(surface.specular_exponent)
{ }

class sphere : public solid
{
public:
    sphere(vector3 const& position, float radius, surface const& surface);

    vector3 normal_at(vector3 const& point) const;

    optional<float> hit_test(ray const& ray) const;

public:
    vector3 position;

    float radius;
};

sphere::sphere(vector3 const& position, float radius, surface const& surface)
: solid(surface)
, position(position)
, radius(radius)
{ }

vector3
sphere::normal_at(vector3 const& point) const
{
    return normalized((point - position) / radius);
}

optional<float>
sphere::hit_test(ray const& ray) const
{
    auto const v = position - ray.origin;
    auto const b = dot(v, ray.direction);
    auto const d = b * b - dot(v, v) + radius * radius;
    if (d <= 0.0)
    {
        return nullopt;
    }

    auto const discriminant = sqrt(d);

    auto const t2 = b + discriminant;
    if (t2 <= ray::epsilon)
    {
        return nullopt;
    }

    auto const t1 = b - discriminant;
    if (t1 > ray::epsilon)
    {
        return t1;
    }

    return t2;
}

class plane : public solid
{
public:
    plane(vector3 const& position, vector3 const& normal, surface const& surface);

    vector3 normal_at(vector3 const& point) const;

    optional<float> hit_test(ray const& ray) const;

public:
    vector3 position;

    vector3 normal;
};

plane::plane(vector3 const& position, vector3 const& normal, surface const& surface)
: solid(surface)
, position(position)
, normal(normalized(normal))
{ }

vector3
plane::normal_at(vector3 const& point) const
{
    return normal;
}

optional<float>
plane::hit_test(ray const& ray) const
{
    auto const denominator = dot(normal, ray.direction);
    if (denominator == 0.0)
    {
        return nullopt;
    }

    auto const numerator = -dot(normal, ray.origin + position);
    auto const t = numerator / denominator;
    if (t <= ray::epsilon)
    {
        return nullopt;
    }

    return t;
}

struct world
{
    rgb96f ambient;

    float min_depth;

    float max_depth;

    rgb96f depth_color;

    rgb96f environment;

    boost::base_collection<solid> objects;

    vector<point_light> lights;
};

struct hit
{
    ::ray const& ray;

    float distance;

    ::solid const& object;
};

optional<hit>
find_nearest_hit(ray const& ray, world const& world)
{
    auto nearest_object_distance = numeric_limits<float>::infinity();
    auto nearest_object = world.objects.end();
    for (auto object_iterator = world.objects.begin(); object_iterator != world.objects.end(); ++object_iterator)
    {
        auto object_distance = object_iterator->hit_test(ray);
        if (object_distance && object_distance < nearest_object_distance)
        {
            nearest_object_distance = *object_distance;
            nearest_object = object_iterator;
        }
    }

    if (nearest_object == world.objects.end())
    {
        return nullopt;
    }

    return hit { ray, nearest_object_distance, *nearest_object };
}

rgb96f
shade(hit const& hit, world const& world, int level, float weight);

rgb96f
trace(ray const& ray, world const& world, int level, float weight)
{
    auto const nearest_hit = find_nearest_hit(ray, world);
    if (nearest_hit)
    {
        return shade(*nearest_hit, world, level, weight);
    }
    return world.environment;
}

rgb96f
trace(ray const& ray, world const& world)
{
    return trace(ray, world, 0, 1.0f);
}

float
shadow(ray const& ray, world const& world, float max_distance);

rgb96f
shade(hit const& hit, world const& world, int level, float weight)
{
    auto const surface_position = hit.ray.origin + hit.ray.direction * hit.distance;
    auto const geometry_normal = hit.object.normal_at(surface_position);
    auto const shading_normal = faceforward(geometry_normal, hit.ray.direction, geometry_normal);
    auto const viewing_vector = -hit.ray.direction;

    // Ambient
    auto color = hit.object.ambient_coefficient * world.ambient * hit.object.surface_color;

    for (auto const& light : world.lights)
    {
        auto const light_vector = normalized(light.position - surface_position);
        auto const amount_of_illumination = dot(shading_normal, light_vector);
        auto const shadow_ray = ray(surface_position, light_vector);
        auto const visibility = shadow(shadow_ray, world, abs(distance(surface_position, light.position)));

        if (amount_of_illumination > 0.0f && visibility > 0.0f)
        {
            // Diffuse
            color += hit.object.diffuse_coefficient * light.color * amount_of_illumination;
            // Specular
            auto const half_vector = normalized(light_vector + viewing_vector);
            color += hit.object.specular_coefficient * light.color * powf(
                max(0.0f, dot(shading_normal, half_vector)), hit.object.specular_exponent
            );
        }
    }

    if (level < max_level)
    {
        // Reflection
        auto const reflective_weight = hit.object.reflective_coefficient * weight;
        if (reflective_weight > min_weight)
        {
            auto const reflected_ray = ray(surface_position, reflect(hit.ray.direction, shading_normal));
            color += hit.object.reflective_coefficient * trace(
                reflected_ray, world, level + 1, reflective_weight
            );
        }

        // Transmission
        auto const transmissive_weight = hit.object.transmissive_coefficient * weight;
        if (transmissive_weight > min_weight)
        {
            auto const eta = dot(shading_normal, hit.ray.direction) < 0.0f ? hit.object.index_of_refraction : 1.0f / hit.object.index_of_refraction;
            auto const transmitted_vector = refract(hit.ray.direction, shading_normal, eta);
            if (mag(transmitted_vector) != 0.0)
            {
                auto const transmitted_ray = ray(surface_position, transmitted_vector);
                color += hit.object.transmissive_coefficient * trace(
                    transmitted_ray, world, level + 1, transmissive_weight
                );
            }
        }
    }

    return color;
}

rgb96f
shade(hit const& hit, world const& world)
{
    return shade(hit, world, 0, 1.0f);
}

float
shadow(ray const& ray, world const& world, float max_distance)
{
    auto const nearest_hit = find_nearest_hit(ray, world);
    if (!nearest_hit || nearest_hit->distance > (max_distance - ::ray::epsilon))
    {
        return 1.0f;
    }
    return 0.0f;
}

argb8888 to_argb8888(rgb96f const& color)
{
    return argb8888(
        255_a8,
        static_cast<r8>(
            static_cast<uint8_t>(
                round(
                    clamp(color.r, 0.0f, 1.0f) * 255.0f
                )
            )
        ),
        static_cast<g8>(
            static_cast<uint8_t>(
                round(
                    clamp(color.g, 0.0f, 1.0f) * 255.0f
                )
            )
        ),
        static_cast<b8>(
            static_cast<uint8_t>(
                round(
                    clamp(color.b, 0.0f, 1.0f) * 255.0f
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

    // Scene
    auto world = ::world
    {
        .ambient = rgb96f { .r = 0.55f, .g = 0.44f, .b = 0.47f },
        .min_depth = 1.0f,
        .max_depth = 298.0f,
        .depth_color = rgb96f { .r = 0.86f, .g = 0.88f, .b = 0.95f },
        .environment = rgb96f { .r = 0.62f, .g = 0.69f, .b = 0.96f }
    };

    // Key light
    world.lights.push_back(
        point_light(
            vector3 { -300.0f, 350.0f, 10.0f },
            rgb96f { .r = 0.70f, .g = 0.689f, .b = 0.6885f }
        )
    );

    // Backdrop plane
    world.objects.insert(
        plane(
            vector3 { 0.0f, 0.0f, 0.0f },
            vector3 { 0.0f, 1.0f, 0.0f },
            surface {
                .color = rgb96f { .r = 1.0f, .g = 1.0f, .b = 1.0f },
                .reflective_coefficient = 0.0f,
                .specular_coefficient = 0.5f,
                .specular_exponent = 0.8f,
            }
        )
    );

    // Large center orange sphere
    world.objects.insert(
        sphere(
            vector3 { 0.0f, 5.25f, 0.0f },
            5.25f,
            surface {
                .color = rgb96f { .r = 0.89f, .g = 0.48f, .b = 0.42f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Small center yellow sphere
    world.objects.insert(
        sphere(
            vector3 { -3.5f, 1.6f, -6.7f },
            1.6f,
            surface {
                .color = rgb96f { .r = 0.95f, .g = 0.93f, .b = 0.31f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Large back right pink sphere
    world.objects.insert(
        sphere(
            vector3 { 14.0f, 7.0f, 6.5f },
            7.0f,
            surface {
                .color = rgb96f { .r = 1.0f, .g = 0.44f, .b = 0.64f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Small front right orange sphere
    world.objects.insert(
        sphere(
            vector3 { 8.2f, 3.5f, -6.5f },
            3.5f,
            surface {
                .color = rgb96f { .r = 0.89f, .g = 0.48f, .b = 0.42f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Large back left pink sphere
    world.objects.insert(
        sphere(
            vector3 { -16.6f, 6.5f, 0.0f },
            6.5f,
            surface {
                .color = rgb96f { .r = 1.0f, .g = 0.44f, .b = 0.64f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Medium front back left pink sphere
    world.objects.insert(
        sphere(
            vector3 { -9.5f, 3.0f, -6.0f },
            3.0f,
            surface {
                .color = rgb96f { .r = 1.0f, .g = 0.44f, .b = 0.64f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Back left yellow sphere
    world.objects.insert(
        sphere(
            vector3 { -15.0f, 3.0f, 12.0f },
            3.0f,
            surface {
                .color = rgb96f { .r = 0.95f, .g = 0.93f, .b = 0.31f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Far back right blue sphere
    world.objects.insert(
        sphere(
            vector3 { 40.0f, 10.0f, 175.0f },
            10.0f,
            surface {
                .color = rgb96f { .r = 0.18f, .g = 0.31f, .b = 0.68f },
                .reflective_coefficient = 0.15f,
                .specular_coefficient = 1.0f,
                .specular_exponent = 165.0f,
            }
        )
    );

    // Default camera
    auto camera = perspective_camera
    {
        .field_of_view = 54.4f
    };

    camera.look_at_lh(
        vector3 { 0.0f, 8.5f, -32.0f },
        vector3 { 0.0f, 8.25f, 0.0f },
        vector3 { 0.0f, 1.0f, 0.0f }
    );

    camera.pitch(6);

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
                {
                    running = false;
                }
                break;

                case event_type::key_up:
                {
                    auto const key_event = event.as<keyboard_event>();
                    auto const symbol = key_event.scan_code();
                    auto const modifiers = key_event.key_modifiers();
                    if (symbol == scan_code::m && modifiers == (key_modifier::left_ctrl | key_modifier::left_alt))
                    {
                        mouse::relative_mode(!mouse::relative_mode());
                    }
                }
                break;

                case event_type::mouse_wheel:
                {
                    auto const wheel_event = event.as<mouse_wheel_event>();
                    auto const amount = wheel_event.y();
                    camera.zoom(static_cast<float>(amount));
                }
                break;
            }
        }
        else
        {
            auto const keys = keyboard::state();
            auto const key_modifiers = keyboard::modifier_state();
            auto const num_lock = key_modifiers.test(key_modifier::num_lock);
            auto const left_alt = key_modifiers.test(key_modifier::left_alt);
            auto const left_ctrl = key_modifiers.test(key_modifier::left_ctrl);
            auto const left_shift = key_modifiers.test(key_modifier::left_shift);

            if (keys.pressed(scan_code::w))
            {
                if (left_alt)
                {
                    camera.move_up(left_shift ? 2.0f : 1.0f);
                }
                else
                {
                    camera.move_forward(left_shift ? 2.0f : 1.0f);
                }
            }
            
            if (keys.pressed(scan_code::s))
            {
                if (left_alt)
                {
                    camera.move_down(left_shift ? 2.0f : 1.0f);
                }
                else
                {
                    camera.move_backward(left_shift ? 2.0f : 1.0f);
                }
            }

            if (keys.pressed(scan_code::a))
            {
                camera.move_left(left_shift ? 2.0f : 1.0f);
            }
            
            if (keys.pressed(scan_code::d))
            {
                camera.move_right(left_shift ? 2.0f : 1.0f);
            }

            if (keys.pressed(scan_code::up) || keys.pressed(scan_code::keypad_8))
            {
                camera.pitch(left_shift ? 2.0f : 1.0f);
            }
            
            if (keys.pressed(scan_code::down) || keys.pressed(scan_code::keypad_2))
            {
                camera.pitch(left_shift ? -2.0f : -1.0f);
            }

            if (keys.pressed(scan_code::left) || keys.pressed(scan_code::keypad_4))
            {
                camera.yaw(left_shift ? -2.0f : -1.0f);
            }

            if (keys.pressed(scan_code::right) || keys.pressed(scan_code::keypad_6))
            {
                camera.yaw(left_shift ? 2.0f : 1.0f);
            }

            if (keys.pressed(scan_code::keypad_plus))
            {
                camera.zoom_in(left_shift ? 2.0f : 1.0f);
            }

            if (keys.pressed(scan_code::keypad_minus))
            {
                camera.zoom_out(left_shift ? 2.0f : 1.0f);
            }

            if (mouse::relative_mode())
            {
                auto const mouse_state = mouse::relative_state();
                camera.yaw(mouse_state.x * 0.1f);
                camera.pitch(mouse_state.y * 0.1f);
            }

            texture.with_lock(
                [&stopwatch, &world, &camera](image<argb8888> &raster)
                {
                    auto const raster_width = raster.width();
                    auto const raster_height = raster.height();
                    auto const screen_left = camera.frustum.left;
                    auto const screen_top = camera.frustum.top;
                    auto const screen_width = camera.frustum.width();
                    auto const screen_height = camera.frustum.height();
                    auto const camera_focal_length = get_focal_length(camera, raster_width, raster_height);
                    auto const camera_to_world_matrix = camera.view_matrix();

                    for (auto raster_y = 0*px; raster_y < raster_height; raster_y += 1*px)
                    {
                        for (auto raster_x = 0*px; raster_x < raster_width; raster_x += 1*px)
                        {
                            auto const ndc_x = (quantity_cast<float>(raster_x) + 0.5f) / quantity_cast<float>(raster_width);
                            auto const ndc_y = (quantity_cast<float>(raster_y) + 0.5f) / quantity_cast<float>(raster_height);
                            auto const screen_x = screen_left + ndc_x * screen_width;
                            auto const screen_y = screen_top - ndc_y * screen_height;
                            auto const primary_ray_direction = vector3 { screen_x, screen_y, camera_focal_length };

                            auto const primary_ray = transform_ray(
                                ray(zero_vector, primary_ray_direction), camera_to_world_matrix
                            );

                            auto const nearest_hit = find_nearest_hit(primary_ray, world);
                            auto const shading_color = (nearest_hit) ? shade(*nearest_hit, world) : world.environment;
                            auto const depth = (nearest_hit) ? clamp((nearest_hit->distance - world.min_depth) / (world.max_depth - world.min_depth), 0.0f, 1.0f) : 1.0f;
                            auto const apparent_color = mix(shading_color, world.depth_color, depth);

                            raster(raster_x, raster_y) = to_argb8888(apparent_color);
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
