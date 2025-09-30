//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Threadpool for the rooms
//

#include "RoomsPool.hpp"
#include <iostream>
#include "Network/Network.hpp"
#include "Network/TCP/TCPInfo.hpp"
#include "Network/TCP/TCPNetwork.hpp"
#include "Rooms.hpp"

// this function will be updated later,
// but should remain the same for now in order to test the functionalities
Room::RoomsPool::RoomsPool(std::uint16_t port, std::uint16_t nbRooms) :
    _nbRooms(nbRooms), _connectionNetwork(port), _gameUpdateNetwork(port)
{
    this->_connectionNetwork.attach(*this);
    for (uint16_t i = 0; i < nbRooms; i += 1)
    {
        this->_threads.emplace_back(
            [this, i]
            {
                Room room(i);
                room.update(1);
            });
    };
    while (true)
    {
        this->_connectionNetwork.connect();
    }
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

void Room::RoomsPool::update(const network::ReceivedData &data)
{
    const auto *TCPData = dynamic_cast<const network::ClientTCPReceivedInfo *>(&data);
    if (TCPData != nullptr) {
        //handle TCP DATA
        return;
    }

    // const auto *UDPData = dynamic_cast<const network::ClientUDPReceivedInfo *>(&data);
    // if (UDPData != nullptr) {
    //     //handle UDP data
    //     return;
    // }
}
