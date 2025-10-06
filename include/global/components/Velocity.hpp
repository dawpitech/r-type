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
            float x;
            float y;

            Velocity(float x, float y): x(x), y(y) {}
    };
} // namespace component
