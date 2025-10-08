//
// EPITECH PROJECT, 2025
// r-type
// File description:
// PlayerCollider.hpp
//

#pragma once

#include <cstdint>
#include "spriteHandler.hpp"

constexpr uint8_t NONE_BIT = 0;
constexpr uint8_t PLAYER_BIT = 1 << 0;
constexpr uint8_t MOB_BIT = 1 << 1;
constexpr uint8_t PLAYER_PROJECTILE_BIT = 1 << 2;
constexpr uint8_t MOB_PROJECTILE_BIT = 1 << 3;

namespace component
{
    enum CollisionLayer : uint8_t
    {
        NONE = NONE_BIT,
        PLAYER = PLAYER_BIT,
        MOB = MOB_BIT,
        PLAYER_PROJECTILE = PLAYER_PROJECTILE_BIT,
        MOB_PROJECTILE = MOB_PROJECTILE_BIT
    };
    struct collider
    {
            uint8_t layer;
            uint8_t mask;
            sprite::Rect rect;
            bool isActive;

            explicit collider(uint8_t layer, uint8_t mask, sprite::Rect rect, bool isActive = true) 
            : layer(layer), mask(mask), rect(rect), isActive(isActive) {}
    };
} // namespace component
