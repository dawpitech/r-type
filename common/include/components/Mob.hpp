//
// EPITECH PROJECT, 2025
// r-type
// File description:
// mob
//

#pragma once

#include "flux/core/Serialization.hpp"
namespace component
{
    struct mob
    {
            int damage;
            float shootCooldown;
            float shootRate;
            int type;
            bool canShoot;

            mob(int damage, int type, bool canShoot = false, float shootCooldown = 0.0f, float shootRate = 0.0f) :
                damage(damage), type(type), canShoot(false), shootCooldown(shootCooldown), shootRate(shootRate) {};
            REFLECT(damage, shootCooldown, shootRate, type, canShoot)
    };
} // namespace component
