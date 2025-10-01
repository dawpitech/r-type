//
// EPITECH PROJECT, 2025
// r-type
// File description:
// sprite
//

#pragma once

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

namespace component
{
    struct sprite
    {
            SDL_Texture* texture = nullptr;

            SDL_FRect srcRect{0, 0, 0, 0};
            SDL_FRect destRect{100, 100, 66, 34};

            float angle = 0.0f;
            SDL_FPoint center = {0.5f, 0.5f};

            SDL_Color tint = {255, 255, 255, 255};

            SDL_Renderer* sdlRenderer = nullptr;

            int layer = 0;
            bool animation = false;
            bool visible = true;

            sprite(SDL_Texture* texture, bool animation, SDL_Renderer* renderer) :
                texture(texture), animation(animation), sdlRenderer(renderer) {};
    };
} // namespace component
