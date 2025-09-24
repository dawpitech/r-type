#include "RoomsPool.hpp"
#include "Rooms.hpp"
#include <cstddef>


Room::RoomsPool::RoomsPool(std::size_t nbRooms): _nbRooms(nbRooms)
{
    for (size_t i = 0; i < nbRooms; i += 1) {
        this->_threads.emplace_back([this] {
            Room room;
        });
    };
    for (auto &thread: this->_threads) {
        thread.join();
    }
}

Room::RoomsPool::~RoomsPool()
{
}
