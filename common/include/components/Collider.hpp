//
// EPITECH PROJECT, 2025
// r-type
// File description:
// PlayerCollider.hpp
//

#pragma once

#include <cstdint>

#include "flux/core/Serialization.hpp"

constexpr uint8_t NONE_BIT = 0;
constexpr uint8_t PLAYER_BIT = 1 << 0;
constexpr uint8_t MOB_BIT = 1 << 1;
constexpr uint8_t PLAYER_PROJECTILE_BIT = 1 << 2;
constexpr uint8_t MOB_PROJECTILE_BIT = 1 << 3;
constexpr uint8_t WALL_BIT = 1 << 4;

namespace component
{
    enum CollisionLayer : int
    {
        NONE = NONE_BIT,
        PLAYER = PLAYER_BIT,
        MOB = MOB_BIT,
        PLAYER_PROJECTILE = PLAYER_PROJECTILE_BIT,
        MOB_PROJECTILE = MOB_PROJECTILE_BIT,
        WALL = WALL_BIT
    };

    struct Collider
    {
            int layer;
            int mask;
            float srcX, srcY, srcW, srcH;
            bool isActive;
            bool hasCollide;

            explicit Collider(int layer = 0, int mask = 0, float srcX = 0, float srcY = 0, float srcw = 0,
                              float srch = 0, bool isActive = true, bool hasCollide = false) :
                layer(layer), mask(mask), srcX(srcX), srcY(srcY), srcW(srcw), srcH(srch), isActive(isActive),
                hasCollide(hasCollide)
            {}
            REFLECT(layer, mask, srcX, srcY, srcW, srcH, isActive, hasCollide)
    };
} // namespace component
