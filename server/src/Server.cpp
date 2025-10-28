//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Threadpool for the rooms
//

#include <chrono>
#include <format>
#include <optional>
#include <thread>

#include "Server.hpp"
#include "rooms/Rooms.hpp"
#include "network/UDP/UDPNetwork.hpp"
#include "network/datatype.hpp"
#include "utils/logger.hpp"

Server::Server::Server(std::uint16_t port, std::uint16_t nbRooms)
    : _nbRooms(nbRooms), _connectionNetwork(port), _gameUpdateNetwork(port), _voiceNetwork(0)
{
    utils::Logger::debug(std::format("Voice port: {}", this->_voiceNetwork.getPort()));
    this->_connectionNetwork.attach<network::ConnectionInfo>(
        [this](const network::ConnectionInfo &info) {
            this->_playerManager.createNewPlayer(
                info, this->_gameUpdateNetwork, this->_voiceNetwork);
            auto playerOpt = this->_playerManager.getPlayer(info.uuid);
            if (!playerOpt.has_value()) {
                return;
            }
            auto cpt = 0;
            for (auto &room : this->_rooms) {
                if (!room->isRoomFull()) {
                    room->waitRoomReady();
                    utils::Logger::debug(
                        std::format("Player added to room {}", cpt));
                    room->addPlayer(playerOpt.value());
                    break;
                }
                cpt += 1;
            };
        });
    this->_connectionNetwork.attach<network::ClientTCPReceivedInfo>(
        [this](network::ClientTCPReceivedInfo info) {
            this->_playerManager.storeInfo(info);
            auto roomNb = this->_playerManager.getPlayerRoom(info.uuid);
            if (roomNb == std::nullopt || roomNb > this->_rooms.size()) {
                utils::Logger::debug("Trying to add player to wrong room");
                return;
            }
            if (this->_rooms[roomNb.value()]->isRoomFull()) {
                this->_rooms[roomNb.value()]->notifyRoomFull();
            }
        });
    this->_gameUpdateNetwork.attach<network::UDPReceivedInfo>(
        [this](network::UDPReceivedInfo info) {
            this->_playerManager.storeInput(info);
        });

    this->_voiceNetwork.attach<network::UDPVoiceInfo>(
        [this](network::UDPVoiceInfo info) {
            this->_playerManager.dispatchSound(info);
        }
);

    for (uint16_t i = 0; i < nbRooms; i += 1) {
        this->_rooms.push_back(std::make_unique<Room::Room>(i));
        Room::Room *roomPtr = this->_rooms.back().get();
        this->_threads.emplace_back([roomPtr] {
            if (roomPtr)
                roomPtr->run();
        });
    }
}

Server::Server::~Server() {}

void Server::Server::run()
{
    while (this->_isRunning) {
        this->_connectionNetwork.connect();
        this->_gameUpdateNetwork.connect();
        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    }
    for (auto &thread : this->_threads) {
        thread.join();
    }
}
