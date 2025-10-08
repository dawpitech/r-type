/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#include "components/Collider.hpp"
#ifdef IS_CLIENT
#include "network/TCPClient.hpp"
#endif

#include "components/Animation.hpp"
#include "components/Mob.hpp"
#include "components/PlayerInput.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "sdlManager.hpp"
#include "Simulation.hpp"
#include "spriteHandler.hpp"
#include "textureManager.hpp"
#include "utils/logger.hpp"

#include "systems/animationSystem.hpp"
#include "systems/collisionSystem.hpp"
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
    const flux::Entity mobEntity = ecs.newEntity();

    ecs.Add<component::sprite>(playerEntity, component::sprite(spriteHandler.getPlayerSprite().texture));
    ecs.Add<component::animation>(playerEntity, component::animation(spriteHandler.getPlayerSprite().spriteMap, true));
    ecs.Add<component::PlayerInput>(playerEntity);
    ecs.Add<component::Transform>(playerEntity, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::Velocity>(playerEntity, component::Velocity());
    ecs.Add<component::mob>(mobEntity, component::mob(10, 0, false, 0.0f, 1.0f));
    ecs.Add<component::sprite>(mobEntity, component::sprite(spriteHandler.getMobSprite().texture));
    ecs.Add<component::animation>(mobEntity, component::animation(spriteHandler.getMobSprite().spriteMap, true));
    ecs.Add<component::Transform>(mobEntity, component::Transform(200, 400, 0, 1, 1));
    ecs.Add<component::Velocity>(mobEntity);
    ecs.Add<component::collider>(
        playerEntity,
        component::collider(
            component::CollisionLayer::PLAYER,
            component::CollisionLayer::MOB | component::CollisionLayer::MOB_PROJECTILE,
            {0, 0, spriteHandler.getPlayerSprite().frameSize.x, spriteHandler.getPlayerSprite().frameSize.y}));

    ecs.Add<component::collider>(
        mobEntity,
        component::collider(
            component::CollisionLayer::MOB,
            component::CollisionLayer::PLAYER | component::CollisionLayer::PLAYER_PROJECTILE,
            sprite::Rect{0, 0, spriteHandler.getMobSprite().frameSize.x, spriteHandler.getMobSprite().frameSize.y}));

    ecs.registerSystem(InputSystem, InputSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(AnimationSystem, AnimationSystemView(ecs), flux::systemType::RENDER);
    ecs.registerSystem(CollisionSystem, CollisionSystemView(ecs), flux::systemType::LOGIC);

    if (hasGUI) {
        ecs.registerSystem(RenderSystem, RenderSystemView(ecs), flux::systemType::RENDER);
        flux::runtimeHooks hooks = {
            .hookBeforeLogic = flux::make_hook(render::SDLManager::handleEvent, std::ref(ecs.getMasterRunState())),
            .hookBeforeRender = [] { render::SDLManager::clear(); },
        };

        hooks.hookAfterRender = [] { render::SDLManager::render(); };
        render::SDLManager::setLastTime();
        ecs.handExecution(hooks);
    }
    else
        ecs.handExecution();
}

#ifdef IS_CLIENT
void Simulation::_setupNetwork(const std::string& serverIp, uint16_t serverPort)
{
    utils::Logger::debug(std::format("Setting up network connection to {}:{}", serverIp, serverPort));
    this->_networkClient = std::make_unique<client::network::TCPClient>(serverIp, serverPort);

    try {
        this->_networkClient->connect();
        utils::Logger::debug("Network setup completed");
    }
    catch (const client::network::NetworkError& e) {
        utils::Logger::debug(std::format("Network connection failed: {}", e.what()));
        throw utils::BaseError("Failed to connect to server", "_setupNetwork");
    }
}
#endif
