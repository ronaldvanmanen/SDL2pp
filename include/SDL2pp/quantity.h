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

#define SDL2PP_QUANTITY_DECL(NAME, BASE)           \
    struct NAME##_tag {};                          \
    using NAME = sdl2::quantity<NAME##_tag, BASE>;

namespace sdl2
{
    template<typename Tag, typename T>
    class quantity
    {
    public:
        explicit quantity(T value);

        quantity(quantity<Tag, T> const& other);

        quantity<Tag, T> & operator=(quantity<Tag, T> const& other);

        quantity<Tag, T> & operator+=(quantity<Tag, T> const& other);

        quantity<Tag, T> & operator-=(quantity<Tag, T> const& other);

        quantity<Tag, T> & operator++();

        quantity<Tag, T> operator++(int);

        quantity<Tag, T> & operator--();

        quantity<Tag, T> operator--(int);

        operator T() const;

    private:
        T _value;
    };

    template<typename Tag, typename T>
    bool operator==(quantity<Tag, T> const& left, quantity<Tag, T> const& right)
    {
        return static_cast<T>(left) == static_cast<T>(right);
    }

    template<typename Tag, typename T>
    bool operator!=(quantity<Tag, T> const& left, quantity<Tag, T> const& right)
    {
        return !(left == right);
    }

    template<typename Tag, typename T>
    bool operator<(quantity<Tag, T> const& left, quantity<Tag, T> const& right)
    {
        return static_cast<T>(left) < static_cast<T>(right);
    }

    template<typename Tag, typename T>
    bool operator>(quantity<Tag, T> const& left, quantity<Tag, T> const& right)
    {
        return right < left;
    }

    template<typename Tag, typename T>
    bool operator<=(quantity<Tag, T> const& left, quantity<Tag, T> const& right)
    {
        return !(right < left);
    }

    template<typename Tag, typename T>
    bool operator>=(quantity<Tag, T> const& left, quantity<Tag, T> const& right)
    {
        return !(left < right);
    }

    template<typename Tag, typename T>
    quantity<Tag, T>::quantity(T value)
    : _value(value)
    { }

    template<typename Tag, typename T>
    quantity<Tag, T>::quantity(quantity<Tag, T> const& other)
    : _value(other._value)
    { }

    template<typename Tag, typename T>
    quantity<Tag, T> &
    quantity<Tag, T>::operator=(quantity<Tag, T> const& other)
    {
        if (*this != other)
        {
            _value = other._value;
        }
        return *this;
    }

    template<typename Tag, typename T>
    quantity<Tag, T> &
    quantity<Tag, T>::operator+=(quantity<Tag, T> const& other)
    {
        _value += other._value;
        return *this;
    }

    template<typename Tag, typename T>
    quantity<Tag, T> &
    quantity<Tag, T>::operator-=(quantity<Tag, T> const& other)
    {
        _value -= other._value;
        return *this;
    }

    template<typename Tag, typename T>
    quantity<Tag, T> &
    quantity<Tag, T>::operator++()
    {
        ++_value;
        return *this;
    }

    template<typename Tag, typename T>
    quantity<Tag, T>
    quantity<Tag, T>::operator++(int)
    {
        return quantity<Tag, T>(_value++);
    }

    template<typename Tag, typename T>
    quantity<Tag, T> &
    quantity<Tag, T>::operator--()
    {
        --_value;
        return *this;
    }

    template<typename Tag, typename T>
    quantity<Tag, T>
    quantity<Tag, T>::operator--(int)
    {
        return quantity<Tag, T>(_value--);
    }

    template<typename Tag, typename T>
    quantity<Tag, T>::operator T() const
    {
        return _value;
    }
}
