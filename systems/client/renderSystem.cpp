//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include <iostream>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>

#include "client/components/animation.hpp"
#include "client/components/sprite.hpp"
#include "flux/core/flux.hpp"
#include "client/sdlManager.hpp"
#include "global/components/playerInput.hpp"

void RenderSystem(flux::ECS& ecs, flux::Entity entity)
{
    if (!ecs.HasComponents<component::sprite, component::animation>(entity))
        return;

    auto& sprite = ecs.GetComponent<component::sprite>(entity);
    auto& animation = ecs.GetComponent<component::animation>(entity);
    auto &playerInput = ecs.GetComponent<component::PlayerInput>(entity);

    if (!sprite.visible)
        return;

    if (playerInput.move_down) {
        sprite.destRect.y += 0.01;
    }
    if (playerInput.move_up) {
        sprite.destRect.y -= 0.01;
    }
    if (playerInput.move_right) {
        sprite.destRect.x += 0.01;
    }
    if (playerInput.move_left) {
        sprite.destRect.x -= 0.01;
    }
    render::SDLManager::renderTexture(sprite.texture, sprite.srcRect, sprite.destRect);
}
