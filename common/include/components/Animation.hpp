//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animation
//

#pragma once

#include <vector>

namespace sprite
{
    struct Rect {
        float srcX = 0.f;
        float srcY = 0.f;
        float destX = 0.f; // width
        float destY = 0.f; // height
    };
} // namespace sprite

namespace component
{
    struct animation
    {
        struct Frame {
            float srcX = 0.f;
            float srcY = 0.f;
            float srcW = 0.f;
            float srcH = 0.f;
        };

        std::vector<Frame> frames;
        int currentFrame = 0;
        const float frameTime = 10.0f;
        float elapsedTime = 0.0f;

        bool loop = true;
        bool playing = true;

        animation() = default;
        explicit animation(const std::vector<Frame>& rects) : frames(rects) {}
    };
} // namespace component
