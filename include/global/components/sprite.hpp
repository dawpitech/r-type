//
// EPITECH PROJECT, 2025
// r-type
// File description:
// sprite
//

#pragma once

#include <string>
#include <utility>
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

namespace component
{
    struct sprite
    {
            std::string spritePath;
            SDL_Texture* texture = nullptr;

            SDL_Rect srcRect = {0, 0, 0, 0};
            SDL_Rect destRect = {0, 0, 0, 0};

            float angle = 0.0f;
            SDL_FPoint center = {0.5f, 0.5f};

            SDL_Color tint = {255, 255, 255, 255};

            int layer = 0;
            bool visible = true;

            explicit sprite(std::string spritePath): spritePath(std::move(spritePath)) {}
    };
} // namespace component
