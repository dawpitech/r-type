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
    enum class ProjectileType : uint8_t
    {
        PLAYER = 0,
        MOB = 1
    };
    struct Projectile
    {
        ProjectileType type;
        float speed;

        Projectile() = default;
        Projectile(ProjectileType type, float speed = 5.0f): type(type), speed(speed) {}
        REFLECT(speed)
    };
} // namespace component
