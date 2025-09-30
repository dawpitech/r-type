//
// EPITECH PROJECT, 2025
// R-Type
// File description:
// Class for rooms logic
//

#pragma once

#include <cstdint>
#include "Network/TCP/TCPInfo.hpp"
#include "Player/Player.hpp"

namespace Room {
    constexpr uint8_t BASEROOMPLAYER = 3;

    class Room {
       public:
        explicit Room(std::size_t roomNumber, std::uint8_t nbPlayers = BASEROOMPLAYER);
        ~Room() = default;

        void update( std::uint8_t nbFrames);
        void clear(std::uint8_t nbPlayers);

        bool addPlayer(network::ClientTCP &clientTCP, const std::string &id);
        bool removePlayer();

       private:
        std::vector<std::unique_ptr<game::Player>> _players;
        std::uint8_t _nbPlayerMax;
        std::size_t _roomNumber;
        
    };
}  // namespace Room
