//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Base function for the network class
//

#include "Network.hpp"
#include <iostream>
#include <type_traits>
#include "Network/TCP/TCPInfo.hpp"

network::Network::Network(const uint16_t port) : _port(port) {}

network::Network::~Network() {}

template <typename T>
void network::Network::attach(std::function<void(const T&)> callback)
{
    if constexpr(std::is_same_v<T, ClientTCPReceivedInfo>) {
        this->_tcpReceivedCallback = callback;
        return;
    }
    if constexpr(std::is_same_v<T, ConnectionInfo>) {
        this->_connectionCallback = callback;
        return;
    }
}

void network::Network::notify(const NetworkData& data)
{
    std::visit(
        [this](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ClientTCPReceivedInfo>) {
                // this->_tcpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ConnectionInfo>) {
                this->_connectionCallback(arg);
                return;
            }
        },
        data);
}

template void network::Network::attach<network::ClientTCPReceivedInfo>(
    std::function<void(const network::ClientTCPReceivedInfo&)>);
template void network::Network::attach<network::ConnectionInfo>(std::function<void(const network::ConnectionInfo&)>);
