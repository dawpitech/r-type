//
// EPITECH PROJECT, 2025
// R-Type
// File description:
// Class for rooms logic
//

#pragma once

#include <cstdint>

namespace Room {
    constexpr uint8_t BASEROOMPLAYER = 3;

    class Room {
       public:
        explicit Room(std::uint8_t nbPlayers = BASEROOMPLAYER);
        ~Room() = default;

        void update( std::uint8_t nbFrames);
        void clear(std::uint8_t nbPlayers);
       private:
        std::uint8_t _nbPlayers;
    };
}  // namespace Room
