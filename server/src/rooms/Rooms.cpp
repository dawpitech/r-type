//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Logic for each rooms
//

#include <chrono>
#include <format>
#include <mutex>
#include <thread>

#include "rooms/Rooms.hpp"
#include "Simulation.hpp"
#include "flux/core/flux.hpp"
#include "network/TCP/TCPInfo.hpp"
#include "player/Player.hpp"
#include "utils/logger.hpp"

Room::Room::Room(const std::size_t roomNumber, const std::uint8_t nbPlayers) :
    _roomNumber(roomNumber), _nbPlayerMax(nbPlayers)
{
}

void Room::Room::run()
{
    this->_simulation.runSimulation();
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    };
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
    return this->_players.size() >= this->_nbPlayerMax;
}
