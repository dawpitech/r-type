//
// EPITECH PROJECT, 2025
// r-type
// File description:
// sprite
//

#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "flux/core/Serialization.hpp"

namespace component
{
    struct sprite
    {
            SDL_Texture* texture = nullptr;

            SDL_FRect srcRect = {0, 0, 0, 0};
            SDL_FRect destRect = {0, 0, 0, 0};

            float angle = 0.0f;
            SDL_FPoint center = {0.5f, 0.5f};

            SDL_Color tint = {255, 255, 255, 255};

            int layer = 0;
            bool visible = true;

            sprite() = default;
            sprite(SDL_Texture* texture, SDL_Color tint) : texture(texture), tint(tint) {};
            REFLECT()
    };
} // namespace component
