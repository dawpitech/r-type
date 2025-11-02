/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#include "Simulation.hpp"
#include <string>
#include "components/Camera.hpp"
#include "components/Collider.hpp"
#include "components/EndGame.hpp"
#include "components/FixOnScreen.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/NetworkIdentification.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Projectile.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/Serialization.hpp"
#include "flux/core/flux.hpp"
#include "mapLoader.hpp"
#include "systems/cameraSystem.hpp"
#include "systems/collisionSystem.hpp"
#include "systems/damageSystem.hpp"
#include "systems/fixOnScreenSystems.hpp"
#include "systems/healthSystem.hpp"
#include "systems/inputSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/projectileSystem.hpp"
#include "systems/shootSystem.hpp"
#include "systems/scoreSystem.hpp"

#include <systems/luaSystem.hpp>

constexpr float MAP_WIDTH = 800;
constexpr float MAP_HEIGHT = 450;

void Simulation::setInitialClientSimState(flux::ECS& ecs, std::string level)
{
    ecs.reset();
    map::MapLoader map(ecs);
    _registerComponent(ecs);
    if (level == "Menu") {
        map.loadMenu();
        _registerMenuSystems(ecs);
    }
    if (level == "Level_0") {
        map.loadGame();
        _createPlayer(ecs, PLAYER_TYPE::PLAYER_ONE);
        _createPlayer(ecs, PLAYER_TYPE::PLAYER_TWO);
        _createPlayer(ecs, PLAYER_TYPE::PLAYER_THREE);
        _createPlayer(ecs, PLAYER_TYPE::PLAYER_FOUR);
        _registerGameSystems(ecs);
    }
}

void Simulation::setInitialServerSimState(flux::ECS& ecs, std::string level)
{
    ecs.reset();
    map::MapLoader map(ecs);
    _registerComponent(ecs);
    if (level == "Level_0") {
        map.loadGame();
        _registerGameSystems(ecs);
    }
    _createPlayer(ecs, PLAYER_TYPE::PLAYER_ONE);
    _createPlayer(ecs, PLAYER_TYPE::PLAYER_TWO);
    _createPlayer(ecs, PLAYER_TYPE::PLAYER_THREE);
    _createPlayer(ecs, PLAYER_TYPE::PLAYER_FOUR);
}

void Simulation::_registerComponent(flux::ECS& ecs)
{
    ecs.registerComponentType<component::Collider>("Collider");
    ecs.registerComponentType<component::Health>("Health");
    ecs.registerComponentType<component::Mob>("Mob");
    ecs.registerComponentType<component::Player>("Player");
    ecs.registerComponentType<component::PlayerInput>("PlayerInput");
    ecs.registerComponentType<component::Projectile>("Projectile");
    ecs.registerComponentType<component::Sprite>("Sprite");
    ecs.registerComponentType<component::Transform>("Transform");
    ecs.registerComponentType<component::Velocity>("Velocity");
    ecs.registerComponentType<component::NetworkIdentification>("NetworkIdentification");
    ecs.registerComponentType<component::Camera>("Camera");
    ecs.registerComponentType<component::FixOnScreen>("FixOnScreen");
    ecs.registerComponentType<component::EndGame>("EndGame");
    ecs.Register<component::Collider>();
    ecs.Register<component::Health>();
    ecs.Register<component::Mob>();
    ecs.Register<component::Player>();
    ecs.Register<component::PlayerInput>();
    ecs.Register<component::Projectile>();
    ecs.Register<component::Sprite>();
    ecs.Register<component::Transform>();
    ecs.Register<component::Velocity>();
    ecs.Register<component::NetworkIdentification>();
    ecs.Register<component::Camera>();
    ecs.Register<component::FixOnScreen>();
    ecs.Register<component::EndGame>();
}

void Simulation::_registerGameSystems(flux::ECS& ecs)
{
    ecs.registerSystem(InputHandlerSystem, InputHandlerSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(FixOnScreenSystem, FixOnScreenSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(MobSystem, MobSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(MobShootSystem, MobShootSystemView(ecs),
    // flux::systemType::LOGIC);
    ecs.registerSystem(ShootSystem, ShootSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(ProjectileSystem, ProjectileSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(CollisionSystem, CollisionSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(DamageSystem, DamageSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(ScoreSystem, ScoreSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(HealthSystem, HealthSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(CameraSystem, CameraSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(LuaSystem, LuaSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(AnimationSystem,
    // AnimationSystemView(ecs), flux::systemType::RENDER);
}

void Simulation::_registerMenuSystems(flux::ECS& ecs)
{
    // ecs.registerSystem(InputHandlerSystem, InputHandlerSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(MobSystem, MobSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(MobShootSystem, MobShootSystemView(ecs),
    // flux::systemType::LOGIC);
    // ecs.registerSystem(ShootSystem, ShootSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(ProjectileSystem, ProjectileSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(CollisionSystem, CollisionSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(DamageSystem, DamageSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(HealthSystem, HealthSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(CameraSystem, CameraSystemView(ecs), flux::systemType::LOGIC);
    // ecs.registerSystem(AnimationSystem,
    // AnimationSystemView(ecs), flux::systemType::RENDER);
}

void Simulation::_createPlayer(flux::ECS& ecs, PLAYER_TYPE type)
{
    constexpr int startX = 2;
    constexpr int width = 64;
    constexpr int height = 29;
    int startY;

    switch (type) {
        case PLAYER_TYPE::PLAYER_ONE:
            startY = 5;
            break;
        case PLAYER_TYPE::PLAYER_TWO:
            startY = 39;
            break;
        case PLAYER_TYPE::PLAYER_THREE:
            startY = 73;
            break;
        default:
        case PLAYER_TYPE::PLAYER_FOUR:
            startY = 107;
            break;
    }

    const flux::Entity playerEntity = ecs.newEntity();
    ecs.Add<component::Sprite>(playerEntity,
                               component::Sprite("./assets/player.png", startX, startY, width, height, 1));
    ecs.Add<component::Player>(playerEntity);
    ecs.Add<component::PlayerInput>(playerEntity);
    ecs.Add<component::Transform>(playerEntity, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::Velocity>(playerEntity, component::Velocity());
    ecs.Add<component::Health>(playerEntity);
    ecs.Add<component::Collider>(
        playerEntity,
        component::Collider(component::CollisionLayer::PLAYER, component::CollisionLayer::WALL, 0, 0, width, height));
    ecs.Add<component::FixOnScreen>(playerEntity, component::FixOnScreen());
}
