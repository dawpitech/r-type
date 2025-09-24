#pragma once

#include <cstddef>
#include <cstdint>
#include <thread>
#include <vector>

namespace Room {
    constexpr uint8_t BASENBROOMS = 10;

    class RoomsPool {
       public:
        RoomsPool(std::size_t nbRooms = BASENBROOMS);
        ~RoomsPool();

        void run();

       private:
        std::uint8_t _nbRooms;
        std::vector<std::thread> _threads;
    };
}  // namespace Room
