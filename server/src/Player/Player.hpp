//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle single player
//

#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

#include "Network/TCP/TCPInfo.hpp"

namespace game
{

    class Player
    {
        public:
            explicit Player(const network::ConnectionInfo& info) : _id(info.uuid) {};
            ~Player() = default;

            [[nodiscard]] const std::string& getId() { return this->_id; };

        private:
            std::string _id;
            uint16_t _score = 0;
    };
} // namespace game
