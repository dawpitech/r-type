//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Handle the players
//

#pragma once

#include "Network/TCP/TCPInfo.hpp"
#include "Player/Player.hpp"
#include "network/datatype.hpp"

namespace game
{
    class PlayersManager final
    {
        public:
            explicit PlayersManager() = default;
            ~PlayersManager() = default;

            void createNewPlayer(const network::ConnectionInfo& info);
            void storeInfo(const network::ClientTCPReceivedInfo &info);

        private:
            std::vector<std::unique_ptr<Player>> _players;
    };
} // namespace game
