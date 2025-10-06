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
#include "client/sdlManager.hpp"
#include "flux/core/flux.hpp"
#include "global/components/Transform.hpp"

void RenderSystem(flux::ECS& ecs)
{
    auto view = ecs.GenerateViewFromComponents<component::sprite, component::animation>();

    auto entities = ecs.QueryViewNotExclusive(view);

    for (flux::Entity entity : entities) {
        auto& sprite = ecs.GetComponent<component::sprite>(entity);
        auto& animation = ecs.GetComponent<component::animation>(entity);
        auto& transform = ecs.GetComponent<component::Transform>(entity);

        sprite.destRect.x = transform.pos.x;
        sprite.destRect.y = transform.pos.y;

        render::SDLManager::renderTexture(sprite.texture, sprite.srcRect, sprite.destRect);
    }
}
