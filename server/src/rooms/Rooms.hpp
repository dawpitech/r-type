//
// EPITECH PROJECT, 2025
// R-Type
// File description:
// Class for rooms logic
//

#pragma once

#include <cstdint>
#include <mutex>

#include "Simulation.hpp"
#include "flux/core/flux.hpp"
#include "player/Player.hpp"

namespace Room {
    constexpr uint8_t BASEROOMPLAYER = 3;

    class Room {
       public:
        explicit Room(std::size_t roomNumber, std::uint8_t nbPlayers = BASEROOMPLAYER);
        ~Room() = default;

        void run();
        void clear(std::uint8_t nbPlayers);

        bool addPlayer(game::Player &player);
        bool _isRoomFull();

       private:
        std::mutex _roomMutex;
        flux::ECS _ecs;
        Simulation _simulation;
        std::vector<std::reference_wrapper<game::Player>> _players;
        std::uint8_t _nbPlayerMax;
        std::size_t _roomNumber;
        
    };
}  // namespace Room
