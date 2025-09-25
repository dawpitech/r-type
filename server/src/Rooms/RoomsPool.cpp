//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Threadpool for the rooms
//

#include "RoomsPool.hpp"
#include "Network/Network.hpp"
#include "Network/TCP/TCPNetwork.hpp"
#include "Rooms.hpp"

Room::RoomsPool::RoomsPool(std::uint16_t port, std::uint16_t nbRooms) :
    _nbRooms(nbRooms), _connectionNetwork(port), _gameUpdateNetwork(port)
{
    for (uint16_t i = 0; i < nbRooms; i += 1)
    {
        this->_threads.emplace_back(
            [this, i]
            {
                Room room(i);
                room.update(1);
            });
    };
    for (auto& thread : this->_threads)
    {
        thread.join();
    }
}

Room::RoomsPool::~RoomsPool() {}

void Room::RoomsPool::run()
{
    while (this->_isRunning)
    {
    }
}
