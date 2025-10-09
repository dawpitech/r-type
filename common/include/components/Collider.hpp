//
// EPITECH PROJECT, 2025
// r-type
// File description:
// PlayerCollider.hpp
//

#pragma once

#include <cstdint>
#include "flux/core/Serialization.hpp"
#include "sdlManager.hpp"

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
            render::Rect rect;
            bool isActive;
            bool hasCollide;

            explicit collider(uint8_t layer, uint8_t mask, render::Rect rect, bool isActive = true,
                              bool hasCollide = false) :
                layer(layer), mask(mask), rect(rect), isActive(isActive), hasCollide(hasCollide)
            {}
            REFLECT(layer, mask, rect, isActive, hasCollide)
    };
} // namespace component
