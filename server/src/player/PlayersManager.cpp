//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Methods to handle the player
//

#include <algorithm>
#include <functional>
#include <mutex>

#include "player/PlayersManager.hpp"
#include "network/UDP/UDPNetwork.hpp"
#include "network/datatype.hpp"
#include "player/Player.hpp"
#include "utils/logger.hpp"

std::optional<std::reference_wrapper<game::Player>> game::PlayersManager::getPlayer(const std::string &id)
{
    std::lock_guard<std::mutex> lock(this->_playerLock);
    for (auto &it: this->_players) {
        if (id == it->getId()) {
            return *it;
        }
    }
    return std::nullopt;
}

void game::PlayersManager::createNewPlayer(const network::ConnectionInfo& info, network::UDPNetwork &network)
{
    
    std::lock_guard<std::mutex> lock(this->_playerLock);
    utils::Logger::debug(std::format("New player added with uuid: {}", info.uuid));
    this->_players.emplace_back(std::make_unique<Player>(info, network));
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

void game::PlayersManager::storeInput(const network::UDPReceivedInfo& info)
{
    for (auto& it : this->_players) {
        if (it->getId() == info.uuid) {
            it->storeInput(info.game);
            return;
        }
    }
    utils::Logger::debug(std::format("No player with uuid: {}", info.uuid));
}

std::optional<uint8_t> game::PlayersManager::getPlayerRoom(const std::string &id)
{
    for (auto& it : this->_players) {
        if (it->getId() == id) {
            return it->getRoom();
        }
    }
    utils::Logger::debug(std::format("No player with uuid: {}", id));
    return std::nullopt;
}
