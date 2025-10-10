//
// EPITECH PROJECT, 2025
// r-type
// File description:
// playerInput
//

#pragma once

#include "flux/core/Serialization.hpp"
namespace component
{
    struct PlayerInput
    {
            bool move_right = false;
            bool move_left = false;
            bool move_up = false;
            bool move_down = false;
            bool shoot = false;
            REFLECT(move_right, move_left, move_up, move_down, shoot)
    };
} // namespace component
