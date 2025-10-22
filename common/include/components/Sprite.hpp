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
    struct Sprite
    {
            std::string assetPath;
            int zHeight;
            float srcX, srcY, srcW, srcH;
            REFLECT(assetPath, srcX, srcY, srcW, srcH)

            Sprite() = default;
            Sprite(std::string assetPath, int zHeight) :
                assetPath(std::move(assetPath)), srcX(0), srcY(0), srcW(0), srcH(0), zHeight(zHeight)
            {}
            Sprite(std::string assetPath, float srcX, float srcY, float srcW, float srcH, int zHeight) :
                assetPath(std::move(assetPath)), srcX(srcX), srcY(srcY), srcW(srcW), srcH(srcH), zHeight(zHeight)
            {}
    };
} // namespace component
