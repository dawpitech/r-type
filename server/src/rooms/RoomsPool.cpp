//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Threadpool for the rooms
//

#include "RoomsPool.hpp"
#include <thread>

#include "network/UDP/UDPNetwork.hpp"
#include "network/datatype.hpp"
#include "Rooms.hpp"

Room::RoomsPool::RoomsPool(std::uint16_t port, std::uint16_t nbRooms) :
    _nbRooms(nbRooms), _connectionNetwork(port), _gameUpdateNetwork(port)
{
    this->_connectionNetwork.attach<network::ConnectionInfo>(
        [this](const network::ConnectionInfo& info)
        {
            this->_playerManager.createNewPlayer(info, this->_gameUpdateNetwork);
            auto playerOpt = this->_playerManager.getPlayer(info.uuid);
            if (!playerOpt.has_value()) {
                return;
            }
            auto cpt = 0;
            for (auto& room : this->_rooms) {
                if (!room->_isRoomFull()) {
                    std::cout << "Player added to room " << cpt << std::endl;
                    room->addPlayer(playerOpt.value());
                    break;
                }
                cpt += 1;
            };
        });
    this->_connectionNetwork.attach<network::ClientTCPReceivedInfo>([this](network::ClientTCPReceivedInfo info)
                                                                    { this->_playerManager.storeInfo(info); });
    this->_gameUpdateNetwork.attach<network::UDPReceivedInfo>([this](network::UDPReceivedInfo info) {
        this->_playerManager.storeInput(info);
    });

    for (uint16_t i = 0; i < nbRooms; i += 1) {
        this->_rooms.push_back(std::make_unique<Room>(i));
        this->_threads.emplace_back([&] { this->_rooms.back()->run(); });
    }
}

Room::RoomsPool::~RoomsPool() {}

void Room::RoomsPool::run()
{
    while (this->_isRunning) {
        this->_connectionNetwork.connect();
        this->_gameUpdateNetwork.connect();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    for (auto& thread : this->_threads) {
        thread.join();
    }
}
