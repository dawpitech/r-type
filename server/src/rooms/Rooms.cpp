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

#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/NetworkIdentification.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Projectile.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "flux/core/Serialization.hpp"
#include "player/Player.hpp"
#include "rooms/Rooms.hpp"
#include "Simulation.hpp"
#include "utils/logger.hpp"

Room::Room::Room(const std::size_t roomNumber, const std::uint8_t nbPlayers) :
    _roomNumber(roomNumber), _nbPlayerMax(nbPlayers)
{}

void Room::Room::run()
{
    flux::runtimeHooks hooks;

    auto lastUpdate = std::chrono::steady_clock::now();
    hooks.hookBeforeUpdate = [this, &lastUpdate](flux::ECS& ecs)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

        if (elapsed.count() < 50) {
            return;
        }
        lastUpdate = now;

        // std::lock_guard<std::mutex> lock(this->_roomMutex);
        auto view = ecs.GenerateViewFromComponents<component::NetworkIdentification, component::PlayerInput>();
        auto entities = ecs.QueryViewNotExclusive(view);

        for (auto& playerRef : this->_players) {
            auto& player = playerRef.get();
            const auto& uuid = player.getId();
            bool inputApplied = false;

            for (auto entity : entities) {
                try {
                    auto& idComp = ecs.GetComponent<component::NetworkIdentification>(entity);

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

                        inputApplied = true;
                        break;
                    }
                }
                catch (const flux::ECS::FluxException& e) {
                    continue;
                }
            }
        }
        // for (auto& player : this->_players) {
        //     player.get().storeInput(component::PlayerInput{});
        // }
    };

    hooks.hooksNetwork = [this](flux::ECS& ecs)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->_networkClock);

        if (elapsed.count() < 50) {
            return;
        }

        this->_networkClock = now;

        uint8_t playerIndex = 0;
        std::unordered_map<flux::Entity, std::vector<std::any>> componentStore;

        ecs.getEntities<component::collider>(ecs, componentStore);
        ecs.getEntities<component::Health>(ecs, componentStore);
        ecs.getEntities<component::mob>(ecs, componentStore);
        ecs.getEntities<component::Player>(ecs, componentStore);
        ecs.getEntities<component::PlayerInput>(ecs, componentStore);
        ecs.getEntities<component::Projectile>(ecs, componentStore);
        ecs.getEntities<component::Transform>(ecs, componentStore);
        ecs.getEntities<component::Velocity>(ecs, componentStore);
        ecs.getEntities<component::NetworkIdentification>(ecs, componentStore);

        std::ostringstream serializedData;
        for (const auto& [entity, components] : componentStore) {
            for (const auto& component : components) {
                if (component.type() == typeid(component::collider)) {
                    auto& comp = std::any_cast<const component::collider&>(component);
                    serializedData << flux::SerializerHandler<component::collider>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::Health)) {
                    auto& comp = std::any_cast<const component::Health&>(component);
                    serializedData << flux::SerializerHandler<component::Health>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::mob)) {
                    auto& comp = std::any_cast<const component::mob&>(component);
                    serializedData << flux::SerializerHandler<component::mob>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::Player)) {
                    auto& comp = std::any_cast<const component::Player&>(component);
                    serializedData << flux::SerializerHandler<component::Player>::serialize(ecs, entity, comp)
                                   << std::endl;
                }
                // if (component.type() == typeid(component::NetworkIdentification)) {
                //     auto& existingId = ecs.GetComponent<component::NetworkIdentification>(entity);
                //     serializedData << flux::SerializerHandler<component::NetworkIdentification>::serialize(ecs,
                //     entity,
                //                                                                                            existingId)
                //                    << std::endl;
                // }
                // if (component.type() == typeid(component::PlayerInput)) {
                //     auto& comp = std::any_cast<const component::PlayerInput&>(component);
                //     serializedData << flux::SerializerHandler<component::PlayerInput>::serialize(ecs, entity, comp)
                //                    << std::endl;
                //     continue;
                // }
                if (component.type() == typeid(component::Projectile)) {
                    auto& comp = std::any_cast<const component::Projectile&>(component);
                    serializedData << flux::SerializerHandler<component::Projectile>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
                if (component.type() == typeid(component::Transform)) {
                    auto& comp = std::any_cast<const component::Transform&>(component);
                    serializedData << flux::SerializerHandler<component::Transform>::serialize(ecs, entity, comp)
                                   << std::endl;
                    ;
                    continue;
                }
                if (component.type() == typeid(component::Velocity)) {
                    auto& comp = std::any_cast<const component::Velocity&>(component);
                    serializedData << flux::SerializerHandler<component::Velocity>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
            }
        };

        // std::this_thread::sleep_for(std::chrono::milliseconds(20));
        // std::cout << "new DATA:\n";
        // std::cout << serializedData.str() << "\n\n";
        for (auto& it : this->_players) {
            std::ostringstream localData;
            localData << serializedData.str();
            auto entity = it.get().getEntity();
            if (entity != game::BASE_ENTITY) {
                if (ecs.HasComponent<component::NetworkIdentification>(entity)) {
                    auto& existingId = ecs.GetComponent<component::NetworkIdentification>(entity);
                    localData << flux::SerializerHandler<component::NetworkIdentification>::serialize(
                                     ecs, it.get().getEntity(), existingId)
                              << std::endl;
                }
                else {
                    if (ecs.HasComponent<component::PlayerInput>(entity)) {
                        auto& existingId = ecs.GetComponent<component::PlayerInput>(entity);
                        localData << flux::SerializerHandler<component::PlayerInput>::serialize(
                                         ecs, it.get().getEntity(), existingId)
                                  << std::endl;
                    }
                }
            }
            try {
                it.get().sendData(localData.str());
            }
            catch (const boost::system::system_error& e) {
                utils::Logger::debug(std::format("Failed to send data to player: {}", e.what()));
            }
        }
    };

    this->_simulation.runServerSimulation(hooks);
}

void Room::Room::clear(const std::uint8_t nbPlayers)
{
    std::lock_guard<std::mutex> lock(this->_roomMutex);
    this->_players.erase(this->_players.begin(), this->_players.end());
    this->_nbPlayerMax = nbPlayers;
}

bool Room::Room::addPlayer(game::Player& player)
{
    if (this->_isRoomFull()) {
        auto log = std::format("Too many player ! Can't add user in room {}", this->_roomNumber);
        utils::Logger::debug(log);
        return false;
    }

    std::lock_guard<std::mutex> lock(this->_roomMutex);
    this->_players.emplace_back(player);

    this->_assignPlayerToEntity(player);
    return true;
}

bool Room::Room::_isRoomFull()
{
    std::lock_guard<std::mutex> lock(this->_roomMutex);
    return this->_players.size() > this->_nbPlayerMax;
}

void Room::Room::_assignPlayerToEntity(game::Player& player)
{
    auto& ecs = this->_simulation.getEcs();
    auto playerView = ecs.GenerateViewFromComponents<component::Player>();
    auto allPlayerEntities = ecs.QueryViewNotExclusive(playerView);

    for (auto entity : allPlayerEntities) {
        if (ecs.HasComponent<component::NetworkIdentification>(entity)) {
            auto& existingId = ecs.GetComponent<component::NetworkIdentification>(entity);
            if (std::strcmp(existingId.uuid, player.getId().c_str()) == 0) {
                return;
            }
        }
    }

    for (auto entity : allPlayerEntities) {
        if (!ecs.HasComponent<component::NetworkIdentification>(entity)) {
            component::NetworkIdentification id{};
            std::strcpy(id.uuid, player.getId().c_str());
            ecs.AddOrReplace(entity, id);
            player.assignEntity(entity);
            utils::Logger::debug(std::format("Assigned uuid {} to entity {}", player.getId(), player.getEntity()));
            return;
        }
    }
}
