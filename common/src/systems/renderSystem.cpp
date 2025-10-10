//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include <iostream>
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
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& sprite = ecs.GetComponent<component::sprite>(entity);
        const auto& transform = ecs.GetComponent<component::Transform>(entity);

        SDL_FRect destRect = {transform.pos.x, transform.pos.y, sprite.srcRect.w * transform.scale.x,
                              sprite.srcRect.h * transform.scale.y};
        render::SDLManager::renderTexture(sprite.texture, sprite.srcRect, destRect);
    };
}
