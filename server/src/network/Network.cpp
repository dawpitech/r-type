//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Base function for the network class
//

#include <type_traits>

#include "Network.hpp"
#include "network/datatype.hpp"

network::ServerNetwork::ServerNetwork(const uint16_t port) : _port(port) {}

network::ServerNetwork::~ServerNetwork() {}

void network::ServerNetwork::connect() { this->_ioContext.poll_one(); }

void network::ServerNetwork::notify(const NetworkData& data)
{
    std::visit(
        [this](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ClientTCPReceivedInfo>) {
                if (this->_tcpReceivedCallback)
                    this->_tcpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, UDPReceivedInfo>) {
                if (this->_udpReceivedCallback)
                    this->_udpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ConnectionInfo>) {
                if (this->_connectionCallback)
                    this->_connectionCallback(arg);
                return;
            }
        },
        data);
}
