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
#include "global/components/Transform.hpp"
#include "global/components/Velocity.hpp"
#include "global/systems/movementSystem.hpp"
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
    ecs.Add<component::Transform>(Entity, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::Velocity>(Entity);

    render::SDLManager::setLastTime();

    while (this->_running) {
        render::SDLManager::handleEvent(_running);
        InputSystem(ecs, Entity);
        MovementSystem(ecs, Entity);
        render::SDLManager::clear();
        AnimationSystem(ecs, Entity);
        RenderSystem(ecs, Entity);
        render::SDLManager::render();
    }
}
