/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#include "Simulation.hpp"
#include "components/Collider.hpp"
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
#include "systems/inputSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/projectileSystem.hpp"
#include "systems/shootSystem.hpp"
#include "systems/collisionSystem.hpp"
#include "systems/damageSystem.hpp"
#include "systems/healthSystem.hpp"

void Simulation::setInitialSimState(flux::ECS &ecs) {
  _registerComponent(ecs);
  _registerSystems(ecs);
  _createEntities(ecs);
}

void Simulation::_registerComponent(flux::ECS &ecs) {
  ecs.registerComponentType<component::Collider>("Collider");
  ecs.registerComponentType<component::Health>("Health");
  ecs.registerComponentType<component::Mob>("Mob");
  ecs.registerComponentType<component::Player>("Player");
  ecs.registerComponentType<component::PlayerInput>("PlayerInput");
  ecs.registerComponentType<component::Projectile>("Projectile");
  ecs.registerComponentType<component::Sprite>("Sprite");
  ecs.registerComponentType<component::Transform>("Transform");
  ecs.registerComponentType<component::Velocity>("Velocity");
  ecs.registerComponentType<component::NetworkIdentification>(
      "NetworkIdentification");
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
}

void Simulation::_registerSystems(flux::ECS &ecs) {
  ecs.registerSystem(InputHandlerSystem, InputHandlerSystemView(ecs),
                     flux::systemType::LOGIC);
  ecs.registerSystem(MovementSystem, MovementSystemView(ecs),
                     flux::systemType::LOGIC);
  // ecs.registerSystem(MobSystem, MobSystemView(ecs), flux::systemType::LOGIC);
  // ecs.registerSystem(MobShootSystem, MobShootSystemView(ecs),
  // flux::systemType::LOGIC);
  ecs.registerSystem(ShootSystem, ShootSystemView(ecs),
                     flux::systemType::LOGIC);
  ecs.registerSystem(ProjectileSystem, ProjectileSystemView(ecs),
                     flux::systemType::LOGIC);
  ecs.registerSystem(CollisionSystem, CollisionSystemView(ecs), flux::systemType::LOGIC);
  ecs.registerSystem(DamageSystem, DamageSystemView(ecs), flux::systemType::LOGIC);
  ecs.registerSystem(HealthSystem, HealthSystemView(ecs), flux::systemType::LOGIC);
 // ecs.registerSystem(AnimationSystem,
  // AnimationSystemView(ecs), flux::systemType::RENDER);
}

void Simulation::_createEntities(flux::ECS &ecs) {
  _createPlayer(ecs, PLAYER_TYPE::PLAYER_ONE);
  _createPlayer(ecs, PLAYER_TYPE::PLAYER_TWO);
  _createPlayer(ecs, PLAYER_TYPE::PLAYER_THREE);
  _createPlayer(ecs, PLAYER_TYPE::PLAYER_FOUR);
  map::MapLoader map(ecs);
  map.initializeGame();

  const flux::Entity background = ecs.newEntity();
  ecs.Add<component::Sprite>(background,
                             component::Sprite("./assets/starfield2.jpg", 0));
  ecs.Add<component::Transform>(background,
                                component::Transform(0, 0, 0, 1, 1));
}

void Simulation::_createPlayer(flux::ECS &ecs, PLAYER_TYPE type) {
  constexpr int startX = 0;
  constexpr int width = 66;
  constexpr int height = 34;
  int startY;

  switch (type) {
  case PLAYER_TYPE::PLAYER_ONE:
    startY = 0;
    break;
  case PLAYER_TYPE::PLAYER_TWO:
    startY = 34;
    break;
  case PLAYER_TYPE::PLAYER_THREE:
    startY = 68;
    break;
  default:
  case PLAYER_TYPE::PLAYER_FOUR:
    startY = 102;
    break;
  }

  const flux::Entity playerEntity = ecs.newEntity();
  ecs.Add<component::Sprite>(playerEntity,
                             component::Sprite("./assets/player.png", startX,
                                               startY, width, height, 1));
  ecs.Add<component::Player>(playerEntity);
  ecs.Add<component::PlayerInput>(playerEntity);
  ecs.Add<component::Transform>(playerEntity,
                                component::Transform(0, 0, 0, 1, 1));
  ecs.Add<component::Velocity>(playerEntity, component::Velocity());
  ecs.Add<component::Health>(playerEntity);
  ecs.Add<component::Collider>(
      playerEntity, component::Collider(component::CollisionLayer::PLAYER,
                                        component::CollisionLayer::WALL, 0, 0,
                                        width, height));
}
