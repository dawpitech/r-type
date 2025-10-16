//
// EPITECH PROJECT, 2025
// R-Type
// File description:
// Class for rooms logic
//

#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <ctime>
#include <mutex>

#include "Simulation.hpp"
#include "flux/core/flux.hpp"
#include "player/Player.hpp"

namespace Room {
    constexpr uint8_t BASEROOMPLAYER = 4;

    class Room {
       public:
        explicit Room(std::size_t roomNumber, std::uint8_t nbPlayers = BASEROOMPLAYER);
        ~Room() = default;

        void run();
        void clear(std::uint8_t nbPlayers);

        bool addPlayer(game::Player &player);

        void setRoomReady();

        bool isRoomFull();
        void waitRoomReady();

       private:
        std::mutex _roomMutex;
        std::mutex _readyMutex;
        std::condition_variable _readyCondition;
        flux::ECS _ecs;
        std::vector<std::reference_wrapper<game::Player>> _players;
        std::chrono::steady_clock::time_point _networkClock;
        std::uint8_t _nbPlayerMax;
        std::size_t _roomNumber;
        bool _isReady = false;

        void _assignPlayerToEntity(game::Player& player);
        void _initHooks(flux::runtimeHooks &hooks);
    };
}  // namespace Room
