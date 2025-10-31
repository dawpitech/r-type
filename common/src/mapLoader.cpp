//
// EPITECH PROJECT, 2025
// pedro
// File description:
// map loader
//

#include "mapLoader.hpp"
#include <cstdlib>
#include <iostream>
#include <LDtkLoader/Entity.hpp>
#include <raylib.h>
#include "components/Camera.hpp"
#include "components/Collider.hpp"
#include "components/EndGame.hpp"
#include "components/FixOnScreen.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "components/score.hpp"
#include "flux/core/Serialization.hpp"
#include "vector4.hpp"

constexpr float MAP_WIDTH = 800;
constexpr float MAP_HEIGHT = 450;

map::MapLoader::MapLoader(flux::ECS& ecs) : _ecs(ecs), _world(std::nullopt), _level(std::nullopt)
{
    this->_ldtkFile = "./world.ldtk";
    try {
        this->_project.loadFromFile(this->_ldtkFile);
        this->_world = this->_project.getWorld();
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void map::MapLoader::loadGame()
{
    this->_level = this->_world->get().getLevel("Level_0");
    this->_setMapTiles();
    this->_setMobs();
    this->_setBackground();
    this->_setCamera();
}

void map::MapLoader::loadMenu()
{
    this->_level = this->_world->get().getLevel("Menu");
    this->_setBackground();
    this->_setIdol();
    this->_setCamera();
}

void map::MapLoader::_setIdol()
{
    for (const ldtk::Entity& idol : this->_level->get().getLayer("Entities").getEntitiesByName("Idol")) {

        const auto& rect = idol.getTextureRect();
        const auto& pos = idol.getPosition();
        const auto& size = idol.getSize();
        float scaleX = static_cast<float>(size.x) / static_cast<float>(rect.width);
        float scaleY = static_cast<float>(size.y) / static_cast<float>(rect.height);

        flux::Entity idolEntity = this->_ecs.newEntity();
        this->_ecs.Add<component::Sprite>(
            idolEntity, component::Sprite(idol.getTexturePath(), rect.x, rect.y, rect.width, rect.height, 1));
        this->_ecs.Add<component::Transform>(idolEntity, component::Transform(pos.x, pos.y, 0, scaleX, scaleY));
    }
}

void map::MapLoader::_setBackground()
{
    auto background = this->_level->get().getBgImage();
    auto rect = background.crop;

    int levelWidth = this->_level->get().size.x;
    int levelHeight = this->_level->get().size.y;

    int tilesX = std::ceil(static_cast<float>(levelWidth) / rect.width);
    int tilesY = std::ceil(static_cast<float>(levelHeight) / rect.height);

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            flux::Entity backgroundEntity = this->_ecs.newEntity();

            float tileX = x * rect.width;
            float tileY = y * rect.height;

            this->_ecs.Add<component::Sprite>(
                backgroundEntity,
                component::Sprite(background.path.c_str(), rect.x, rect.y, rect.width, rect.height, 0));
            this->_ecs.Add<component::Transform>(backgroundEntity, component::Transform(tileX, tileY, 0, 1, 1));
        }
    }
}

void map::MapLoader::_setMapTiles()
{
    const auto& wall = this->_level->get().getLayer("Wall");

    for (const auto& tile : wall.allTiles()) {
        const auto& rect = tile.getTextureRect();
        const auto& pos = tile.getWorldPosition();

        utils::Vector4<float> srcRect(static_cast<float>(rect.x), static_cast<float>(rect.y),
                                      static_cast<float>(rect.width), static_cast<float>(rect.height));

        utils::Vector4<float> destRect(static_cast<float>(pos.x), static_cast<float>(pos.y),
                                       static_cast<float>(rect.width), static_cast<float>(rect.height));
        flux::Entity newTile = this->_ecs.newEntity();
        this->_ecs.AddOrReplace<component::Sprite>(
            newTile, component::Sprite(wall.getTileset().path, srcRect.x, srcRect.y, srcRect.w, srcRect.h, 1));
        this->_ecs.AddOrReplace<component::Transform>(newTile,
                                                      component::Transform(destRect.x, destRect.y, 0, 1.0f, 1.0f));
        this->_ecs.AddOrReplace<component::Collider>(newTile,
                                                     component::Collider(component::CollisionLayer::WALL,
                                                                         component::CollisionLayer::PLAYER, pos.x,
                                                                         pos.y, rect.width, rect.height));
    }
}

void map::MapLoader::_setMobs()
{
    for (const ldtk::Entity& mob : this->_level->get().getLayer("Entities").getEntitiesByName("Mob")) {
        flux::Entity mobEntity = this->_ecs.newEntity();
        const auto& rect = mob.getTextureRect();

        const auto& pos = mob.getPosition();
        const auto& size = mob.getSize();
        this->_ecs.Add<component::Mob>(mobEntity, component::Mob());
        this->_ecs.Add<component::Sprite>(
            mobEntity, component::Sprite(mob.getTexturePath(), rect.x, rect.y, rect.width, rect.height, 1));
        this->_ecs.Add<component::Transform>(
            mobEntity, component::Transform(static_cast<float>(pos.x), static_cast<float>(pos.y), 0, 1, 1));
        this->_ecs.Add<component::Velocity>(mobEntity);
        this->_ecs.Add<component::Collider>(
            mobEntity,
            component::Collider(component::CollisionLayer::MOB,
                                component::CollisionLayer::PLAYER | component::CollisionLayer::PLAYER_PROJECTILE, pos.x,
                                pos.y, rect.width, rect.height));
        this->_ecs.Add<component::Health>(mobEntity, component::Health(mob.getField<int>("Health").value()));
        this->_ecs.Add<component::Score>(mobEntity, component::Score());
    }
}

void map::MapLoader::_setCamera() {
    flux::Entity camera = this->_ecs.newEntity();
    this->_ecs.Add<component::Camera>(camera, component::Camera(MAP_WIDTH / 2, MAP_HEIGHT / 2));
    this->_ecs.Add<component::Transform>(camera, component::Transform(MAP_WIDTH / 2, MAP_HEIGHT / 2, 0, 1, 1));
    this->_ecs.Add<component::Velocity>(camera, component::Velocity());
    this->_ecs.Add<component::FixOnScreen>(camera, component::FixOnScreen());
    this->_ecs.Add<component::EndGame>(camera, component::EndGame(this->_level->get().size.x));
}
