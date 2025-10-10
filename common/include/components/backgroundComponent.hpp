//
// EPITECH PROJECT, 2025
// r-type
// File description:
// background
//

#pragma once

#include <iostream>
#include <vector>
#include "sdlManager.hpp"
namespace component
{
    struct background
    {
            float speed;
            std::vector<render::Rect> frames;
            int currentFrame = 0;
            float elapsedTime = 0.0f;
            float scrollOffset = 0.0f;

            explicit background(const std::vector<render::Rect>& rect, float speed = 1) : speed(speed)
            {
                for (auto it : rect) {
                    render::Rect rect{it.srcX, it.srcY, it.srcw, it.srch};
                    this->frames.emplace_back(rect);
                }
            }
    };

} // namespace component
