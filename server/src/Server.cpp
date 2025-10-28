//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Threadpool for the rooms
//

#include <chrono>
#include <exception>
#include <format>
#include <optional>
#include <string>
#include <thread>

#include "Server.hpp"
#include "admin/AdminHandler.hpp"
#include "network/UDP/UDPNetwork.hpp"
#include "network/datatype.hpp"
#include "rooms/Rooms.hpp"
#include "utils/logger.hpp"

Server::Server::Server(std::uint16_t port, std::uint16_t nbRooms, bool cli)
    : _nbRooms(nbRooms), _connectionNetwork(port), _gameUpdateNetwork(port), _cli(cli)
{
    this->_connectionNetwork.attach<network::ConnectionInfo>([this](const network::ConnectionInfo &info) {
        this->_playerManager.createNewPlayer(info, this->_gameUpdateNetwork);
        auto playerOpt = this->_playerManager.getPlayer(info.uuid);
        if (!playerOpt.has_value()) {
            return;
        }
        auto cpt = 0;
        for (auto &room : this->_rooms) {
            if (!room->isRoomFull()) {
                room->waitRoomReady();
                utils::Logger::debug(std::format("Player added to room {}", cpt));
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
        [this](network::UDPReceivedInfo info) { this->_playerManager.storeInput(info); });

    this->_setupRooms();
}

void Server::Server::_setupRooms()
{
    for (uint16_t i = 0; i < this->_nbRooms; i += 1) {
        this->_rooms.push_back(std::make_unique<Room::Room>(i));
        Room::Room *roomPtr = this->_rooms.back().get();
        this->_threads.emplace_back([roomPtr] {
            if (roomPtr)
                roomPtr->run();
        });
    }
}

Server::Server::~Server()
{
    for (auto &room : this->_rooms) {
        room->stop();
    }
    for (auto &thread : this->_threads) {
        thread.join();
    }
}

void Server::Server::run()
{
    if (this->_cli) {
        std::thread cliThread([this] {
            AdminHandler admin(*this);
            std::cout << "Welcome Dear Administrator" << std::endl;
            while (this->_isRunning) {
                std::string input;
                std::cout << ">> ";
                std::getline(std::cin, input);
                std::cin.clear();
                admin.executeInput(input);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
        cliThread.detach();
    }
    while (this->_isRunning) {
        this->_connectionNetwork.connect();
        this->_gameUpdateNetwork.connect();
        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    }
}

void Server::Server::displayRoomsInfos()
{
    for (auto &it : this->_rooms) {
        it->displayInfo();
    }
}

void Server::Server::displayRoomInfos(uint8_t roomNumber)
{
    if (roomNumber >= this->_rooms.size())
        return;
    this->_rooms[roomNumber]->displayInfo();
}

void Server::Server::resetRooms()
{
    for (auto &room : this->_rooms) {
        room->stop();
    }
    for (auto &thread : this->_threads) {
        thread.join();
    }
    this->_rooms.clear();
    this->_threads.clear();
    this->_setupRooms();
}

void Server::Server::resetRoom(uint8_t roomNumber) {}
