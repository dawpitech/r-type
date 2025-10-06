//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Methods to handle the player
//

#include "Player/PlayersManager.hpp"
#include <algorithm>
#include "network/datatype.hpp"

void game::PlayersManager::createNewPlayer(const network::ConnectionInfo& info)
{
    utils::Logger::debug(std::format("New player added with uuid: {}", info.uuid));
    this->_players.emplace_back(std::make_unique<Player>(info));
}

void game::PlayersManager::storeInfo(const network::ClientTCPReceivedInfo& info)
{
    for (auto& it : this->_players) {
        if (it->getId() == info.uuid) {
            it->storeInfo(info);
            return;
        }
    }
    utils::Logger::debug(std::format("No player with uuid: {}", info.uuid));
}
