/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Velocity.hpp
*/

#pragma once

#include "flux/core/Serialization.hpp"
namespace component
{
    struct Velocity
    {
        float x = 0;
        float y = 0;

        Velocity() = default;
        Velocity(const float x, const float y): x(x), y(y) {}
        REFLECT(x, y)
    };
} // namespace component
