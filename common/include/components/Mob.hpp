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
            bool isShooting = false;
            float speed;

            mob(int damage, int type, bool canShoot = false, float shootCooldown = 0.0f, float shootRate = 1.0f,
                float speed = 1) :
                damage(damage), type(type), canShoot(canShoot), shootCooldown(shootCooldown), shootRate(shootRate),
                speed(speed) {};
            REFLECT(damage, shootCooldown, shootRate, type, canShoot, isShooting, speed)
    };
} // namespace component
