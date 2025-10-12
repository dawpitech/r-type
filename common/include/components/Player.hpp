//
// EPITECH PROJECT, 2025
// r-type
// File description:
// player
//

#pragma once

#include "flux/core/Serialization.hpp"
namespace component
{
    struct Player
    {
            int score = 0;
            int lvl = 0;
            float shootCooldown = 0.0f;
            REFLECT(score, lvl, shootCooldown)
    };

} // namespace component
