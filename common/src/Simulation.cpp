/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#include <memory>
#ifdef IS_CLIENT
#include "network/datatype.hpp"
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
    this->_setupNetwork(serverIP, serverPort, hooks);
    this->_registerComponent();
    this->_createEntities();
    this->runSimulation(hooks, true);
}

void Simulation::runServerSimulation(std::optional<flux::runtimeHooks> hooks = std::nullopt)
{
    this->_registerComponent();
    this->_createEntities();
    this->runSimulation(std::move(hooks), false);
}

void Simulation::_registerComponent() {
    this->_ecs.registerComponentType<component::animation>("Animation");
    this->_ecs.registerComponentType<component::collider>("Collider");
    this->_ecs.registerComponentType<component::Health>("Health");
    this->_ecs.registerComponentType<component::mob>("Mob");
    this->_ecs.registerComponentType<component::Player>("Player");
    this->_ecs.registerComponentType<component::PlayerInput>("PlayerInput");
    this->_ecs.registerComponentType<component::Projectile>("Projectile");
    this->_ecs.registerComponentType<component::sprite>("Sprite");
    this->_ecs.registerComponentType<component::Transform>("Transform");
    this->_ecs.registerComponentType<component::Velocity>("Velocity");
    this->_ecs.registerComponentType<component::background>("Background");
    this->_ecs.Register<component::animation>();
    this->_ecs.Register<component::collider>();
    this->_ecs.Register<component::Health>();
    this->_ecs.Register<component::mob>();
    this->_ecs.Register<component::Player>();
    this->_ecs.Register<component::PlayerInput>();
    this->_ecs.Register<component::Projectile>();
    this->_ecs.Register<component::sprite>();
    this->_ecs.Register<component::Transform>();
    this->_ecs.Register<component::Velocity>();
    this->_ecs.Register<component::background>();
}

void Simulation::_createEntities() {
    flux::Entity background = this->_ecs.newEntity();
    const flux::Entity playerEntity = this->_ecs.newEntity();
    const flux::Entity mobEntity = this->_ecs.newEntity();
    const render::SpriteData& playerSprite = render::SDLManager::load("./assets/player.gif");
    const render::SpriteData& mobSprite = render::SDLManager::load("./assets/mob1.gif");
    const render::SpriteData& backgroundSprite = render::SDLManager::load("./assets/starfield2.jpg");

    this->_ecs.Add<component::background>(background, component::background(backgroundSprite.spriteMap, 100.0f));
    this->_ecs.Add<component::sprite>(background, component::sprite(backgroundSprite.texture));
    this->_ecs.Add<component::Transform>(background, component::Transform(0, 0, 0, 1, 1));
    this->_ecs.Add<component::sprite>(playerEntity, component::sprite(component::sprite(playerSprite.texture)));
    this->_ecs.Add<component::Player>(playerEntity);
    this->_ecs.Add<component::animation>(playerEntity, component::animation(playerSprite.spriteMap, true));
    this->_ecs.Add<component::PlayerInput>(playerEntity);
    this->_ecs.Add<component::Transform>(playerEntity, component::Transform(0, 0, 0, 1, 1));
    this->_ecs.Add<component::Velocity>(playerEntity, component::Velocity());
    this->_ecs.Add<component::Health>(playerEntity);
    this->_ecs.Add<component::collider>(
        playerEntity,
        component::collider(component::CollisionLayer::PLAYER,
                            component::CollisionLayer::MOB | component::CollisionLayer::MOB_PROJECTILE,
                            {0, 0, playerSprite.frameSize.x, playerSprite.frameSize.y}));
    this->_ecs.Add<component::mob>(mobEntity, component::mob(10, 0, true, 0.0f, 1.3f, 0.3));
    this->_ecs.Add<component::sprite>(mobEntity, component::sprite(mobSprite.texture));
    this->_ecs.Add<component::animation>(mobEntity, component::animation(mobSprite.spriteMap, true));
    this->_ecs.Add<component::Transform>(mobEntity, component::Transform(2000, 150, 0, 1, 1));
    this->_ecs.Add<component::Velocity>(mobEntity);
    this->_ecs.Add<component::collider>(
        mobEntity,
        component::collider(component::CollisionLayer::MOB,
                                  component::CollisionLayer::PLAYER |
                                      component::CollisionLayer::PLAYER_PROJECTILE,
                                  render::Rect{0, 0, mobSprite.frameSize.x, mobSprite.frameSize.y}));
    this->_ecs.Add<component::Health>(mobEntity, component::Health(100));
    const flux::Entity mobEntity2 = this->_ecs.newEntity();
    this->_ecs.Add<component::mob>(mobEntity2, component::mob(10, 0, true, 0.0f, 1.5f, 0.5));
    this->_ecs.Add<component::sprite>(mobEntity2, component::sprite(mobSprite.texture));
    this->_ecs.Add<component::animation>(mobEntity2, component::animation(mobSprite.spriteMap, true));
    this->_ecs.Add<component::Transform>(mobEntity2, component::Transform(1900, 300, 0, 1, 1));
    this->_ecs.Add<component::Velocity>(mobEntity2);
    this->_ecs.Add<component::collider>(
        mobEntity2,
        component::collider(component::CollisionLayer::MOB,
                            component::CollisionLayer::PLAYER | component::CollisionLayer::PLAYER_PROJECTILE,
                            render::Rect{0, 0, mobSprite.frameSize.x, mobSprite.frameSize.y}));
}

void Simulation::runSimulation(std::optional<flux::runtimeHooks> hooks, bool hasGUI)
{
    if (hasGUI)
        render::SDLManager::init();

    this->_ecs.registerSystem(InputSystem, InputSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(MovementSystem, MovementSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(MobSystem, MobSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(MobShootSystem, MobShootSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(ShootSystem, ShootSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(ProjectileSystem, ProjectileSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(CollisionSystem, CollisionSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(DamageSystem, DamageSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(HealthSystem, HealthSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(BackgroundSystem, BackgroundSystemView(this->_ecs), flux::systemType::RENDER);
    this->_ecs.registerSystem(AnimationSystem, AnimationSystemView(this->_ecs), flux::systemType::RENDER);

    if (hasGUI) {
        this->_ecs.registerSystem(RenderSystem, RenderSystemView(this->_ecs), flux::systemType::RENDER);
        if (hooks.has_value())
            hooks->hookBeforeLogic =
                flux::make_hook(render::SDLManager::handleEvent, std::ref(this->_ecs.getMasterRunState())),
            this->_ecs.handExecution(hooks);
    }
    else
        this->_ecs.handExecution(hooks);
}

void Simulation::_setupNetwork(const std::string& serverIp, uint16_t serverPort, std::optional<flux::runtimeHooks> &hooks)
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
    this->_networkUDPClient->attach<network::UDPSentInfo>([this](const network::UDPSentInfo& info) {
            std::cout << "Data received through udp are :" << info.serializedData << std::endl;
            this->_ecs.unserializeAllComponents(info.serializedData);
    });
    hooks->hooksNetwork = [this](flux::ECS &ecs) {
        this->_networkUDPClient->connect();
    };

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
