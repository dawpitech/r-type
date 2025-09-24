#pragma once

#include <cstdint>

namespace Room {
    constexpr uint8_t BASEROOMPLAYER = 3;

    class Room {
       public:
        Room(std::uint8_t nbPlayers = BASEROOMPLAYER);
        ~Room() = default;

        void update( std::uint8_t nbFrames);
        void clear(std::uint8_t nbPlayers);
       private:
        std::uint8_t _nbPlayers;
    };
}  // namespace Room
