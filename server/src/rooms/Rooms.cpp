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
    hooks.hooksNetwork = [this](flux::ECS& ecs)
    {
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
                    if (this->_players.size() > playerIndex) {
                        std::cout << "Attributing the network" << std::endl;
                        component::NetworkIdentification id{};
                        std::strcpy(id.uuid, this->_players[playerIndex].get().getId().c_str());
                        ecs.AddOrReplace(entity, id);
                        serializedData << flux::SerializerHandler<component::NetworkIdentification>::serialize(ecs, entity, id);
                    }
                    playerIndex += 1;
                    continue;
                }
                if (component.type() == typeid(component::PlayerInput)) {
                    auto& comp = std::any_cast<const component::PlayerInput&>(component);
                    serializedData << flux::SerializerHandler<component::PlayerInput>::serialize(ecs, entity, comp)
                                   << std::endl;
                    continue;
                }
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
                // if (component.type() == typeid(component::NetworkIdentification)) {
                //     auto& comp = std::any_cast<const component::NetworkIdentification&>(component);
                //     serializedData << flux::SerializerHandler<component::NetworkIdentification>::serialize(ecs, entity,
                //                                                                                            comp)
                //                    << std::endl;
                //     continue;
                // }
            }
        };

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        for (auto& it : this->_players) {
            it.get().sendData(serializedData.str());
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
    return true;
}

bool Room::Room::_isRoomFull()
{
    std::lock_guard<std::mutex> lock(this->_roomMutex);
    return this->_players.size() > this->_nbPlayerMax;
}
