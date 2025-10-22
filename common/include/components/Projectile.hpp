//
// EPITECH PROJECT, 2025
// r-type
// File description:
// projectile
//

#pragma once

#include <cstdint>
#include "flux/core/Serialization.hpp"

namespace component
{
    enum class ProjectileType : int
    {
        PLAYER = 0,
        MOB = 1
    };

    struct Projectile
    {
        int type;
        float speed;

        Projectile() = default;
        Projectile(int type, float speed = 5.0f): type(type), speed(speed) {}
        REFLECT(type,speed)
    };
} // namespace component
