//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Admin handler
//

#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace Server {
    class Server;

    class AdminHandler {
       public:
        AdminHandler(Server &server);

        ~AdminHandler() = default;
        void executeInput(const std::string &input);

       private:
        Server &_server;

        std::unordered_map<std::string, std::function<void()>> _commandsMap;
        void _initMap();

        void _getNbRooms();
        void _getRoomsInfo();

        void _getRoomInfo();
        
        void _resetRooms();
        void _resetRoom();
    };
}  // namespace Server
