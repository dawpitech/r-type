//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Threadpool for the rooms
//

#include "RoomsPool.hpp"
#include <chrono>
#include <thread>
#include "Rooms.hpp"
#include "network/datatype.hpp"

// this function will be updated later,
// but should remain the same for now in order to test the functionalities
Room::RoomsPool::RoomsPool(std::uint16_t port, std::uint16_t nbRooms) :
    _nbRooms(nbRooms), _connectionNetwork(port), _gameUpdateNetwork(port)
{
    this->_connectionNetwork.attach<network::ConnectionInfo>([this](const network::ConnectionInfo& info)
                                                             { this->_playerManager.createNewPlayer(info); });
    this->_connectionNetwork.attach<network::ClientTCPReceivedInfo>(
        [this](network::ClientTCPReceivedInfo info) {
            this->_playerManager.storeInfo(info);
        });
    this->_gameUpdateNetwork.attach<network::UDPReceivedInfo>([this](network::UDPReceivedInfo info) {

        return;
    });
    for (uint16_t i = 0; i < nbRooms; i += 1) {
        this->_threads.emplace_back(
            [this, i]
            {
                Room room(i);
                room.update(1);
            });
    };
    while (true) {
        this->_connectionNetwork.connect();
    }
    for (auto& thread : this->_threads) {
        thread.join();
    }
}

Room::RoomsPool::~RoomsPool() {}

void Room::RoomsPool::run()
{
    while (this->_isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
