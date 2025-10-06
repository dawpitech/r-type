/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Velocity.hpp
*/

#pragma once

namespace component
{
    struct Velocity
    {
        float x = 0;
        float y = 0;

        Velocity() {}
        Velocity(const float x, const float y): x(x), y(y) {}
    };
} // namespace component
