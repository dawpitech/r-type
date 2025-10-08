//
// EPITECH PROJECT, 2025
// r-type
// File description:
// health component
//

#pragma once

namespace component
{
    struct Health
    {
            int healthPoint;

            explicit Health(int healthPoint = 1): healthPoint(healthPoint) {}
    };
} // namespace component
