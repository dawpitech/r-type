//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Base function for the network class
//

#include <type_traits>

#include "Network.hpp"
#include "network/datatype.hpp"

network::Network::Network(const uint16_t port) : _port(port) {}

network::Network::~Network() {}

template <typename T> requires network::NetworkDataType<T>
void network::Network::attach(std::function<void(const T&)> callback)
{
    if constexpr(std::is_same_v<T, ClientTCPReceivedInfo>) {
        this->_tcpReceivedCallback = std::move(callback);
        return;
    }
    if constexpr(std::is_same_v<T, ConnectionInfo>) {
        this->_connectionCallback = std::move(callback);
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

template void network::Network::attach<network::ClientTCPReceivedInfo>(
    std::function<void(const network::ClientTCPReceivedInfo&)>);
template void network::Network::attach<network::UDPReceivedInfo>(
    std::function<void(const network::UDPReceivedInfo&)>);
template void network::Network::attach<network::ConnectionInfo>(std::function<void(const network::ConnectionInfo&)>);
