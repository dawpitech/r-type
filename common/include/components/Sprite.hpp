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
        int startX;
        int startY;
        int width;
        int height;
        REFLECT(assetPath, startX, startY, width, height)

        Sprite() = default;
        Sprite(const std::string& assetPath)
            : assetPath(assetPath), startX(0), startY(0), width(-1), height(-1) {}
        Sprite(const std::string& assetPath, int startX, int startY, int width, int height)
            : assetPath(assetPath), startX(startX), startY(startY), width(width), height(height) {}
    };
} // namespace component
