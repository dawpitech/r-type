//
// EPITECH PROJECT, 2025
// r-type
// File description:
// game
//

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <iostream>
#include "client/game.hpp"
#include "flux/core/flux.hpp"
#include "global/components/Transform.hpp"
#include "global/components/health.hpp"
#include "global/systems/healthSystem.hpp"

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

    const flux::Entity windowEntity = ecs.newEntity();
    const flux::Entity e1 = ecs.newEntity();
    const flux::Entity e2 = ecs.newEntity();

    ecs.Add<component::Transform>(e2, component::Transform{1, 3, 0, 1.0, 1.0});
    ecs.Add<component::Health>(e1);
    std::cout << static_cast<int>(
                     ecs.GetComponent<component::Health>(e1).healthPoint)
              << std::endl;
    HealthSystem(ecs, e1);
    std::cout << static_cast<int>(
                     ecs.GetComponent<component::Health>(e1).healthPoint)
              << std::endl;
    while (true) {
        SDL_RenderPresent(this->_sdlRenderer);
    }
}

void rTypeClient::Game::_initSdl()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        throw GameError("Failed to init sdl", "_initSdl");
    }
    this->_window = SDL_CreateWindow(
        this->_windowTitle.c_str(), this->_windowWidth, this->_windowHeight, 0);
    if (this->_window == nullptr) {
        throw GameError("Failed to create window", "_initSdl");
    }
    this->_sdlRenderer = SDL_CreateRenderer(this->_window, nullptr);
    if (this->_sdlRenderer == nullptr) {
        throw GameError("Failed to create Renderer", "_initSdl");
    }
}
