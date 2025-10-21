//
// EPITECH PROJECT, 2025
// r-type
// File description:
// vector4
//

#pragma once

#include <ostream>
namespace utils
{
    template <typename T>
    struct Vector4
    {
            T x;
            T y;
            T w;
            T h;
            explicit Vector4(T x = 0, T y = 0, T w = 0, T h = 0) : x(x), y(y), w(w), h(h) {}
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const utils::Vector4<T>& vec)
    {
        stream << vec.x << " " << vec.y << " " << vec.w << " " << vec.h;
        return stream;
    }

    template <typename T>
    std::istream& operator>>(std::istream& stream, utils::Vector4<T>& vec)
    {
        stream >> vec.x >> vec.y >> vec.w >> vec.h;
        return stream;
    }
} // namespace utils
