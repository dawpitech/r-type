/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#ifdef IS_CLIENT
    #include "network/TCPClient.hpp"
#endif

#include "Simulation.hpp"
#include "components/Animation.hpp"
#include "components/PlayerInput.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "sdlManager.hpp"
#include "spriteHandler.hpp"
#include "textureManager.hpp"
#include "utils/logger.hpp"

#include "systems/animationSystem.hpp"
#include "systems/inputSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/renderSystem.hpp"

void Simulation::runSimulationWithNetwork(const bool hasGUI, const std::string& serverIP, uint16_t serverPort)
{
    this->_setupNetwork(serverIP, serverPort);
    this->runSimulation(hasGUI);
}

void Simulation::runSimulation(const bool hasGUI)
{
    flux::ECS ecs;
    if (hasGUI)
        render::SDLManager::init();
    utils::TextureManager TextureManager;
    const sprite::SpriteHandler spriteHandler(TextureManager, render::SDLManager::getRenderer());

    const flux::Entity playerEntity = ecs.newEntity();

    ecs.Add<component::sprite>(
        playerEntity, component::sprite(spriteHandler.getPlayerSprite().texture, true));
    ecs.Add<component::animation>(playerEntity, component::animation(spriteHandler.getPlayerSprite().spriteMap));
    ecs.Add<component::PlayerInput>(playerEntity);
    ecs.Add<component::Transform>(playerEntity, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::Velocity>(playerEntity, component::Velocity());

    ecs.registerSystem(InputSystem, InputSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(AnimationSystem, AnimationSystemView(ecs), flux::systemType::RENDER);
    ecs.registerSystem(RenderSystem, RenderSystemView(ecs), flux::systemType::RENDER);

    flux::runtimeHooks hooks = {
        .hookBeforeLogic = flux::make_hook(render::SDLManager::handleEvent, std::ref(ecs.getMasterRunState())),
        .hookBeforeRender = []{ render::SDLManager::clear(); },
    };

    if (hasGUI)
        hooks.hookAfterRender = []{ render::SDLManager::render(); };

    render::SDLManager::setLastTime();
    ecs.handExecution(hooks);
}

#ifdef IS_CLIENT
void Simulation::_setupNetwork(const std::string& serverIp, uint16_t serverPort)
{
    utils::Logger::debug(std::format("Setting up network connection to {}:{}", serverIp, serverPort));
    this->_networkClient = std::make_unique<client::network::TCPClient>(serverIp, serverPort);

    try {
        this->_networkClient->connect();
        utils::Logger::debug("Network setup completed");
    } catch (const client::network::NetworkError& e) {
        utils::Logger::debug(std::format("Network connection failed: {}", e.what()));
        throw utils::BaseError("Failed to connect to server", "_setupNetwork");
    }
}
#endif
