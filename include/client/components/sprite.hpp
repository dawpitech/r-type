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
#include "SDL3/SDL_surface.h"

namespace component
{
    struct sprite
    {
            std::string spritePath;
            SDL_Texture* texture = nullptr;
            SDL_Surface* surface = nullptr;

            SDL_FRect srcRect = {0.0f, 0.0f, 0.0f, 0.0f};
            SDL_FRect destRect = {0.0f, 0.0f, 0.0f, 0.0f};

            float angle = 0.0f;
            SDL_FPoint center = {0.5f, 0.5f};

            SDL_Color tint = {255, 255, 255, 255};

            SDL_Renderer* sdlRenderer;

            int layer = 0;
            bool visible = true;

            explicit sprite(std::string spritePath, SDL_Renderer* sdlRenderer) :
                spritePath(std::move(spritePath)), sdlRenderer(sdlRenderer)
            {}
    };
} // namespace component
