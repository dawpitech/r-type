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
#include <limits>

void Server::AdminHandler::_getNbRooms()
{
    std::cout << std::format("{}", this->_server.getNbRooms()) << std::endl;
}

void Server::AdminHandler::_getRoomsInfo()
{
    this->_server.displayRoomsInfos();
}

void Server::AdminHandler::_getRoomInfo()
{
    int roomNumberInt = 0;
    std::cout << "Type room number(start at 0):" << std::endl;
    std::cin >> roomNumberInt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (roomNumberInt < 0)
        return;
    this->_server.displayRoomInfos(roomNumberInt);
    std::cin.clear();
}
