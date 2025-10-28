//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Command handler for admin
//

#include "AdminHandler.hpp"
#include "Server.hpp"
#include <format>
#include <iostream>

void Server::AdminHandler::_getNbRooms()
{
    std::cout << std::format("{}", this->_server.getNbRooms()) << std::endl;
}

void Server::AdminHandler::_getRoomsInfo()
{
    this->_server.displayRoomsInfos();
}
