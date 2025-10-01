//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include "client/components/animation.hpp"
#include "client/components/sprite.hpp"
#include "flux/core/flux.hpp"

void RenderSystem(flux::ECS& ecs, flux::Entity entity)
{
    if (!ecs.HasComponents<component::sprite, component::animation>(entity))
        return;

    auto& sprite = ecs.GetComponent<component::sprite>(entity);
    auto& animation = ecs.GetComponent<component::animation>(entity);

    if (!sprite.visible)
        return;

    SDL_RenderTexture(sprite.sdlRenderer, sprite.texture, &sprite.srcRect, &sprite.destRect);
}
