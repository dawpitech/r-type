//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include <SDL3/SDL_render.h>
#include <vector>

#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "flux/core/flux.hpp"
#include "sdlManager.hpp"

flux::View RenderSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::sprite, component::Transform>();
}

void RenderSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (const auto& entity : entities) {
        auto& sprite = ecs.GetComponent<component::sprite>(entity);
        const auto& transform = ecs.GetComponent<component::Transform>(entity);

        sprite.destRect.x = transform.pos.x;
        sprite.destRect.y = transform.pos.y;

        render::SDLManager::renderTexture(sprite.texture, sprite.srcRect, sprite.destRect);
    };
}
