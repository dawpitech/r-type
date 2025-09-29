//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle rooms threadpool
//
#pragma once

#include <cstddef>
#include <cstdint>
#include <thread>
#include <vector>
#include "Network/TCP/TCPNetwork.hpp"
#include "Network/UDP/UDPNetwork.hpp"

namespace Room {
    constexpr uint16_t BASENBROOMS = 10;

    class RoomsPool {
       public:
        explicit RoomsPool(std::uint16_t port, std::uint16_t nbRooms = BASENBROOMS);
        ~RoomsPool();

        void run();

       private:
        std::uint8_t _nbRooms;
        network::TCPNetwork _connectionNetwork;
        network::UDPNetwork _gameUpdateNetwork;
        std::vector<std::thread> _threads;
        bool _isRunning = true;
    };
}  // namespace Room
