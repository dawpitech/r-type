/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#ifdef IS_CLIENT
#include "network/TCPClient.hpp"
#endif

#include "components/Projectile.hpp"
#include "components/Player.hpp"
#include "components/Collider.hpp"
#include "components/Animation.hpp"
#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/PlayerInput.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "flux/core/Serialization.hpp"
#include "sdlManager.hpp"
#include "Simulation.hpp"
#include "spriteHandler.hpp"
#include "utils/logger.hpp"
#include "utils/error.hpp"

#include "systems/animationSystem.hpp"
#include "systems/collisionSystem.hpp"
#include "systems/damageSystem.hpp"
#include "systems/healthSystem.hpp"
#include "systems/inputSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/projectileSystem.hpp"
#include "systems/renderSystem.hpp"
#include "systems/shootSystem.hpp"
#include "systems/mobSystem.hpp"
#include "systems/mobShootSystem.hpp"

void Simulation::runSimulationWithNetwork(std::optional<flux::runtimeHooks> hooks, const bool hasGUI,
                                          const std::string& serverIP, uint16_t serverPort)
{
    this->_setupNetwork(serverIP, serverPort);
    this->runSimulation(hooks, hasGUI);
}

void Simulation::runSimulation(std::optional<flux::runtimeHooks> hooks, const bool hasGUI)
{
    flux::ECS ecs;
    if (hasGUI)
        render::SDLManager::init();

    const flux::Entity playerEntity = ecs.newEntity();
    const flux::Entity mobEntity = ecs.newEntity();
    const render::SpriteData& playerSprite = render::SDLManager::load("./assets/player.gif");
    const render::SpriteData& mobSprite = render::SDLManager::load("./assets/mob1.gif");

    // ecs.registerComponentType<component::sprite>("Sprite");
    // ecs.registerComponentType<component::Player>("Player");
    // ecs.registerComponentType<component::animation>("Animation");
    // ecs.registerComponentType<component::PlayerInput>("PlayerInput");
    // ecs.registerComponentType<component::Transform>("Transform");
    // ecs.registerComponentType<component::Velocity>("Velocity");
    ecs.registerComponentType<component::Health>("Health");
    // ecs.registerComponentType<component::collider>("Collider");
    // ecs.registerComponentType<component::mob>("Mob");
    // ecs.registerComponentType<component::Projectile>("Projectile");
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
    ecs.Add<component::mob>(mobEntity, component::mob(10, 0, true, 0.0f, 1.0f));
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
    ecs.Add<component::mob>(mobEntity2, component::mob(10, 0, true, 0.0f, 0.8f));
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

    ecs.registerSystem(InputSystem, InputSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MobSystem, MobSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MobShootSystem, MobShootSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(ShootSystem, ShootSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(ProjectileSystem, ProjectileSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(CollisionSystem, CollisionSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(DamageSystem, DamageSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(HealthSystem, HealthSystemView(ecs), flux::systemType::LOGIC);
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
