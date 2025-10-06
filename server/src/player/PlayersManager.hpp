//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Handle the players
//

#pragma once

#include "network/TCP/TCPInfo.hpp"
#include "Player.hpp"

namespace game
{
    class PlayersManager final
    {
        public:
            explicit PlayersManager() = default;
            ~PlayersManager() = default;

            void createNewPlayer(const network::ConnectionInfo& info);

        private:
            std::vector<std::unique_ptr<Player>> _players;
    };
} // namespace game
