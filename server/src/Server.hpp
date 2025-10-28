//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle rooms threadpool
//
#pragma once

#include <cstdint>
#include <thread>
#include <vector>
#include "player/PlayersManager.hpp"
#include "network/TCP/TCPNetwork.hpp"
#include "network/UDP/UDPNetwork.hpp"
#include "rooms/Rooms.hpp"

namespace Server {
    constexpr uint16_t BASENBROOMS = 10;

    class Server final {
       public:
        explicit Server(std::uint16_t port, std::uint16_t nbRooms = BASENBROOMS, bool cli = false);
        ~Server();

        void run();
        void stop() {this->_isRunning = false;}
        std::uint8_t getNbRooms() const {return this->_nbRooms;}
        void displayRoomsInfos();
        void displayRoomInfos(uint8_t roomNumber);
        
       private:
        std::uint8_t _nbRooms = 0;
        bool _isRunning = true;
        bool _cli;

        network::TCPNetwork _connectionNetwork;
        network::UDPNetwork _gameUpdateNetwork;

        game::PlayersManager _playerManager;
        std::vector<std::jthread> _threads;
        std::vector<std::unique_ptr<Room::Room>> _rooms;
    };
}  // namespace Room
