//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Handle the players
//

#pragma once

#include <functional>
#include "Player.hpp"
#include "components/PlayerInput.hpp"
#include "network/UDP/UDPNetwork.hpp"
#include "network/datatype.hpp"
#include "network/TCP/TCPInfo.hpp"

namespace game
{
    class PlayersManager final
    {
        public:
            explicit PlayersManager() = default;
            ~PlayersManager() = default;

            std::optional<std::reference_wrapper<Player>> getPlayer(const std::string &id);

            void createNewPlayer(const network::ConnectionInfo& info, network::UDPNetwork& network);
            void storeInfo(const network::ClientTCPReceivedInfo &info);
            void storeInput(const network::UDPReceivedInfo &info);

        private:
            std::mutex _lock;
            std::vector<std::unique_ptr<Player>> _players;
    };
} // namespace game
