//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include <iostream>

#include "flux/core/flux.hpp"

#if IS_CLIENT
    #include <SDL3/SDL_render.h>

    #include "components/Sprite.hpp"
    #include "sdlManager.hpp"
    #include "components/Transform.hpp"

    void RenderSystem(flux::ECS& ecs)
    {
        const auto view = ecs.GenerateViewFromComponents<component::sprite, component::Transform>();

        const auto& entities = ecs.QueryViewNotExclusive(view);

        for (const flux::Entity& entity : entities) {
            auto& sprite = ecs.GetComponent<component::sprite>(entity);
            const auto& transform = ecs.GetComponent<component::Transform>(entity);

            sprite.destRect.x = transform.pos.x;
            sprite.destRect.y = transform.pos.y;

            render::SDLManager::renderTexture(sprite.texture, sprite.srcRect, sprite.destRect);
        }
    }
#else
    void RenderSystem(flux::ECS&) { return; }
#endif
