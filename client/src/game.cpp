//
// EPITECH PROJECT, 2025
// r-type
// File description:
// game
//

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>

#include "client/components/animation.hpp"
#include "client/components/sprite.hpp"
#include "client/game.hpp"
#include "client/sdlManager.hpp"
#include "client/systems/animationSystem.hpp"
#include "client/systems/renderSystem.hpp"
#include "flux/core/flux.hpp"

void rTypeClient::Game::launchGame()
{
    flux::ECS ecs;
    render::SDLManager::init();
    utils::TextureManager TextureManager;
    sprite::SpriteHandler spriteHandler(TextureManager, render::SDLManager::getRenderer());

    const flux::Entity Entity = ecs.newEntity();

    ecs.Add<component::sprite>(
        Entity, component::sprite(spriteHandler.getPlayerSprite().texture, true, render::SDLManager::getRenderer()));
    ecs.Add<component::animation>(Entity, component::animation(spriteHandler.getPlayerSprite().spriteMap));

    SDL_Event test_event;

    uint32_t lastTime = SDL_GetTicks();

    while (this->_running) {
        uint32_t currentTime = SDL_GetTicks();
        float deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
                case SDL_EVENT_QUIT:
                    this->_running = false;
                    break;
            }
        }
        render::SDLManager::clear();
        AnimationSystem(ecs, Entity, deltaTime);
        RenderSystem(ecs, Entity);
        render::SDLManager::render();
    }
}
