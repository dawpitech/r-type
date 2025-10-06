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
#include "client/network/TCPClient.hpp"
#include "global/network/datatype.hpp"
#include "global/utils/logger.hpp"

void rTypeClient::Game::launchGame(const std::string& serverIp, uint16_t serverPort)
{
    this->_setupNetwork(serverIp, serverPort);
    
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
    ecs.Add<component::Velocity>(Entity, component::Velocity(50.0, 10.0));

    render::SDLManager::setLastTime();

    while (this->_running) {
        render::SDLManager::handleEvent(_running);
        InputSystem(ecs, Entity);
        MovementSystem(ecs, Entity);
        render::SDLManager::clear();
        AnimationSystem(ecs, Entity);
        RenderSystem(ecs, Entity);
        render::SDLManager::render();
        
        if (this->_networkClient) {
        }
    }
}

void rTypeClient::Game::_setupNetwork(const std::string& serverIp, uint16_t serverPort)
{
    utils::Logger::debug(std::format("Setting up network connection to {}:{}", serverIp, serverPort));
    this->_networkClient = std::make_unique<client::network::TCPClient>(serverIp, serverPort);
    
    try {
        this->_networkClient->connect();
        utils::Logger::debug("Network setup completed");
    } catch (const client::network::NetworkError& e) {
        utils::Logger::debug(std::format("Network connection failed: {}", e.what()));
        throw GameError("Failed to connect to server", "_setupNetwork");
    }
}
