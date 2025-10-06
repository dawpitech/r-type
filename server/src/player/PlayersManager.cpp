//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Methods to handle the player
//

#include "player/PlayersManager.hpp"
#include "utils/logger.hpp"

void game::PlayersManager::createNewPlayer(const network::ConnectionInfo &info)
{
    utils::Logger::debug(std::format("New player added with uuid: {}", info.uuid));
    this->_players.emplace_back(std::make_unique<Player>(info));
}
