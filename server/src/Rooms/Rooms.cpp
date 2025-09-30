//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Logic for each rooms
//

#include <chrono>
#include <format>
#include <thread>

#include "Network/TCP/TCPInfo.hpp"
#include "Player/Player.hpp"
#include "Rooms.hpp"
#include "flux/core/flux.hpp"
#include "utils/logger.hpp"

Room::Room::Room(std::size_t roomNumber, const std::uint8_t nbPlayers) :
    _roomNumber(roomNumber), _nbPlayerMax(nbPlayers)
{
    flux::ECS ecs;
}

void Room::Room::update(const std::uint8_t nbFrames)
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Room::Room::clear(const std::uint8_t nbPlayers)
{
    this->_players.erase(this->_players.begin(), this->_players.end());
    this->_nbPlayerMax = nbPlayers;
}

bool Room::Room::addPlayer(network::ClientTCP& clientTCP, const std::string& id)
{
    if (this->_players.size() == this->_nbPlayerMax) {
        auto log = std::format("Too many player ! Can't add user {} in room {}", id, this->_roomNumber);
        utils::Logger::debug(log);
        return false;
    }

    this->_players.emplace_back(std::make_unique<game::Player>(clientTCP, id));
    return true;
}
