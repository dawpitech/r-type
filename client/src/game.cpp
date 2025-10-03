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

#include "client/components/animation.hpp"
#include "client/components/sprite.hpp"
#include "client/game.hpp"
#include "client/sdlManager.hpp"
#include "client/systems/animationSystem.hpp"
#include "client/systems/renderSystem.hpp"
#include "flux/core/flux.hpp"
#include "global/components/playerInput.hpp"
#include "client/systems/inputSystem.hpp"

void rTypeClient::Game::launchGame()
{
    flux::ECS ecs;
    render::SDLManager::init();
    utils::TextureManager TextureManager;
    sprite::SpriteHandler spriteHandler(TextureManager, render::SDLManager::getRenderer());

    const flux::Entity Entity = ecs.newEntity();

    ecs.Add<component::sprite>(
        Entity, component::sprite(spriteHandler.getPlayerSprite().texture, true));
    ecs.Add<component::animation>(Entity, component::animation(spriteHandler.getPlayerSprite().spriteMap));
    ecs.Add<component::PlayerInput>(Entity);

    render::SDLManager::setLastTime();

    while (this->_running) {
        render::SDLManager::handleEvent(_running);
        InputSystem(ecs, Entity);
        render::SDLManager::clear();
        AnimationSystem(ecs, Entity);
        RenderSystem(ecs, Entity);
        render::SDLManager::render();
    }
}
