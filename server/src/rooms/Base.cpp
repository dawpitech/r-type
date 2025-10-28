//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Logic for each rooms
//

#include <exception>
#include <format>
#include <mutex>

#include "Simulation.hpp"
#include "components/NetworkIdentification.hpp"
#include "components/Player.hpp"
#include "flux/core/Serialization.hpp"
#include "flux/core/flux.hpp"
#include "player/Player.hpp"
#include "rooms/Rooms.hpp"
#include "utils/logger.hpp"
#include <condition_variable>

using std::uint8_t;

Room::Room::Room(const std::size_t roomNumber, const std::uint8_t nbPlayers)
    : _roomNumber(roomNumber), _nbPlayerMax(nbPlayers)
{}

Room::Room::~Room()
{
    this->stop();
}

void Room::Room::displayInfo() const
{
    auto msg = std::format(
        "Room number {}\n"
        "\tPlayer {} / {}"
        ,this->_roomNumber, this->_players.size(), this->_nbPlayerMax
    );
    std::cout << msg << std::endl;
}

void Room::Room::run()
{
    std::thread threadRun([this] {
        {
            std::lock_guard<std::mutex> lock(this->_roomMutex);
            Simulation::setInitialSimState(this->_ecs, "Level_0");
        }
        this->_setRoomReady();
        this->_waitRoomFull();

        if (!this->_isRunning) {
            return;
        }
        flux::runtimeHooks hooks;
        this->_initHooks(hooks);
        this->_ecs.handExecution(hooks);
    });
    threadRun.join();
}

void Room::Room::stop()
{
    this->_isRunning = false;
    this->_readyCondition.notify_all();
    this->_fullCondition.notify_all();
    this->_ecs.stop();
}

void Room::Room::_waitRoomFull()
{
    std::unique_lock<std::mutex> lock(this->_roomMutex);
    this->_fullCondition.wait(lock, [this] { return !this->_isRunning || this->_players.size() >= BASEROOMPLAYER; });
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
    player.assignRoom(this->_roomNumber);

    this->_assignPlayerToEntity(player);
    return true;
}

void Room::Room::notifyRoomFull()
{
    if (this->_players.size() >= BASEROOMPLAYER)
        this->_fullCondition.notify_all();
}

bool Room::Room::isRoomFull()
{
    std::lock_guard<std::mutex> lock(this->_roomMutex);
    return this->_players.size() >= this->_nbPlayerMax;
}

void Room::Room::_setRoomReady()
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
