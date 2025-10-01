//
// EPITECH PROJECT, 2025
// r-type
// File description:
// game
//

#include "client/game.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "SDL3/SDL_events.h"
#include "client/components/animation.hpp"
#include "client/components/sprite.hpp"
#include "client/systems/animationSystem.hpp"
#include "client/systems/renderSystem.hpp"
#include "flux/core/flux.hpp"
#include "client/sdlManager.hpp"


void rTypeClient::Game::launchGame()
{
    flux::ECS ecs;
    render::SDLManager::init();
    utils::TextureManager TextureManager;
    sprite::SpriteHandler spriteHandler(TextureManager, render::SDLManager::getRenderer());

    const flux::Entity Entity = ecs.newEntity();

    ecs.Add<component::sprite>(Entity,
                               component::sprite(spriteHandler.getPlayerSprite().texture, true, render::SDLManager::getRenderer()));
    ecs.Add<component::animation>(Entity, component::animation(spriteHandler.getPlayerSprite().spriteMap));

    bool running = true;
    SDL_Event test_event;

    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }
        render::SDLManager::clear();
        AnimationSystem(ecs, Entity, deltaTime);
        RenderSystem(ecs, Entity);
        render::SDLManager::render();
    }
}
