//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Methods to handle the player
//

#include "Player/PlayersManager.hpp"
#include <iostream>
#include "Network/TCP/TCPInfo.hpp"

void game::PlayersManager::createNewPlayer(const network::ConnectionInfo &info)
{
    std::cout << "New player added with uuid: " << info.uuid << std::endl;
    this->_players.emplace_back(std::make_unique<Player>(info));
}
