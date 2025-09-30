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
#include "client/systems/renderSystem.hpp"
#include "flux/core/flux.hpp"
#include "client/systems/animationSystem.hpp"

rTypeClient::Game::Game() { this->_initSdl(); }

rTypeClient::Game::~Game()
{
    if (this->_sdlRenderer != nullptr)
        SDL_DestroyRenderer(this->_sdlRenderer);
    if (this->_window != nullptr)
        SDL_DestroyWindow(this->_window);
    SDL_Quit();
}

void rTypeClient::Game::launchGame()
{
    flux::ECS ecs;
    utils::TextureManager TextureManager;
    sprite::SpriteHandler spriteHandler(TextureManager, this->_sdlRenderer);

    const flux::Entity Entity = ecs.newEntity();

    ecs.Add<component::sprite>(Entity, component::sprite(spriteHandler.getPlayerSprite().texture, true, this->_sdlRenderer));
    ecs.Add<component::animation>(Entity, component::animation(spriteHandler.getPlayerSprite().spriteMap));

    bool running = true;
    SDL_Event test_event;

    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }
        SDL_RenderClear(this->_sdlRenderer);
        AnimationSystem(ecs, Entity, deltaTime);
        RenderSystem(ecs, Entity);
        SDL_RenderPresent(this->_sdlRenderer);
    }
}

void rTypeClient::Game::_initSdl()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        throw GameError("Failed to init sdl", "_initSdl");
    }
    this->_window = SDL_CreateWindow(this->_windowTitle.c_str(), this->_windowWidth, this->_windowHeight, 0);
    if (this->_window == nullptr) {
        throw GameError("Failed to create window", "_initSdl");
    }
    this->_sdlRenderer = SDL_CreateRenderer(this->_window, nullptr);
    if (this->_sdlRenderer == nullptr) {
        throw GameError("Failed to create Renderer", "_initSdl");
    }
}
