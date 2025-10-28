//
// EPITECH PROJECT, 2025
// R-type File description:
// Admin handler
//

#include "AdminHandler.hpp"
#include "Server.hpp"
#include <algorithm>
#include <cctype>

Server::AdminHandler::AdminHandler(Server &server) : _server(server)
{
    this->_initMap();
}

void Server::AdminHandler::executeInput(const std::string &input)
{
    std::string lowerInput = "";
    for (auto &it : input) {
        lowerInput += std::tolower(it);
    }

    if (this->_commandsMap[lowerInput]) {
        this->_commandsMap[lowerInput]();
        return;
    }
    std::cout << "Command \"" << lowerInput << "\" not found" << std::endl;
}

void Server::AdminHandler::_initMap()
{
    this->_commandsMap["help"] = [this] {
        std::cout << "Commands are: " << std::endl;
        for (auto &[key, _] : this->_commandsMap) {
            std::cout << "\t" << key << std::endl;
        }
    };
    this->_commandsMap["rooms nb"] = [this] {
        this->_getNbRooms();
    };
    this->_commandsMap["rooms info"] = [this] {
        this->_getRoomsInfo();
    };
    this->_commandsMap["kick player"] = [this] {
        this->_kickPlayer();
    };
    this->_commandsMap["get mobs"] = [this] {
        this->_getRemainingMobs();
    };
    this->_commandsMap["spawn mob"] = [this] {
        this->_spawnMob();
    };
    this->_commandsMap["exit"] = [this] {
        this->_server.stop();
    };
}
