//
// EPITECH PROJECT, 2025
// R-Type
// File description:
// Class for rooms logic
//

#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <mutex>

#include "flux/core/flux.hpp"
#include "player/Player.hpp"
#include "Simulation.hpp"

namespace Room
{
    constexpr uint8_t BASEROOMPLAYER = 3;

    class Room
    {
        public:
            explicit Room(std::size_t roomNumber, std::uint8_t nbPlayers = BASEROOMPLAYER);
            ~Room() = default;

            void run();
            void clear(std::uint8_t nbPlayers);

            bool addPlayer(game::Player& player);
            bool _isRoomFull();
            void setRoomReady(bool state)
            {
                std::lock_guard<std::mutex> lock(this->_roomMutex);
                this->_roomIsReady = state;
            }
            bool isRoomReady()
            {
                std::lock_guard<std::mutex> lock(this->_roomMutex);
                return this->_roomIsReady;
            }

        private:
            std::mutex _roomMutex;
            std::mutex _readyMutex;
            flux::ECS _ecs;
            bool _roomIsReady = false;
            Simulation _simulation;
            std::vector<std::reference_wrapper<game::Player>> _players;
            std::chrono::steady_clock::time_point _networkClock;
            std::uint8_t _nbPlayerMax;
            std::size_t _roomNumber;

            void _assignPlayerToEntity(game::Player& player);
    };
} // namespace Room
