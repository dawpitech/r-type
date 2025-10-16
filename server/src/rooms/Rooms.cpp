//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Logic for each rooms
//

#include <any>
#include <chrono>
#include <format>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>

#include "Simulation.hpp"
#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/NetworkIdentification.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Projectile.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/Serialization.hpp"
#include "flux/core/flux.hpp"
#include "player/Player.hpp"
#include "rooms/Rooms.hpp"
#include "utils/logger.hpp"
#include <condition_variable>

Room::Room::Room(const std::size_t roomNumber, const std::uint8_t nbPlayers)
    : _roomNumber(roomNumber), _nbPlayerMax(nbPlayers)
{}

void Room::Room::_initHooks(flux::runtimeHooks &hooks)
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
                try {
                    auto &idComp = ecs.GetComponent<component::NetworkIdentification>(entity);

                    if (!ecs.HasComponent<component::Health>(entity)) {
                        continue;
                    }
                    auto health = ecs.GetComponent<component::Health>(entity);
                    if (health.healthPoint == 0) {
                        continue;
                    }

                    if (uuid == idComp.uuid) {
                        auto inputComp = player.getInput();
                        ecs.AddOrReplace<component::PlayerInput>(entity, inputComp);

                        break;
                    }
                } catch (const flux::ECS::FluxException &e) {
                    continue;
                }
            }
        }
    };

    hooks.hooksNetwork = [this](flux::ECS &ecs) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->_networkClock);

        if (elapsed.count() < 50) {
            return;
        }

        this->_networkClock = now;

        uint8_t playerIndex = 0;
        std::unordered_map<flux::Entity, std::vector<std::any>> componentStore;

        ecs.getEntities<component::Collider>(ecs, componentStore);
        ecs.getEntities<component::Health>(ecs, componentStore);
        ecs.getEntities<component::Mob>(ecs, componentStore);
        ecs.getEntities<component::Player>(ecs, componentStore);
        ecs.getEntities<component::PlayerInput>(ecs, componentStore);
        ecs.getEntities<component::Projectile>(ecs, componentStore);
        ecs.getEntities<component::Transform>(ecs, componentStore);
        ecs.getEntities<component::Velocity>(ecs, componentStore);
        ecs.getEntities<component::NetworkIdentification>(ecs, componentStore);

        std::ostringstream serializedData;
        for (const auto &[entity, components] : componentStore) {
            for (const auto &component : components) {
                if (component.type() == typeid(component::Collider)) {
                    auto &comp = std::any_cast<const component::Collider &>(component);
                    serializedData << flux::SerializerHandler<component::Collider>::serialize(
                                          ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::Health)) {
                    auto &comp = std::any_cast<const component::Health &>(component);
                    serializedData << flux::SerializerHandler<component::Health>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::Mob)) {
                    auto &comp = std::any_cast<const component::Mob &>(component);
                    serializedData << flux::SerializerHandler<component::Mob>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::Player)) {
                    auto &comp = std::any_cast<const component::Player &>(component);
                    serializedData << flux::SerializerHandler<component::Player>::serialize(ecs, entity, comp)
                                   << std::endl;
                }
                if (component.type() == typeid(component::Projectile)) {
                    auto &comp = std::any_cast<const component::Projectile &>(component);
                    serializedData << flux::SerializerHandler<component::Projectile>::serialize(
                                          ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::Transform)) {
                    auto &comp = std::any_cast<const component::Transform &>(component);
                    serializedData << flux::SerializerHandler<component::Transform>::serialize(
                                          ecs, entity, comp)
                                   << std::endl;
                    ;
                    continue;
                }
                if (component.type() == typeid(component::Velocity)) {
                    auto &comp = std::any_cast<const component::Velocity &>(component);
                    serializedData << flux::SerializerHandler<component::Velocity>::serialize(
                                          ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
            }
        };
        std::lock_guard<std::mutex> lock(this->_roomMutex);

        for (auto &it : this->_players) {
            std::ostringstream localData;
            localData << serializedData.str();
            auto entity = it.get().getEntity();
            if (entity != game::BASE_ENTITY) {
                if (ecs.HasComponent<component::NetworkIdentification>(entity)) {
                    auto &existingId = ecs.GetComponent<component::NetworkIdentification>(entity);
                    localData << flux::SerializerHandler<component::NetworkIdentification>::serialize(
                                     ecs, it.get().getEntity(), existingId)
                              << std::endl;
                } else {
                    if (ecs.HasComponent<component::PlayerInput>(entity)) {
                        auto &input = ecs.GetComponent<component::PlayerInput>(entity);
                        localData << flux::SerializerHandler<component::PlayerInput>::serialize(
                                         ecs, it.get().getEntity(), input)
                                  << std::endl;
                    }
                }
            }
            try {
                it.get().sendData(localData.str());
            } catch (const boost::system::system_error &e) {
                utils::Logger::debug(std::format("Failed to send game data to player: {}", e.what()));
            }
        }
    };
}

void Room::Room::run()
{
    {
        std::lock_guard<std::mutex> lock(this->_roomMutex);
        Simulation::setInitialSimState(this->_ecs);
    }
    this->setRoomReady();

    flux::runtimeHooks hooks;
    this->_initHooks(hooks);
    this->_ecs.handExecution(hooks);
}

void Room::Room::clear(const std::uint8_t nbPlayers)
{
    std::lock_guard<std::mutex> lock(this->_roomMutex);
    this->_players.erase(this->_players.begin(), this->_players.end());
    this->_nbPlayerMax = nbPlayers;
}

bool Room::Room::addPlayer(game::Player &player)
{
    std::lock_guard<std::mutex> lock(this->_roomMutex);
    if (this->_players.size() >= this->_nbPlayerMax) {
        auto log = std::format("Too many player ! Can't add user in room {}", this->_roomNumber);
        utils::Logger::debug(log);
        return false;
    }

    this->_players.push_back(player);

    this->_assignPlayerToEntity(player);
    return true;
}

bool Room::Room::isRoomFull()
{
    std::lock_guard<std::mutex> lock(this->_roomMutex);
    return this->_players.size() >= this->_nbPlayerMax;
}

void Room::Room::setRoomReady()
{
    std::lock_guard<std::mutex> lock(this->_readyMutex);
    this->_isReady = true;
    this->_readyCondition.notify_all();
}

void Room::Room::waitRoomReady()
{
    std::unique_lock<std::mutex> lock(this->_readyMutex);
    this->_readyCondition.wait(lock, [this] { return this->_isReady; });
}

void Room::Room::_assignPlayerToEntity(game::Player &player)
{
    const auto playerView = this->_ecs.GenerateViewFromComponents<component::Player>();
    const auto allPlayerEntities = this->_ecs.QueryViewNotExclusive(playerView);

    for (auto entity : allPlayerEntities) {
        if (this->_ecs.HasComponent<component::NetworkIdentification>(entity)) {
            auto &existingId = this->_ecs.GetComponent<component::NetworkIdentification>(entity);
            if (std::strcmp(existingId.uuid, player.getId().c_str()) == 0) {
                return;
            }
        }
    }

    for (auto entity : allPlayerEntities) {
        if (!this->_ecs.HasComponent<component::NetworkIdentification>(entity)) {
            component::NetworkIdentification id{};
            std::strcpy(id.uuid, player.getId().c_str());
            this->_ecs.AddOrReplace(entity, id);
            player.assignEntity(entity);
            utils::Logger::debug(
                std::format("Assigned uuid {} to entity {}", player.getId(), player.getEntity()));
            return;
        }
    }
}
