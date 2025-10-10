//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle single player
//

#pragma once

#include <format>

#include <string>

#include "network/TCP/TCPInfo.hpp"
#include "network/UDP/UDPNetwork.hpp"

namespace game
{
    class Player
    {
        public:
            explicit Player(const network::ConnectionInfo& info, network::UDPNetwork& network) :
                _id(info.uuid), _ip(info.ip), _network(network)
            {}
            ~Player() = default;

            [[nodiscard]] const std::string& getId() const { return this->_id; }
            void storeInfo(const network::ClientTCPReceivedInfo& info)
            {
                utils::Logger::debug(std::format("Player {} on port {}", this->_id, info.portUDP));
                this->_udpPort = info.portUDP;
            }

            void sendData(const std::string& string) { this->_network.sendData(this->_ip, this->_udpPort, string); }

        private:
            network::UDPNetwork& _network;
            std::string _id;
            uint16_t _score = 0;
            uint16_t _udpPort = 0;
            std::string _ip;
    };
} // namespace game
