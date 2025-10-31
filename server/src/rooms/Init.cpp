//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Init the rooms
//

#include <sstream>
#include <unordered_map>

#include "Rooms.hpp"
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
#include "components/score.hpp"
#include "flux/core/Serialization.hpp"

void Room::Room::_initHooks(flux::runtimeHooks &hooks)
{
    this->_initUpdateHook(hooks);
    this->_initNetworkHook(hooks);
}

void Room::Room::_initUpdateHook(flux::runtimeHooks &hooks)
{
    hooks.hookBeforeUpdate = [this](flux::ECS &ecs) {
        std::lock_guard<std::mutex> lock(this->_roomMutex);
        auto view =
            ecs.GenerateViewFromComponents<component::NetworkIdentification, component::PlayerInput>();
        auto entities = ecs.QueryViewNotExclusive(view);

        for (auto &playerRef : this->_players) {
            auto &player = playerRef.get();
            const auto &uuid = player.getId();

            for (auto entity : entities) {
                if (!ecs.HasComponent<component::NetworkIdentification>(entity) &&
                    !ecs.HasComponent<component::Health>(entity))
                    continue;
                auto &idComp = ecs.GetComponent<component::NetworkIdentification>(entity);
                auto health = ecs.GetComponent<component::Health>(entity);

                if (health.healthPoint == 0)
                    continue;

                if (uuid == idComp.uuid) {
                    auto inputComp = player.getInput();
                    ecs.AddOrReplace<component::PlayerInput>(entity, inputComp);
                    break;
                }
            }
        }
    };
}

void Room::Room::_initNetworkHook(flux::runtimeHooks &hooks)
{
    hooks.hooksNetwork = [this](flux::ECS &ecs) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->_networkClock);

        if (elapsed.count() < 16) {
            return;
        }

        this->_networkClock = now;

        std::unordered_map<flux::Entity, std::vector<std::any>> componentStore;
        this->_getSnapshot(componentStore);

        std::ostringstream serializedData;
        for (const auto &[entity, components] : componentStore) {
            for (const auto &component : components) {
                this->_serializeComponent(entity, component, serializedData);
            }
        };
        std::lock_guard<std::mutex> lock(this->_roomMutex);

        this->_sendSnapshotToPlayer(serializedData);
    };
}

void Room::Room::_getSnapshot(std::unordered_map<flux::Entity, std::vector<std::any>> &componentStore)
{
    this->_ecs.getEntities<component::Collider>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Health>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Mob>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Player>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::PlayerInput>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Projectile>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Transform>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Velocity>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Sprite>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Camera>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::FixOnScreen>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::EndGame>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::NetworkIdentification>(this->_ecs, componentStore);
    this->_ecs.getEntities<component::Score>(this->_ecs, componentStore);

    this->_snapshots.push_back(componentStore);
    if (this->_snapshots.size() > NB_SNAPSHOTS) {
        this->_snapshots.pop_front();
    }
}

void Room::Room::_serializeComponent(
    const unsigned entity, const std::any &component, std::ostringstream &out)
{
    this->_getSerializedComponent<component::Collider>(entity, component, out);
    this->_getSerializedComponent<component::Health>(entity, component, out);
    this->_getSerializedComponent<component::Mob>(entity, component, out);
    this->_getSerializedComponent<component::Player>(entity, component, out);
    this->_getSerializedComponent<component::Projectile>(entity, component, out);
    this->_getSerializedComponent<component::Transform>(entity, component, out);
    this->_getSerializedComponent<component::Velocity>(entity, component, out);
    this->_getSerializedComponent<component::Sprite>(entity, component, out);
    this->_getSerializedComponent<component::Camera>(entity, component, out);
    this->_getSerializedComponent<component::FixOnScreen>(entity, component, out);
    this->_getSerializedComponent<component::EndGame>(entity, component, out);
    this->_getSerializedComponent<component::Score>(entity, component, out);
}

void Room::Room::_sendSnapshotToPlayer(std::ostringstream &serializedData)
{
    for (auto &it : this->_players) {
        auto &player = it.get();
        std::ostringstream localData;
        localData << serializedData.str();
        auto entity = player.getEntity();
        if (entity != game::BASE_ENTITY) {
            if (this->_ecs.HasComponent<component::NetworkIdentification>(entity)) {
                auto &component = this->_ecs.GetComponent<component::NetworkIdentification>(entity);
                localData << flux::SerializerHandler<component::NetworkIdentification>::serialize(
                                 this->_ecs, player.getEntity(), component)
                          << std::endl;
            } else if (this->_ecs.HasComponent<component::PlayerInput>(entity)) {
                auto &input = this->_ecs.GetComponent<component::PlayerInput>(entity);
                localData << flux::SerializerHandler<component::PlayerInput>::serialize(
                                 this->_ecs, player.getEntity(), input)
                          << std::endl;
            }
        }
        try {
            player.sendData(localData.str());
        } catch (const boost::system::system_error &e) {
            utils::Logger::debug(
                std::format("Failed to send game data to player {}:\n\t{}", player.getId(), e.what()));
        }
    }
}
