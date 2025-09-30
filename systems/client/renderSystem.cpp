//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>
#include "client/components/sprite.hpp"
#include "flux/core/flux.hpp"
#include "global/utils/error.hpp"

static void createTexture(const std::string &assetsPath, SDL_Texture *texture, SDL_Renderer *renderer) 
{
    if (texture == nullptr) {
        texture = IMG_LoadTexture(renderer, assetsPath.c_str());
    }
    if (texture == nullptr)
        throw utils::BaseError("failed to load texture", "createTexture");
}

void RenderSystem(flux::ECS& ecs, flux::Entity entity)
{
    if (!ecs.HasComponent<component::sprite>(entity))
        return;

    auto& sprite = ecs.GetComponent<component::sprite>(entity);

    if (!sprite.visible)
        return;

    createTexture(sprite.spritePath, sprite.texture, sprite.sdlRenderer);
    if (sprite.srcRect.w == 0 || sprite.srcRect.h == 0)
    {
        float w, h;
        SDL_GetTextureSize(sprite.texture, &w, &h);
        sprite.srcRect.w = static_cast<float>(w);
        sprite.srcRect.h = static_cast<float>(h);
        std::cout << "pos x = " << w << "and  pos y = " << h << std::endl;
        sprite.destRect.x = 100;
        sprite.destRect.y = 100;
        sprite.destRect.w = sprite.srcRect.w;
        sprite.destRect.h = sprite.srcRect.h;
    }
    SDL_RenderTexture(sprite.sdlRenderer, sprite.texture, &sprite.srcRect,
                      &sprite.destRect);
}
