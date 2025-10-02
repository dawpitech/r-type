//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle single player
//

#pragma once

#include <format>
#include <string>
#include "network/datatype.hpp"

namespace game
{

    class Player
    {
        public:
            explicit Player(const network::ConnectionInfo& info) : _id(info.uuid) {}
            ~Player() = default;

            [[nodiscard]] const std::string& getId() const { return this->_id; }
            void storeInfo(const network::ClientTCPReceivedInfo& info)
            {
                utils::Logger::debug(std::format("Player {} on port {}", this->_id, info.portUDP));
                this->_udpPort = info.portUDP;
            }

        private:
            std::string _id;
            uint16_t _score = 0;
            uint16_t _udpPort = 0;
    };
} // namespace game
