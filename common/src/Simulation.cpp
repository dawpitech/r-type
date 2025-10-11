/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#include <memory>
#include "network/datatype.hpp"
#ifdef IS_CLIENT
#include "network/TCPClient.hpp"
#include "network/UDPClient.hpp"
#endif

#include "components/Animation.hpp"
#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Projectile.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/background.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "flux/core/Serialization.hpp"
#include "sdlManager.hpp"
#include "Simulation.hpp"

#include <utility>
#include "spriteHandler.hpp"
#include "utils/error.hpp"
#include "utils/logger.hpp"

#include "systems/animationSystem.hpp"
#include "systems/backgroundSystem.hpp"
#include "systems/collisionSystem.hpp"
#include "systems/damageSystem.hpp"
#include "systems/healthSystem.hpp"
#include "systems/inputSystem.hpp"
#include "systems/mobShootSystem.hpp"
#include "systems/mobSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/projectileSystem.hpp"
#include "systems/renderSystem.hpp"
#include "systems/shootSystem.hpp"

void Simulation::runClientSimulation(std::optional<flux::runtimeHooks> hooks, const std::string& serverIP,
                                     uint16_t serverPort)
{
    flux::ECS ecs;
    this->_setupNetwork(serverIP, serverPort);
    this->_registerComponent(ecs);
    this->_createEntities(ecs);
    this->runSimulation(ecs, hooks, true);
}

void Simulation::runServerSimulation(std::optional<flux::runtimeHooks> hooks = std::nullopt)
{
    flux::ECS ecs;
    this->_registerComponent(ecs);
    this->_createEntities(ecs);
    this->runSimulation(ecs, std::move(hooks), false);
}

void Simulation::_registerComponent(flux::ECS &ecs) {
    ecs.registerComponentType<component::animation>("Animation");
    ecs.registerComponentType<component::collider>("Collider");
    ecs.registerComponentType<component::Health>("Health");
    ecs.registerComponentType<component::mob>("Mob");
    ecs.registerComponentType<component::Player>("Player");
    ecs.registerComponentType<component::PlayerInput>("PlayerInput");
    ecs.registerComponentType<component::Projectile>("Projectile");
    ecs.registerComponentType<component::sprite>("Sprite");
    ecs.registerComponentType<component::Transform>("Transform");
    ecs.registerComponentType<component::Velocity>("Velocity");
    ecs.registerComponentType<component::background>("Background");
}

void Simulation::_createEntities(flux::ECS &ecs) {
    flux::Entity background = ecs.newEntity();
    const flux::Entity playerEntity = ecs.newEntity();
    const flux::Entity mobEntity = ecs.newEntity();
    const render::SpriteData& playerSprite = render::SDLManager::load("./assets/player.gif");
    const render::SpriteData& mobSprite = render::SDLManager::load("./assets/mob1.gif");
    const render::SpriteData& backgroundSprite = render::SDLManager::load("./assets/starfield2.jpg");

    ecs.Add<component::background>(background, component::background(backgroundSprite.spriteMap, 100.0f));
    ecs.Add<component::sprite>(background, component::sprite(backgroundSprite.texture));
    ecs.Add<component::Transform>(background, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::sprite>(playerEntity, component::sprite(component::sprite(playerSprite.texture)));
    ecs.Add<component::Player>(playerEntity);
    ecs.Add<component::animation>(playerEntity, component::animation(playerSprite.spriteMap, true));
    ecs.Add<component::PlayerInput>(playerEntity);
    ecs.Add<component::Transform>(playerEntity, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::Velocity>(playerEntity, component::Velocity());
    ecs.Add<component::Health>(playerEntity);
    ecs.Add<component::collider>(
        playerEntity,
        component::collider(component::CollisionLayer::PLAYER,
                            component::CollisionLayer::MOB | component::CollisionLayer::MOB_PROJECTILE,
                            {0, 0, playerSprite.frameSize.x, playerSprite.frameSize.y}));
    ecs.Add<component::mob>(mobEntity, component::mob(10, 0, true, 0.0f, 1.3f, 0.3));
    ecs.Add<component::sprite>(mobEntity, component::sprite(mobSprite.texture));
    ecs.Add<component::animation>(mobEntity, component::animation(mobSprite.spriteMap, true));
    ecs.Add<component::Transform>(mobEntity, component::Transform(2000, 150, 0, 1, 1));
    ecs.Add<component::Velocity>(mobEntity);
    ecs.Add<component::collider>(
        mobEntity,
        component::collider(component::CollisionLayer::MOB,
                            component::CollisionLayer::PLAYER | component::CollisionLayer::PLAYER_PROJECTILE,
                            render::Rect{0, 0, mobSprite.frameSize.x, mobSprite.frameSize.y}));
    ecs.Add<component::Health>(mobEntity, component::Health(100));
    const flux::Entity mobEntity2 = ecs.newEntity();
    ecs.Add<component::mob>(mobEntity2, component::mob(10, 0, true, 0.0f, 1.5f, 0.5));
    ecs.Add<component::sprite>(mobEntity2, component::sprite(mobSprite.texture));
    ecs.Add<component::animation>(mobEntity2, component::animation(mobSprite.spriteMap, true));
    ecs.Add<component::Transform>(mobEntity2, component::Transform(1900, 300, 0, 1, 1));
    ecs.Add<component::Velocity>(mobEntity2);
    ecs.Add<component::collider>(
        mobEntity2,
        component::collider(component::CollisionLayer::MOB,
                            component::CollisionLayer::PLAYER | component::CollisionLayer::PLAYER_PROJECTILE,
                            render::Rect{0, 0, mobSprite.frameSize.x, mobSprite.frameSize.y}));
    ecs.Add<component::Health>(mobEntity2, component::Health(100));
    flux::Entity newEntity = ecs.newEntity();
    ecs.Add<component::Projectile>(newEntity, component::Projectile(component::ProjectileType::PLAYER));
}

void Simulation::runSimulation(flux::ECS& ecs, std::optional<flux::runtimeHooks> hooks, bool hasGUI)
{
    if (hasGUI)
        render::SDLManager::init();

    ecs.registerSystem(InputSystem, InputSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MobSystem, MobSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MobShootSystem, MobShootSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(ShootSystem, ShootSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(ProjectileSystem, ProjectileSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(CollisionSystem, CollisionSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(DamageSystem, DamageSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(HealthSystem, HealthSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(BackgroundSystem, BackgroundSystemView(ecs), flux::systemType::RENDER);
    ecs.registerSystem(AnimationSystem, AnimationSystemView(ecs), flux::systemType::RENDER);

    if (hasGUI) {
        ecs.registerSystem(RenderSystem, RenderSystemView(ecs), flux::systemType::RENDER);
        if (hooks.has_value())
            hooks->hookBeforeLogic =
                flux::make_hook(render::SDLManager::handleEvent, std::ref(ecs.getMasterRunState())),
            ecs.handExecution(hooks);
    }
    else
        ecs.handExecution(hooks);
}

void Simulation::_setupNetwork(const std::string& serverIp, uint16_t serverPort)
{
#ifdef IS_CLIENT
    utils::Logger::debug(std::format("Setting up network connection to {}:{}", serverIp, serverPort));
    this->_networkUDPClient = std::make_unique<client::network::UDPClient>(serverIp, serverPort);
    this->_networkTCPClient =
        std::make_unique<client::network::TCPClient>(serverIp, serverPort, this->_networkUDPClient->getLocalPort());

    this->_networkTCPClient->attach<network::ClientTCPSentInfo>(
        [this](const network::ClientTCPSentInfo& info)
        {
            this->gameInfo = info;
        });
    try {
        this->_networkTCPClient->connect();
        utils::Logger::debug("Network setup completed");
    }
    catch (const client::network::NetworkError& e) {
        utils::Logger::debug(std::format("Network connection failed: {}", e.what()));
        throw utils::BaseError("Failed to connect to server", "_setupNetwork");
    }
#endif
}
