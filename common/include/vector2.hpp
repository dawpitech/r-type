//
// EPITECH PROJECT, 2025
// r-type
// File description:
// vector2
//

#pragma once

#include <ostream>
namespace utils
{
    template <typename T>
    struct Vector2
    {
            T x;
            T y;
            explicit Vector2(T x = 0, T y = 0) : x(x), y(y) {}
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const utils::Vector2<T>& vec)
    {
        stream << vec.x << " " << vec.y;
        return stream;
    }

    template <typename T>
    std::istream& operator>>(std::istream& stream, utils::Vector2<T>& vec)
    {
        stream >> vec.x >> vec.y;
        return stream;
    }
} // namespace utils
