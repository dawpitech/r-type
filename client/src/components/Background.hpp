//
// EPITECH PROJECT, 2025
// r-type
// File description:
// background
//

#pragma once

#include <utility>
#include <vector>

#include "flux/core/Serialization.hpp"

namespace component
{
    struct background
    {
        float speed;
        std::vector<render::Rect> frames;
        int currentFrame;
        float elapsedTime;
        float scrollOffset;

        background() = default;
        background(std::vector<render::Rect> rect, float speed = 1, int currentFrame = 0, float elapsedTime = 0.0f,
                   float scrollOffset = 0.0f) :
            speed(speed), currentFrame(currentFrame), elapsedTime(elapsedTime), scrollOffset(scrollOffset),
            frames(std::move(rect))
        {}
        REFLECT(speed, currentFrame, elapsedTime, scrollOffset);
    };

} // namespace component
