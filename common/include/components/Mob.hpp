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
            int type;
            bool canShoot;
            float shootCooldown;
            float shootRate;
            float speed;
            bool isShooting;

            mob(int damage = 10, int type = 0, bool canShoot = true, float shootCooldown = 0.0f,
                float shootRate = 1.0f, float speed = 1, bool isShooting = false) :
                damage(damage), type(type), canShoot(canShoot), shootCooldown(shootCooldown), shootRate(shootRate),
                speed(speed), isShooting(isShooting) {};
            REFLECT(damage, type, canShoot, shootCooldown, shootRate, speed, isShooting)
    };
} // namespace component
