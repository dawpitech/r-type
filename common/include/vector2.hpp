//
// EPITECH PROJECT, 2025
// r-type
// File description:
// vector2
//

#pragma once

namespace utils
{
    template <typename T>
    struct Vector2
    {
            T x;
            T y;
            Vector2(T x, T y) : x(x), y(y) {}
    };
} // namespace utils
