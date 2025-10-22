//
// EPITECH PROJECT, 2025
// pedro
// File description:
// map loader
//

#include "mapLoader.hpp"
#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/Serialization.hpp"
#include "vector4.hpp"
#include <LDtkLoader/Entity.hpp>
#include <cstdlib>
#include <iostream>
#include <raylib.h>

map::MapLoader::MapLoader(flux::ECS &ecs)
    : _ecs(ecs), _world(std::nullopt), _level(std::nullopt) {
  this->_ldtkFile = "./world.ldtk";
  try {
    this->_project.loadFromFile(this->_ldtkFile);
    this->_world = this->_project.getWorld();
    this->_level = this->_world->get().getLevel("Level_0");
  } catch (std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

void map::MapLoader::initializeGame() {
  this->_getMapTiles();
  this->_getMobs();
}

void map::MapLoader::_getMapTiles() {
  const auto &wall = this->_level->get().getLayer("Wall");

  for (const auto &tile : wall.allTiles()) {
    const auto &rect = tile.getTextureRect();
    const auto &pos = tile.getWorldPosition();

    utils::Vector4<float> srcRect(
        static_cast<float>(rect.x), static_cast<float>(rect.y),
        static_cast<float>(rect.width), static_cast<float>(rect.height));

    utils::Vector4<float> destRect(
        static_cast<float>(pos.x), static_cast<float>(pos.y),
        static_cast<float>(rect.width), static_cast<float>(rect.height));
    flux::Entity newTile = this->_ecs.newEntity();
    this->_ecs.AddOrReplace<component::Sprite>(
        newTile, component::Sprite(wall.getTileset().path, srcRect.x, srcRect.y,
                                   srcRect.w, srcRect.h, 1));
    this->_ecs.AddOrReplace<component::Transform>(
        newTile, component::Transform(destRect.x, destRect.y, 0, 1.0f, 1.0f));
    this->_ecs.AddOrReplace<component::Collider>(
        newTile, component::Collider(component::CollisionLayer::WALL,
                                     component::CollisionLayer::PLAYER, pos.x,
                                     pos.y, rect.width, rect.height));
  }
}

void map::MapLoader::_getMobs() {
  for (const ldtk::Entity &mob :
       this->_level->get().getLayer("Entities").getEntitiesByName("Mob")) {
    flux::Entity mobEntity = this->_ecs.newEntity();
    const auto &rect = mob.getTextureRect();

    const auto &pos = mob.getPosition();
    const auto &size = mob.getSize();
    this->_ecs.Add<component::Mob>(mobEntity, component::Mob());
    this->_ecs.Add<component::Sprite>(
        mobEntity, component::Sprite(mob.getTexturePath(), rect.x, rect.y,
                                     rect.width, rect.height, 1));
    this->_ecs.Add<component::Transform>(
        mobEntity, component::Transform(static_cast<float>(pos.x),
                                        static_cast<float>(pos.y), 0, 1, 1));
    this->_ecs.Add<component::Velocity>(mobEntity);
    this->_ecs.Add<component::Collider>(
        mobEntity,
        component::Collider(component::CollisionLayer::MOB,
                            component::CollisionLayer::PLAYER |
                                component::CollisionLayer::PLAYER_PROJECTILE,
                            pos.x, pos.y, rect.width, rect.height));
    this->_ecs.Add<component::Health>(mobEntity, component::Health(mob.getField<int>("Health").value()));
  }
}
