//
// EPITECH PROJECT, 2025
// r-type
// File description:
// EndGame component
//

#pragma once

#include "flux/core/Serialization.hpp"

namespace component
{
    struct EndGame
    {
            int endX;

            EndGame(int endX = 0) : endX(endX) {}
            REFLECT(endX)
    };
} // namespace component
