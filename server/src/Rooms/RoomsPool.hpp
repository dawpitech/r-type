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
#include "Network/TCP/TCPNetwork.hpp"
#include "Network/UDP/UDPNetwork.hpp"
#include "Player/PlayersManager.hpp"

namespace Room {
    constexpr uint16_t BASENBROOMS = 10;

    class RoomsPool final {
       public:
        explicit RoomsPool(std::uint16_t port, std::uint16_t nbRooms = BASENBROOMS);
        ~RoomsPool();

        void run();
        
       private:
        std::uint8_t _nbRooms;
        bool _isRunning = true;

        network::TCPNetwork _connectionNetwork;
        network::UDPNetwork _gameUpdateNetwork;

        game::PlayersManager _playerManager;
        std::vector<std::thread> _threads;
    };
}  // namespace Room
