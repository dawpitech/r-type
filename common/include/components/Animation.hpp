//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animation
//

#pragma once

#include <SDL3/SDL_rect.h>
#include <vector>
#include "sdlManager.hpp"

namespace component
{
    struct animation
    {
        std::vector<SDL_FRect> frames;
        int currentFrame = 0;
        float frameTime = 0.1f;
        float elapsedTime = 0.0f;

        bool loop = true;
        bool playing = true;
        bool isAnimate = true;

        explicit animation(const std::vector<render::Rect>& rect, bool isAnimate) : isAnimate(isAnimate)
        {
            for (auto it : rect) {
                SDL_FRect rect{it.srcX, it.srcY, it.srcw, it.srch};
                frames.emplace_back(rect);
            }
        }
    };
} // namespace component
