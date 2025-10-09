//
// EPITECH PROJECT, 2025
// r-type
// File description:
// health component
//

#pragma once

#include <cstdint>
#include "flux/core/Serialization.hpp"

namespace component
{
    struct Health
    {
            uint8_t healthPoint = 100;
            REFLECT(healthPoint)
    };
} // namespace component
