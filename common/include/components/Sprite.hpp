//
// EPITECH PROJECT, 2025
// r-type
// File description:
// sprite
//

#pragma once

#include <raylib-cpp.hpp>

#include "flux/core/Serialization.hpp"

namespace component
{
    struct Sprite {
        std::string assetPath;
        int zHeight;
        int startX;
        int startY;
        int width;
        int height;
        REFLECT(assetPath, startX, startY, width, height)

        Sprite() = default;
        Sprite(const std::string& assetPath, int zHeight)
            : assetPath(assetPath), startX(0), startY(0), width(-1), height(-1), zHeight(zHeight) {}
        Sprite(const std::string& assetPath, int startX, int startY, int width, int height, int zHeight)
            : assetPath(assetPath), startX(startX), startY(startY), width(width), height(height), zHeight(zHeight) {}
    };
} // namespace component
