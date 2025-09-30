//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle single player
//

#pragma once

#include <string>
#include "Network/TCP/TCPInfo.hpp"

namespace game
{
    class Player
    {
        public:
            explicit Player(network::ClientTCP& client, std::string id) : _id(std::move(id)), _tcpClient(client) {};
            ~Player() = delete;

        private:
            std::string _id;
            uint16_t score;
            network::ClientTCP& _tcpClient;
    };
} // namespace game
